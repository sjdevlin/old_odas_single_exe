#ifndef audio_h
#define audio_h

#include "../../include/odas/odas.h"

#include "parameters.h"
#include "configs.h"
#include "objects.h"
#include "threads.h"

#include <time.h>
#include <signal.h>

// for cpp
#include <string>

extern char debug_mode;

class AUDIO
{

public:
    AUDIO();
    ~AUDIO();
    void start(char *);
    void stop(char *);
    void get_data();

    float energy_array[4];
    float x_array[4];
    float y_array[4];
    float freq_array[4];
    categories_obj *safe_odas_data;

private:
    objects *objs;
    aobjects *aobjs;
    configs *cfgs;
    char debug;
};

#endif