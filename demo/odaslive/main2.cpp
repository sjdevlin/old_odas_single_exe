#include <signal.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <mutex>

#include "audio.h"
#include "meeting.h"
#include "meetpie.h"
#include "ble.h"
#include "../include/ggk.h"

// These macro definitions are for main loop logic
#define STOPPED 0x00
#define STOP 0x01
#define START 0x02
#define RUNNING 0x03
#define SETMODE 0x04

#define SHORTPRESS 1
#define LONGPRESS 6

#define NUMLEDS 35          // eventually this will go into a parameter file
#define POLLINGFREQ 500000  // also this into a parameter file
#define BUTTONFREQ 250000   // also this into a parameter file
#define TWO_SECONDS 2000000 // also this into a parameter file

char debug_mode = 0x00;       // global variable for debugging flag
char auto_mode = 0x00;        // global variable for auto run without button - used in development only
char shutdown_request = 0x00; // Used in signal handler for orderly shut down on Ctrl C

void signal_handler(int signum)
{
    switch (signum)
    {
    case SIGINT:
        printf("SIGINT recieved, shutting down");
        shutdown_request = 0x01; // JJ check this is OK
        break;
    case SIGTERM:
        printf("SIGTERM recieved, shutting down");
        shutdown_request = 0x01;
        break;
    }
}

int main(int argc, char *argv[])
{
    // Create objects
    AUDIO audio_obj;
    MEETING meeting_obj;
    MEETPIE meetpie_obj(NUMLEDS);
    BLE ble_obj;

    // Command-line parser  // need to add error condition
    int c;
    char *file_config;

    while ((c = getopt(argc, argv, "adc:")) != -1)
    {
        switch (c)
        {
        case 'c':
            file_config = (char *)malloc(sizeof(char) * (strlen(optarg) + 1));
            strcpy(file_config, optarg);
            break;
        case 'd':
            debug_mode = 0x01;
            break;
        case 'a':
            auto_mode = 0x01;
            break;
        }
    }

    // JJ remember to add error codes that bubble up

    // Setup our signal handlers
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    char status = STOPPED; // make sure main loop starts in right mode
    int time_button_pressed = 0;
    unsigned int blink_counter = 0;

    // start up the bluetooth server

    /*    if (!ble_obj.start())  // fatal error if bluetooth is unable to start
        {
            printf("error starting bluetooth");
            exit(0);
        }
    */

    // only for development
    if (auto_mode == 0x01)
        status = START;

    while (shutdown_request != 0x01)
    {
        switch (status)
        {
        case STOPPED:
            meetpie_obj.light_led(STARTLED, RED);

            if (meetpie_obj.button_pressed(meetpie_obj.start_stop_pin))
            {
                ++time_button_pressed;
                if (time_button_pressed >= 16)
                    shutdown_request = 0x01;
            }
            else
            {

                if (time_button_pressed >= LONGPRESS)
                {
                    if (debug_mode == 0x01)
                        printf("Entering Set Mode\n");
                    meetpie_obj.light_led(STARTLED, OFF);
                    status = SETMODE;
                }
                else if (time_button_pressed >= SHORTPRESS)
                {
                    meetpie_obj.light_led(STARTLED, AMBER);
                    status = START;
                }

                time_button_pressed = 0;
            }

            usleep(BUTTONFREQ);
            break;

        case SETMODE:

            if (++blink_counter % 3 == 0)
            {
                blink_counter = 0;
                meetpie_obj.light_led(STARTLED, RED);

                switch (meetpie_obj.mode)
                {
                case DARK:
                    meetpie_obj.light_led(DARKLED, AMBER);
                    break;

                case SHAREOFVOICE:
                    meetpie_obj.light_led(PIELED, AMBER);
                    break;

                case DEBUG:
                    meetpie_obj.light_led(DEBUGLED, AMBER);
                    break;
                }
            }
            else
            {
                meetpie_obj.darken_all_led();
            }

            if (meetpie_obj.button_pressed(meetpie_obj.start_stop_pin))
            {
                ++time_button_pressed;
            }
            else
            {
                if (time_button_pressed >= LONGPRESS)
                {
                    if (debug_mode == 0x01)
                        meetpie_obj.darken_all_led();
                    status = STOPPED;
                }
                else if (time_button_pressed >= SHORTPRESS)
                {
                    meetpie_obj.mode = (meetpie_obj.mode < MAXMODES - 1) ? meetpie_obj.mode + 1 : 0;
                    if (debug_mode == 0x01)
                        printf("Mode changed to %d\n", meetpie_obj.mode);
                }

                time_button_pressed = 0;
            }

            usleep(BUTTONFREQ);

            break;

        case START:

            audio_obj.start(file_config);
            meeting_obj.initialize();

            meetpie_obj.light_led(STARTLED, GREEN);
            usleep(TWO_SECONDS);
            meetpie_obj.light_led(STARTLED, OFF);

            status = RUNNING;
            break;

        case RUNNING:

            audio_obj.get_data();                       // Get Audio source data
            meeting_obj.process_latest_data(audio_obj); // update meeting object
            meetpie_obj.update(meeting_obj);

            //            ble_obj.update(meeting_obj);

            // if room silent for more than a certain time assume meeting over

            if (meeting_obj.total_silence > MAXSILENCE)
            {
                if (debug_mode == 0x01)
                    printf("Stopping due to silence of %d units\n", meeting_obj.total_silence);
                status = STOP;
            }


            usleep(POLLINGFREQ); // This is the crucial delay that determines frequnecy of polling
            if (meetpie_obj.button_pressed(meetpie_obj.start_stop_pin) == 1) {
                            meetpie_obj.darken_all_led();
                            meetpie_obj.light_led(STARTLED, AMBER);
                            status = STOP;
            }

            break;

        case STOP:

            meetpie_obj.write_to_file(meeting_obj);
            audio_obj.stop();

            status = STOPPED;

            break;

        default:
            break;
        }
    }

    if (status == RUNNING) // this means it was a shutdown request from ctrl C
    {
        meetpie_obj.write_to_file(meeting_obj);
        audio_obj.stop();
    }

    free((void *)file_config);
    meetpie_obj.darken_all_led();

    //    ble_obj.stop();  // Stop the ggk server

    exit(0);
}
