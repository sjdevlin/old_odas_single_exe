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

#define MAXPART 8
#define MAXMODES 3
#define DARK 0
#define SHAREOFVOICE 1
#define DEBUG 2

#define STARTLED 9
#define MODELED 12
#define DARKLED 15
#define PIELED 18
#define DEBUGLED 21

#define RED 0x01
#define AMBER 0x02
#define GREEN 0x03
#define OFF 0x00



class MEETPIE
{

public:
    MEETPIE(uint16_t);
    void update(MEETING);
    void write_to_file(MEETING);
    bool button_pressed(uint16_t);
    void light_led(uint16_t, char);
    void darken_all_led();



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
    uint16_t* closest_led;
    uint16_t* segment_led;

    const int red[MAXPART] = {0, 200, 0, 0, 100, 0, 100, 100}; // Array to store unique colour for each participant
    const int green[MAXPART] = {0, 0, 200, 0, 100, 100, 0, 50};
    const int blue[MAXPART] = {0, 0, 0, 150, 0, 100, 100, 50};

    uint16_t gpio_input_mode;

};

#endif
