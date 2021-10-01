#include "audio.h"

AUDIO::AUDIO()
{
    safe_odas_data = (categories_obj *)malloc(sizeof(categories_obj));
    memset(safe_odas_data, 0x00, sizeof(categories_obj));
}

void AUDIO::get_data()
{
    safe_odas_data->nSignals = objs->snk_categories_object->in->categories->nSignals; // need to optimise here lots of waste
    memcpy(safe_odas_data->energy_array, objs->snk_categories_object->in->categories->energy_array, sizeof(float) * safe_odas_data->nSignals);
    memcpy(safe_odas_data->X_array, objs->snk_categories_object->in->categories->X_array, sizeof(float) * safe_odas_data->nSignals);
    memcpy(safe_odas_data->Y_array, objs->snk_categories_object->in->categories->Y_array, sizeof(float) * safe_odas_data->nSignals);
    memcpy(safe_odas_data->freq_array, objs->snk_categories_object->in->categories->freq_array, sizeof(float) * safe_odas_data->nSignals);
    //is this thread safe ?

    for (int i = 0; i++; i < safe_odas_data->nSignals)
    {
        energy_array[i] = safe_odas_data->energy_array[i];
        y_array[i] = safe_odas_data->X_array[i];
        y_array[i] = safe_odas_data->Y_array[i];
        freq_array[i] = safe_odas_data->freq_array[i];

    if (debug_mode == 0x01)
        printf("Odas data [%d]: E[%1.1f],X[%1.2f],Y[%1.2f],f[%3.0f]\n", i, energy_array[i], x_array[i], y_array[i], freq_array[i]);
    }
}

void AUDIO::stop(char * file_config)
{
    if (debug_mode == 0x01)
        printf("| + Stopping Threads.................  \n ");
    threads_multiple_stop(aobjs);

    if (debug_mode == 0x01)
        printf("[Done] |\n");

    if (debug == 0x01)
        printf("| + Free memory......................  \n");

    aobjects_destroy(aobjs);
    configs_destroy(cfgs);
    free((void *)file_config);

    if (debug == 0x01)
        printf("[Done] |\n");
}

void AUDIO::start(char * file_config)
{

    if (file_config == NULL)
    {
        printf("Missing configuration file.\n");
        exit(EXIT_FAILURE);  // change to bubble this up so we can shut down ble etc.
    }

    if (debug == 0x01)
        printf("| + Initializing configurations...... \n");

    cfgs = configs_construct(file_config);

    if (debug == 0x01)
        printf("[Done] |\n");

    if (debug == 0x01)
        printf("| + Initializing objects............. \n");

    aobjs = aobjects_construct(cfgs);

    if (debug == 0x01)
        printf("[Done] |\n");

    if (debug == 0x01)
        printf("| + Launch threads................... \n");

    threads_multiple_start(aobjs);
 
    if (debug == 0x01)
        printf("[Done] |\n");
}