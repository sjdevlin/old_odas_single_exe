#ifndef ble_h
#define ble_h

#include <signal.h>
#include <iostream>
#include <thread>
#include <sstream>
#include <mutex>
#include <fstream>

#include "meeting.h"
#include "../include/ggk.h"

class BLE
{

public:
    BLE();
    ~BLE();
    int Start();
    void Stop();
    void Update(MEETING);
    const static void * dataGetter(const char *);
    static int dataSetter(const char *, const void *);

    //mutex so that we can multi-thread
    static std::mutex mutex_buffer;
    // The battery level ("battery/level") reported by the server (see Server.cpp)
    static uint8_t serverDataBatteryLevel;
    // The text string ("text/string") used by our custom text string service (see Server.cpp)
    static std::string serverDataTextString;

private:
    // Maximum time to wait for any single async process to timeout during initialization
    static int kMaxAsyncInitTimeoutMS;
};

#endif