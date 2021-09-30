#include "meetpie.h"

//  Main Class Methods for the Meet Pie Device

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
    GPIOInputMode = 0;   // input
    start_stop_pin = 0;  // GPIO 0
    mode_pin = 1;        //GPIO 1
    mode = SHAREOFVOICE; // default mode

    num_leds = image1d.leds.size();
}

void MEETPIE::Update(MEETING Meeting_Obj)
{
    switch (mode)
    {
    case DEBUG:

        for (int i = 1; i <= Meeting_Obj.num_participants; i++)
        {
            if (Meeting_Obj.participant[i].is_talking == 1)
            {
                int led_talker = Meeting_Obj.participant[i].angle * num_leds / 360;
                image1d.leds[led_talker].red = red[i];
                image1d.leds[led_talker].green = green[i];
                image1d.leds[led_talker].blue = blue[i];
            }
        }
        break;

    case SHAREOFVOICE:

        break;

    default:
        break;
    }
}

void MEETPIE::WriteToFile(MEETING MeetingObj)
{
    std::string textline;

    if (Meeting_Obj.num_participants > 0)
    {
        struct tm *timenow;
        std::string filename = "MP_";

        time_t now = time(NULL);
        timenow = gmtime(&now);
        filename += std::to_string(now);

        std::ofstream file(filename);
        //	std::cin >> buffer;

			textline = std::to_string(Meeting_Obj.total_meeting_time)

			for (int i = 1; i < MAXPART; i++)
			{
				textline += ",";
				textline += std::to_string(Meeting_Obj.participant[i].angle);
				textline += ",";
				textline += std::to_string(Meeting_Obj.participant[i].is_talking);
				textline += ",";
				textline += std::to_string(Meeting_Obj.participant[i].num_turns);
				textline += ",";
				textline += std::to_string(Meeting_Obj.participant[i].total_talk_time);
            }

        file << textline;

        file.close();
    }
}