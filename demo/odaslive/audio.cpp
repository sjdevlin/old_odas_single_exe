#include "audio.h"

AUDIO::AUDIO()
{
    SafeODASData = (categories_obj *)malloc(sizeof(categories_obj));
    memset(SafeODASData, 0x00, sizeof(categories_obj));
}

void AUDIO::GetData()
{
    SafeODASData->nSignals = objs->snk_categories_object->in->categories->nSignals; // need to optimise here lots of waste
    memcpy(SafeODASData->energy_array, objs->snk_categories_object->in->categories->energy_array, sizeof(float) * SafeODASData->nSignals);
    memcpy(SafeODASData->X_array, objs->snk_categories_object->in->categories->X_array, sizeof(float) * SafeODASData->nSignals);
    memcpy(SafeODASData->Y_array, objs->snk_categories_object->in->categories->Y_array, sizeof(float) * SafeODASData->nSignals);
    memcpy(SafeODASData->freq_array, objs->snk_categories_object->in->categories->freq_array, sizeof(float) * SafeODASData->nSignals);
    //is this thread safe ?

    for (int i = 0; i++; i < SafeODASData->nSignals)
    {
        energy_array[i] = SafeODASData->energy_array[i];
        X_array[i] = SafeODASData->X_array[i];
        Y_array[i] = SafeODASData->X_array[i];
        freq_array[i] = SafeODASData->freq_array[i];

    if (debug_mode == 0x01)
        printf("Odas data [%d]: E[%1.1f],X[%1.2f],Y[%1.2f],f[%3.0f]\n", i, energy_array[i], X_array[i], Y_array[i], freq_array[i]);
    }
}

void AUDIO::Stop(char * file_config)
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

void AUDIO::Start(char * file_config)
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