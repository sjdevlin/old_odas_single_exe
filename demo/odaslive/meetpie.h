#ifndef meetpie_h
#define meetpie_h

#include "meeting.h"
// system calls
#include <unistd.h>
// IO streams and functions
#include <iostream>

// Matrix GPIO interface
#include "matrixio_bus.h"
// Matrix communication layer
#include "gpio_control.h"

// These are for LED ring
#include "everloop.h"
#include "everloop_image.h"

//#include "../cpp/driver/imu_data.h"
//#include "../cpp/driver/imu_sensor.h"

#define MAXMODES 3
#define DARK 0
#define SHAREOFVOICE 1
#define DEBUG 2

class MEETPIE
{

public:
    MEETPIE();
    ~MEETPIE();
    void update(MEETING);
    void write_to_file(MEETING);
    bool button_pressed(uint16_t);

    int mode;
    uint16_t start_stop_pin;
    uint16_t mode_pin;

private:
    matrix_hal::MatrixIOBus matrix_bus;
    matrix_hal::GPIOControl matrix_gpio;
    matrix_hal::EverloopImage matrix_image1d;
    matrix_hal::Everloop matrix_everloop;

    int red[MAXPART], blue[MAXPART], green[MAXPART];
    int mode_list[MAXMODES];
    uint16_t gpio_input_mode;
    int num_leds; // should be 16 bit ?
};

#endif