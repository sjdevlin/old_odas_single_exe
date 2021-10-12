#include "meetpie.h"

//  Main Class Methods for the Meet Pie Device

MEETPIE::MEETPIE(uint16_t leds) : matrix_image1d(leds)
{

    gpio_input_mode = 0; // input
    start_stop_pin = 0;  // GPIO 0
    mode_pin = 1;        //GPIO 1
    mode = SHAREOFVOICE; // default mode for now (will change to dark for produciton)
    num_leds = leds;     // This allows both Matrix Creator (32 Leds) and Voice (18 Leds) can be used

    int red[MAXPART] = {0, 200, 0, 0, 100, 0, 100, 100}; // Array to store unique colour for each participant
    int green[MAXPART] = {0, 0, 200, 0, 100, 100, 0, 50};
    int blue[MAXPART] = {0, 0, 0, 150, 0, 100, 100, 50};

    if (!matrix_bus.Init())
        printf("Bus did not load \n"); // Question for JJ.  Advice for error handling...

    // Initialize the Matrix Hal Bus and set GPI pins to input
    matrix_gpio.Setup(&matrix_bus);
    matrix_gpio.SetMode(start_stop_pin, 0); // 0 sets pin for input
    matrix_gpio.SetMode(mode_pin, 0);       // 0 sets pin for input
    matrix_everloop.Setup(&matrix_bus);

    // these variables are for share of voice led display
    virtual_led_ring = malloc(uint16_t, num_leds);
    memset(virtual_led_ring, 0, num_leds);
    uint16_t closest_led[MAXPART] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint16_t segment_led[MAXPART] = {0, 0, 0, 0, 0, 0, 0, 0};

    // now switch off all the LEDs
    for (matrix_hal::LedValue &led : matrix_image1d.leds)
    {
        led.red = 0;
        led.green = 0;
        led.blue = 0;
        led.white = 0;
    }

    matrix_everloop.Write(&matrix_image1d);
}

void MEETPIE::update(MEETING meeting_obj)
{

    switch (mode)
    {
    case DEBUG:

        // debug mode just lights the closest led of each active talker

        for (int i = 1; i <= meeting_obj.num_participants; i++)
        {

            closest_led[i] = meeting_obj.participant[i].angle * num_leds / 360; // this is not optimal will look to stoare in a vector eventually

            if (meeting_obj.participant[i].is_talking == 1)
            {
                matrix_image1d.leds[closest_led[i]].red = red[i];
                matrix_image1d.leds[closest_led[i]].green = green[i];
                matrix_image1d.leds[closest_led[i]].blue = blue[i];
            }
            else
            {
                matrix_image1d.leds[closest_led[i]].red = 0;
                matrix_image1d.leds[closest_led[i]].green = 0;
                matrix_image1d.leds[closest_led[i]].blue = 0;
            }
        }
        
        matrix_everloop.Write(&matrix_image1d);

        break;

    case SHAREOFVOICE:

        // calculate share of LEDS

        if (meeting_obj.total_talk_time % 20 == 0) // this section fairly long so only do every 20 cycles
        {
            uint16_t num_leds_per_participant;
            uint16_t next_person;
            uint16_t last_led = 0;

            // build the pie from 0 degrees in clockwise order
            // we will offset to center on the talker later

            for (int i = 1; i <= meeting_obj.num_participants; i++)

            {
                closest_led[i] = meeting_obj.participant[i].angle * num_leds / 360;

                next_person = meeting_obj.participant_clockface[i]; // this means we are starting at the talker closest to 0

                num_leds_per_participant = num_leds * meeting_obj.total_talk_time / meeting_obj.participant[next_person].total_talk_time;

                for (int j = 0; j < num_leds_per_participant; j++)
                {
                    // this finds the centre of the segment - needed for centering the segment on the active talker
                    if (j == num_leds_per_participant / 2)
                        segment_led[i] = last_led + j;
                    virtual_led_ring[last_led] = next_person;
                    ++last_led;
                }
            }

            // now we will work out how much to rotate the array to be centered on the current speaker
            int offset = closest_led[meeting_obj.last_talker] - segment_led[meeting_obj.last_talker];

            for (int i = 0; i < num_leds; i++)
            {
                if (i + offset >= 0)
                {
                    if (i + offset < num_leds)
                    {
                        matrix_image1d.leds[i].red = red[virtual_led_ring[i+offset]];
                        matrix_image1d.leds[i].green = green[virtual_led_ring[i+offset]];
                        matrix_image1d.leds[i].blue = blue[virtual_led_ring[i+offset]];
                    }
                    else
                    {
                        matrix_image1d.leds[i].red = red[virtual_led_ring[i+offset-num_leds]];
                        matrix_image1d.leds[i].green = green[virtual_led_ring[i+offset-num_leds]];
                        matrix_image1d.leds[i].blue = blue[virtual_led_ring[i+offset-num_leds]];                    }
                }
                else
                {
                        matrix_image1d.leds[i].red = red[virtual_led_ring[i+offset+num_leds]];
                        matrix_image1d.leds[i].green = green[virtual_led_ring[i+offset+num_leds]];
                        matrix_image1d.leds[i].blue = blue[virtual_led_ring[i+offset+num_leds]];                    }
                }
            }
        }

        matrix_everloop.Write(&matrix_image1d);
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

        text_line = std::to_string(meeting_obj.total_meeting_time);

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
    bool temp;
    temp = matrix_gpio.GetGPIOValue(pin);
    printf("button %d: %d \n", pin, temp);
    return temp;
}
