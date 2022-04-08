#include "meetpie.h"

//  Main Class Methods for the Meet Pie Device

MEETPIE::MEETPIE(uint16_t leds) : matrix_image1d(leds)
{

    gpio_input_mode = 0; // input
    start_stop_pin = 0;  // GPIO 0
    mode_pin = 1;        // GPIO 1
    mode = DEBUG; // default mode for now (will change to dark for produciton)
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
    for (int i = 0; i <= 40; i++)
    {
        // For each led in everloop_image.leds, set led value
        for (matrix_hal::LedValue &led : matrix_image1d.leds)
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
        matrix_everloop.Write(&matrix_image1d);

        // Sleep for 40000 microseconds
        usleep(40000);
    }

    for (int i = 0; i <= 80; i++)
    {
        // For each led in everloop_image.leds, set led value
        for (matrix_hal::LedValue &led : matrix_image1d.leds)
        {
            // Sine waves 120 degrees out of phase for rainbow
            if (led.red > 0)
                --led.red;
            if (led.green > 0)
                --led.green;
            if (led.blue > 0)
                --led.blue;
            matrix_everloop.Write(&matrix_image1d);

            // Sleep for 40000 microseconds
            usleep(500);
        }
    }

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
            closest_led[i] = (meeting_obj.participant[i].angle * num_leds / 360);

            if (debug_mode == 0x01)
                printf(" led: %d    angle: %d \n", closest_led[i], meeting_obj.participant[i].angle);

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

        if (meeting_obj.num_participants > 0 && meeting_obj.total_talk_time % 5 == 0) // this section fairly long so only do every 20 cycles
        {

            uint16_t num_leds_for_seat_position;
            uint16_t seat_position;
            uint16_t last_led = 0;

            // build the pie from 0 degrees in clockwise order
            // we will offset to center on the talker later

            for (int i = 1; i <= meeting_obj.num_participants; i++)

            {
                closest_led[i] = meeting_obj.participant[i].angle * num_leds / 360; // dont need to do every time !
                seat_position = meeting_obj.participant_clock_order[i];               // this means we are starting at the talker closest to 0
                num_leds_for_seat_position = num_leds * meeting_obj.participant[seat_position].total_talk_time / meeting_obj.total_talk_time;

                for (int j = 0; j < num_leds_for_seat_position; j++)
                {
                    // this finds the centre of the segment - needed for centering the segment on the active talker
                    if (j == (num_leds_for_seat_position / 2))
                    {
                        segment_led[seat_position] = last_led;
                        if (debug_mode == 0x01) printf ("Person in position %d has segment led : %d\n", i, segment_led[seat_position]);
                    }
                    virtual_led_ring[last_led] = seat_position;
                    ++last_led;
                }

                if (debug_mode == 0x01)
                {
                    printf("closest led for p%d is %d, ", i, closest_led[i]);
                    printf("in position %d, ", seat_position);
                    printf("total talk %d, perp talk %d\n ", meeting_obj.total_talk_time,
                           meeting_obj.participant[i].total_talk_time);
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
                printf("last talker %d, ", meeting_obj.last_talker);

            int offset = closest_led[meeting_obj.last_talker] - segment_led[meeting_obj.last_talker];

            if (debug_mode == 0x01)
                printf("offset %d \n", offset);

            for (int i = 0; i < num_leds; i++)
            {
                if (i + offset >= 0)
                {
                    if (i + offset < num_leds)
                    {
                        matrix_image1d.leds[i + offset].red = red[virtual_led_ring[i]];
                        matrix_image1d.leds[i + offset].green = green[virtual_led_ring[i]];
                        matrix_image1d.leds[i + offset].blue = blue[virtual_led_ring[i]];
                    }
                    else
                    {
                        matrix_image1d.leds[i + offset - num_leds].red = red[virtual_led_ring[i]];
                        matrix_image1d.leds[i + offset - num_leds].green = green[virtual_led_ring[i]];
                        matrix_image1d.leds[i + offset - num_leds].blue = blue[virtual_led_ring[i]];
                    }
                }
                else
                {
                    matrix_image1d.leds[i + offset + num_leds].red = red[virtual_led_ring[i]];
                    matrix_image1d.leds[i + offset + num_leds].green = green[virtual_led_ring[i]];
                    matrix_image1d.leds[i + offset + num_leds].blue = blue[virtual_led_ring[i]];
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

    if (debug_mode == 0x01)
        printf("writing to file\n");

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
    else
    {
        if (debug_mode == 0x01)
            printf("No participnts\n");
    }

    // switch off any lights

    for (int i = 0; i <= 255; i++)
    {
        // For each led in everloop_image.leds, set led value
        for (matrix_hal::LedValue &led : matrix_image1d.leds)
        {
            // Sine waves 120 degrees out of phase for rainbow
            if (led.red > 0)
                --led.red;
            if (led.green > 0)
                --led.green;
            if (led.blue > 0)
                --led.blue;
            matrix_everloop.Write(&matrix_image1d);

            // Sleep for 100 microseconds
            usleep(100);
        }
    }
}

bool MEETPIE::button_pressed(uint16_t pin)
{
    bool temp;
    temp = matrix_gpio.GetGPIOValue(pin);
    //    printf("button %d: %d \n", pin, temp);
    return temp;
}
