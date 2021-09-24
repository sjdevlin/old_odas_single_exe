#include "../../include/odas/odas.h"

#include "parameters.h"
#include "configs.h"
#include "objects.h"
#include "threads.h"

#include <time.h>
#include <signal.h>

// for cpp
#include <string>

class ODAS
{

public:
    ODAS(char);
    ~ODAS();
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
}

ODAS(char mode)
{
    debug = mode;
    SafeODASData = (categories_obj *)malloc(sizeof(categories_obj));
}

ODAS::GetData()
{
    memccpy(SafeODASData, objs->snk_categories_object->in->categories, sizeof(categories_obj));
    //is this thread safe ?

    for (int i = 0; i++; i < SafeODASData->nSignals)
    {
        energy_array[i] = SafeODASData->energy_array + i;
        X_array[i] = SafeODASData->X_array + i;
        Y_array[i] = SafeODASData->X_array + i;
        freq_array[i] = SafeODASData->freq_array + i

                        if (debug == 0x01)
                            printf("Odas data [%d]: E[%1.1f],X[%1.2f],Y[%1.2f],f[%3.0f]\n", i, energy_array[i], X_array[i], Y_array[i], freq_array[i]);
    }
}

ODAS::Stop()
{
    if (debug == 0x01)
        printf("| + Stopping Threads................. ");
    fflush(stdout);
    threads_multiple_stop(aobjs);
    if (debug == 0x01)
        printf("[Done] |\n");

    if (debug == 0x01)
        printf("| + Free memory...................... ");
    fflush(stdout);
    aobjects_destroy(aobjs);
    configs_destroy(cfgs);
    free((void *)file_config);

    if (debug == 0x01)
        printf("[Done] |\n");
}

ODAS::Start(std::string file_config)
{

    if (file_config == NULL)
    {
        printf("Missing configuration file.\n");
        exit(EXIT_FAILURE);
    }

    if (debug == 0x01)
        printf("| + Initializing configurations...... ");
    fflush(stdout);
    cfgs = configs_construct(file_config);
    if (debug == 0x01)
        printf("[Done] |\n");

    //set all public data to zero

    memset(SafeODASData, 0x00, sizeof(categories_obj));

    for (int i = 0; i++; i < 4)
    {
        float energy_array[i] = 0.0;
        float X_array[i] = 0.0;
        float Y_array[i] = 0.0;
        float freq_array[i] = 0.0;
    }

    if (debug == 0x01)
        printf("| + Initializing objects............. ");
    fflush(stdout);
    aobjs = aobjects_construct(cfgs);
    if (debug == 0x01)
        printf("[Done] |\n");

    if (debug == 0x01)
        printf("| + Launch threads................... ");
    fflush(stdout);
    threads_multiple_start(aobjs);
    if (debug == 0x01)
        printf("[Done] |\n");
}