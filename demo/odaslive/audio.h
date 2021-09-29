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
    void Start(std::string, char);
    void Stop();
    void GetData();

    float energy_array[4];
    float X_array[4];
    float Y_array[4];
    float freq_array[4];

private:
    objects *objs;
    aobjects *aobjs;
    configs *cfgs;
    categories_obj *SafeODASData;
    char debug;
};

#endif