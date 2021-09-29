#include <signal.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <mutex>

#include "audio.h"
#include "meetpie.h"
#include "../include/ggk.h"

//cpp class files

char debug_mode = 0x00; // global variable for debugging flag

void signalHandler(int signum)
{
    switch (signum)
    {
    case SIGINT:
        printf("SIGINT recieved, shutting down");
        // sd need to put code in here to free up any memeory and also close file socket
        Odas_obj::Stop(); // sd question for JJ.  Do I need Join here to wait for all threads to stop?
        ggkShutdownAndWait();
        break;
    case SIGTERM:
        Odas_obj::Stop();
        printf("SIGTERM recieved, shutting down");
        ggkShutdownAndWait();
        break;
    }
}

+++

  MEETPIE::MEETPIE()
{

    matrix_bus = new matrix_hal::MatrixIOBus;
    if (!matrix_bus.Init())
        return false;

    matrix_gpio = new matrix_hal::GPIOControl;
    matrix_gpio.Setup(&matrix_bus);
    matrix_gpio.SetMode(start_stop_pin, GPIOInputMode);
    matrix_gpio.SetMode(mode_pin, GPIOInputMode);

    matrix_image1d = new matrix_hal::EverloopImage(matrix_bus.MatrixLeds());
    matrix_everloop = new matrix_hal::Everloop;
    matrix_everloop.Setup(&matrix_bus);

    // switch off all the LEDs

    for (matrix_hal::LedValue &led : matrix_image1d.leds)
    {
        led.red = 0;
        led.green = 0;
        led.blue = 0;
        led.white = 0;
    }

    mode_list[MAXMODES] = [ DARK, SHAREOFVOICE, DEBUG ];
    GPIOInputMode = 0;  // input
    start_stop_pin = 0; // GPIO 0
    mode_pin = 1;       //GPIO 1
    mode = SHAREOFVOICE; // default mode

    num_leds = image1d.leds.size();

}

void MEETPIE::Update(MEETING Meeting_Obj)
{

    if (mode == DEBUG) // light up single LED pointing to each talker
        for (int i = 1; i <= Meeting_Obj.num_participants; i++)
        {
            if (Meeting_Obj.participant[i].is_talking == 1) {
                int led_talker = Meeting_Obj.participant[i].angle * num_leds / 360;
                image1d.leds[led_talker].red = red[i];
                image1d.leds[led_talker].green = green[i];
                image1d.leds[led_talker].blue = blue[i];
                
            }
        }
}

// for cpp

extern char debug_mode;

+++

  int
  main(int argc, char *argv[])
{

    // Command-line parser

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

    // Setup our signal handlers
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    // Create objects
    AUDIO Odas_obj;
    MEETING Meeting_Obj;
    FACILITATOR Facilitator_Obj;

    // Start Bluetooth
    if (!ggkStart("gobbledegook", "Gobbledegook", "Gobbledegook", dataGetter, dataSetter, kMaxAsyncInitTimeoutMS))
        return -1;

    While(1) // consider using done and sigterm
    {
        if (GPIO = true)
        {
            if (Status = "running")
            {
                Odas_obj::Stop();
                Meeting_Obj::WriteToFile();
                Status = "Stopped"
            }
            else if (Status = "Stopped")
            {
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
