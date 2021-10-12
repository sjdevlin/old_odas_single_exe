#ifndef meetpie_h
#define meetpie_h

#include "meeting.h"
// system calls
#include <unistd.h>
// IO streams and functions
#include <iostream>
#include <fstream>

// Matrix GPIO interface
#include "matrixio_bus.h"
// Matrix communication layer
#include "gpio_control.h"

// These are for LED ring
#include "everloop.h"
#include "everloop_image.h"

//#include "../cpp/driver/imu_data.h"
//#include "../cpp/driver/imu_sensor.h"


class MEETPIE
{

public:
    MEETPIE(uint16_t);
    void update(MEETING);
    void write_to_file(MEETING);
    bool button_pressed(uint16_t);

    int mode;
    uint16_t start_stop_pin;
    uint16_t mode_pin;
    uint16_t num_leds;

private:
    matrix_hal::MatrixIOBus matrix_bus;
    matrix_hal::GPIOControl matrix_gpio;
    matrix_hal::EverloopImage matrix_image1d;
    matrix_hal::Everloop matrix_everloop;

    uint16_t* virtual_led_ring;
    uint16_t closest_led[MAXPART];
    uint16_t segment_led[MAXPART];

    int red[MAXPART], blue[MAXPART], green[MAXPART];
    uint16_t gpio_input_mode;

};

#endif
