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
    gpio_input_mode = 0;   // input
    start_stop_pin = 0;  // GPIO 0
    mode_pin = 1;        //GPIO 1
    mode = SHAREOFVOICE; // default mode

    num_leds = image1d.leds.size();
}

void MEETPIE::update(MEETING meeting_obj)
{
    switch (mode)
    {
    case DEBUG:

        for (int i = 1; i <= meeting_obj.num_participants; i++)
        {
            if (meeting_obj.participant[i].is_talking == 1)
            {
                int led_talker = meeting_obj.participant[i].angle * num_leds / 360;
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

void MEETPIE::write_to_file(MEETING meeting_obj)
{
    std::string text_line;

    if (meeting_obj.num_participants > 0)
    {
        struct tm *timenow;
        std::string filename = "MP_";

        time_t now = time(NULL);
        timenow = gmtime(&now);
        filename += std::to_string(now);

        std::ofstream file(filename);
        //	std::cin >> buffer;

			text_line = std::to_string(meeting_obj.total_meeting_time)

			for (int i = 1; i < MAXPART; i++)
			{
				text_line += ",";
				text_line += std::to_string(meeting_obj.participant[i].angle);
				text_line += ",";
				text_line += std::to_string(meeting_obj.participant[i].is_talking);
				text_line += ",";
				text_line += std::to_string(meeting_obj.participant[i].num_turns);
				text_line += ",";
				text_line += std::to_string(meeting_obj.participant[i].total_talk_time);
            }

        file << text_line;

        file.close();
    }
}

bool MEETPIE::button_pressed(uint16_t pin)
{
return (matrix_gpio.GetGPIOValue(pin));
}