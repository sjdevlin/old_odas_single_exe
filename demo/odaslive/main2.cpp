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
#define NUMLEDS 18

char debug_mode = 0x00; // global variable for debugging flag
char shutdown_request = 0x00;   // Used in signal handler for orderly shut down on Ctrl C

void signal_handler(int signum)
{
    switch (signum)
    {
    case SIGINT:
        printf("SIGINT recieved, shutting down");
        shutdown_request = 0x01;
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

    while ((c = getopt(argc, argv, "c:d")) != -1)
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
        }
    }

    // remember to add error codes that bubble up

    // Setup our signal handlers
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    char status = STOPPED; // make sure main loop starts in right mode
    char start_stop_pressed = 0x00;
    char mode_pressed = 0x00;

    // start up bluetooth server
/*    if (!ble_obj.start())
    {
        printf("error starting bluetooth");
        exit(0);
    }
*/
    while (shutdown_request != 0x01)
    {
        switch (status)
        {
        case STOPPED:
            while (meetpie_obj.button_pressed(meetpie_obj.start_stop_pin))
            {
                status = START;
            } // put in logic to stop repeating
            while (meetpie_obj.button_pressed(meetpie_obj.mode_pin))
            {
                mode_pressed = 0x01;
            }
            if (mode_pressed == 0x01)
            {
                meetpie_obj.mode = (meetpie_obj.mode < MAXMODES - 1) ? meetpie_obj.mode + 1 : 0;
                mode_pressed = 0x00;
                if (debug_mode == 0x01) printf ("Mode changed to %d\n",meetpie_obj.mode);
            }
            sleep(1);
            break;

        case START:
            printf ("starting odas\n");
            audio_obj.start(file_config);
            printf("initializing meeting\n");
            meeting_obj.initialize();
            printf("temporary wait to ensure all odas threads are running\n");
            sleep(5);
            status = RUNNING;
            break;

        case RUNNING:

            sleep(1); // This is the crucial delay that determines frequnecy of polling
            if (meetpie_obj.button_pressed(meetpie_obj.start_stop_pin) == 1)
                status = STOP;

            audio_obj.get_data();                      // Get Audio source data
            meeting_obj.process_latest_data(audio_obj); // update meeting object
            meetpie_obj.update(meeting_obj);
            ble_obj.update(meeting_obj);

            // if room silent for more than a certain time assume meeting over

            if (meeting_obj.total_silence > MAXSILENCE)
                status=STOP;

            break;

        case STOP:

            meetpie_obj.write_to_file(meeting_obj);
            audio_obj.stop(file_config);
            status = STOPPED;

            break;

        default:
            break;
        }
    }

    // shutting down
    // first write summary to file
    meetpie_obj.write_to_file(meeting_obj);

    // stop threads and clean up audio objects
    audio_obj.stop(file_config);
 
    ble_obj.stop();
 
    exit(0);
}
