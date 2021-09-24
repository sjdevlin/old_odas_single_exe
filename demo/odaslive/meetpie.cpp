#include <signal.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <mutex>

#include "meeting.h"
#include "odas_public.h"
#include "../include/ggk.h"

void signalHandler(int signum)
{
    switch (signum)
    {
    case SIGINT:
        printf("SIGINT recieved, shutting down");
        // sd need to put code in here to free up any memeory and also close file socket
        Odas_obj::Stop();  // sd question for JJ.  Do I need Join here to wait for all threads to stop?
        ggkShutdownAndWait();
        break;
    case SIGTERM:
        Odas_obj::Stop();
        printf("SIGTERM recieved, shutting down");
        ggkShutdownAndWait();
        break;
    }
}

int main(int argc, char **ppArgv)
{
    char Debug = 0x00;

    // Command-line parser
    file_config = (char *)NULL;
    char verbose = 0x00;
    while ((c = getopt(argc, argv, "c:d")) != -1)
    {

        switch (c)
        {
        case 'c':
            file_config = (char *)malloc(sizeof(char) * (strlen(optarg) + 1));
            strcpy(file_config, optarg);
            break;
        case 'd':
            Debug = 0x01;
            break;
        }

    }

    // Setup our signal handlers
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    // Create objects
    ODAS Odas_obj(Debug);
    MEETING Meeting_Obj(Debug);

    // Start Bluetooth
    if (!ggkStart("gobbledegook", "Gobbledegook", "Gobbledegook", dataGetter, dataSetter, kMaxAsyncInitTimeoutMS))
        return -1;

    While(1) // consider using done and sigterm
    {
        if (GPIO = true)
        {
            if (Status = "running") {
            Odas_obj::Stop();
            Meeting_Obj::WriteToFile();
                Status = "Stopped"
        } else if (Status = "Stopped"){

            
        }

        if (Status = "start")
        {
            Odas_obj::Start();
            Meeting_Obj::Initialize();
            Status = "run"
        }

        // every .5 sec get new odas data
        Odas_obj::GetData();
        Meeting_Obj::ProcessLatestData(Odas_obj)
        // ask JJ about how to make this more event driven with call backs etc.
    }
}
