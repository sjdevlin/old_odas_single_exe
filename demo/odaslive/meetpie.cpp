#include "meetpie.h"

//  Main Class Methods for the Meet Pie Device

MEETPIE::MEETPIE(uint16_t leds) : matrix_image1d(leds)
{

    gpio_input_mode = 0; // input
    start_stop_pin = 0;  // GPIO 0
    mode_pin = 1;        // GPIO 1
    mode = DEBUG;        // default mode for now (will change to dark for produciton)
    num_leds = leds;     // This allows both Matrix Creator (32 Leds) and Voice (18 Leds) can be used

    if (!matrix_bus.Init())
        printf("Bus did not load \n"); // Question for JJ.  Advice for error handling...

    // Initialize the Matrix Hal Bus and set GPI pins to input
    matrix_gpio.Setup(&matrix_bus);
    matrix_gpio.SetMode(start_stop_pin, 0); // 0 sets pin for input
    matrix_gpio.SetMode(mode_pin, 0);       // 0 sets pin for input
    matrix_everloop.Setup(&matrix_bus);

    // these variables are for share of voice led display
    virtual_led_ring = (uint16_t *)malloc(num_leds * sizeof(uint16_t));
    memset(virtual_led_ring, 0, num_leds * sizeof(uint16_t));
    closest_led = (uint16_t *)malloc(MAXPART * sizeof(uint16_t));
    memset(closest_led, 0, MAXPART * sizeof(uint16_t));
    segment_led = (uint16_t *)malloc(MAXPART * sizeof(uint16_t));
    memset(segment_led, 0, MAXPART * sizeof(uint16_t));

    // light up and then switch off all the LEDs
    for (matrix_hal::LedValue &led : matrix_image1d.leds)
    {
        led.red = 0;
        led.green = 0;
        led.blue = 0;
        led.white = 0;
    }

    float counter = 0;
    const float freq = 0.375;

    // 10 sec loop for rainbow effect 250*40000 microsec = 10 sec
    for (int i = 0; i <= 10; i++)
    {
        // For each led in everloop_image.leds, set led value
        for (matrix_hal::LedValue &led : everloop_image.leds)
        {
            // Sine waves 120 degrees out of phase for rainbow
            led.red =
                (std::sin(freq * counter + (M_PI / 180 * 240)) * 155 + 100) / 10;
            led.green =
                (std::sin(freq * counter + (M_PI / 180 * 120)) * 155 + 100) / 10;
            led.blue = (std::sin(freq * counter + 0) * 155 + 100) / 10;
            // If MATRIX Creator, increment by 0.51
            if (num_leds == 35)
            {
                counter = counter + 0.51;
            }
            // If MATRIX Voice, increment by 1.01
            if (num_leds == 18)
            {
                counter = counter + 1.01;
            }
        }

        // Updates the LEDs
        everloop.Write(&everloop_image);

        // Sleep for 40000 microseconds
        usleep(100000);
    }

    for (matrix_hal::LedValue &led : matrix_image1d.leds)
    {
        led.red = 0;
        led.green = 0;
        led.blue = 0;
        led.white = 0;
        usleep(100000);
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

                closest_led[i] = num_leds - (meeting_obj.participant[i].angle * num_leds / 360); // this is not optimal will look to stoare in a vector eventually

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

            if (meeting_obj.num_participants > 0 && meeting_obj.total_talk_time % 10 == 0) // this section fairly long so only do every 20 cycles
            {

                uint16_t num_leds_per_participant;
                uint16_t next_person;
                uint16_t last_led = 0;

                // build the pie from 0 degrees in clockwise order
                // we will offset to center on the talker later

                for (int i = 1; i <= meeting_obj.num_participants; i++)

                {
                    closest_led[i] = meeting_obj.participant[i].angle * num_leds / 360;
                    if (debug_mode == 0x01)
                        printf("closest led for talker i %d is %d\n", i, closest_led[i]);

                    next_person = meeting_obj.participant_clock_order[i]; // this means we are starting at the talker closest to 0
                    if (debug_mode == 0x01)
                        printf("person in position %d is %d\n", i, next_person);

                    printf("total talk %d, perp total talk %d, num_leds %d\n", meeting_obj.total_talk_time,
                           meeting_obj.participant[next_person].total_talk_time, num_leds);

                    num_leds_per_participant = num_leds * meeting_obj.participant[next_person].total_talk_time / meeting_obj.total_talk_time;
                    if (debug_mode == 0x01)
                        printf("num led for talker i %d is %d\n", i, num_leds_per_participant);

                    for (int j = 0; j < num_leds_per_participant; j++)
                    {
                        // this finds the centre of the segment - needed for centering the segment on the active talker
                        if (j == (num_leds_per_participant / 2))
                            segment_led[next_person] = last_led + j;
                            virtual_led_ring[last_led] = next_person;
                        ++last_led;
                    }
                }

                if (debug_mode == 0x01)
                {
                    printf("virtual led ring = ");
                    for (int i = 0; i < num_leds; i++)
                        printf("%d ", virtual_led_ring[i]);
                    printf("\n");
                }

                // now we will work out how much to rotate the array to be centered on the current speaker

                if (debug_mode == 0x01)
                    printf("last talker %d", meeting_obj.last_talker);
                int offset = closest_led[meeting_obj.last_talker] - segment_led[meeting_obj.last_talker];
                if (debug_mode == 0x01)
                    printf("offset %d", offset);

                for (int i = 0; i < num_leds; i++)
                {
                    if (i + offset >= 0)
                    {
                        if (i + offset < num_leds)
                        {
                            matrix_image1d.leds[i].red = red[virtual_led_ring[i + offset]];
                            matrix_image1d.leds[i].green = green[virtual_led_ring[i + offset]];
                            matrix_image1d.leds[i].blue = blue[virtual_led_ring[i + offset]];
                        }
                        else
                        {
                            matrix_image1d.leds[i].red = red[virtual_led_ring[i + offset - num_leds]];
                            matrix_image1d.leds[i].green = green[virtual_led_ring[i + offset - num_leds]];
                            matrix_image1d.leds[i].blue = blue[virtual_led_ring[i + offset - num_leds]];
                        }
                    }
                    else
                    {
                        matrix_image1d.leds[i].red = red[virtual_led_ring[i + offset + num_leds]];
                        matrix_image1d.leds[i].green = green[virtual_led_ring[i + offset + num_leds]];
                        matrix_image1d.leds[i].blue = blue[virtual_led_ring[i + offset + num_leds]];
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
        //    printf("button %d: %d \n", pin, temp);
        return temp;
    }
