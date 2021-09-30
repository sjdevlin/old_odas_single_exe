#include <signal.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <mutex>

#include "meetpie.h"
#include "meeting.h"
#include "audio.h"
#include "ble.h"
#include "../include/ggk.h"

#define STOPPED 0x00
#define STOP 0x01
#define START 0x02
#define RUNNING 0x03

char debug_mode = 0x00; // global variable for debugging flag
char shutdown = 0x00;   // Used for orderly shut down on Ctrl C

void signalHandler(int signum)
{
    switch (signum)
    {
    case SIGINT:
        printf("SIGINT recieved, shutting down");
        shutdown = 0x01;
        break;
    case SIGTERM:
        printf("SIGTERM recieved, shutting down");
        shutdown = 0x01;
        break;
    }
}

int main(int argc, char *argv[])
{
    // Create objects
    AUDIO Odas_obj;
    MEETING Meeting_Obj;
    MEETPIE MeetPie_Obj;
    BLE Ble_Obj;

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
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    char status = STOPPED; // make sure main loop starts in right mode
    char start_stop_pressed = 0x00;
    char mode_pressed = 0x00;

    // start up bluetooth server
    if (!Ble_Obj.Start())
    {
        printf("error starting bluetooth");
        exit(0);
    }

    while (shutdown != 0x01)
    {
        switch (status)
        {
        case STOPPED:
            while (MeetPie_Obj.ButtonPressed(MeetPie_Obj.start_stop_pin) == 1)
            {
                status = START;
            } // put in logic to stop repeating
            while (MeetPie_Obj.ButtonPressed(MeetPie_Obj.mode_pin) == 1)
            {
                mode_pressed = 0x01;
            }
            if (mode_pressed == 0x01)
            {
                MeetPie_Obj.mode = (MeetPie_Obj.mode < MAXMODES - 1) ? MeetPie_Obj.mode + 1 : 0;
                mode_pressed = 0x00;
            }
            sleep(.1);
            break;

        case START:
            Odas_obj.Start(file_config);
            Meeting_Obj.Initialize();
            printf("temporary wait to ensure all odas threads are running");
            sleep(5);
            status = RUNNING;
            break;

        case RUNNING:
            sleep(.5); // This is the crucial delay that determines frequnecy of polling
            if (MeetPie_Obj.ButtonPressed(MeetPie_Obj.start_stop_pin) == 1)
                status = STOP;

            Odas_obj.GetData();                      // Get Audio source data
            Meeting_Obj.ProcessLatestData(Odas_obj); // update meeting object
            Ble_Obj.Update(Meeting_Obj);

            if (Meeting_Obj.total_silence > MAXSILENCE)
            {
                status=STOP;
            }
            break;

        case STOP:
            Odas_obj.Stop(file_config);
            MeetPie_Obj.WriteToFile(Meeting_Obj);
            status = STOPPED;

            break;

        default:
            break;
        }
    }

    Odas_obj.Stop(file_config);
    Odas_obj.~AUDIO();

    MeetPie_Obj.WriteToFile(Meeting_Obj);
 
    Meeting_Obj.~MEETING();

    MeetPie_Obj.~MEETPIE();

    Ble_Obj.Stop();
    Ble_Obj.~BLE();

    exit(0);
}