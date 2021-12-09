
   /**
    * \file     snk_categories.c
    * \author   François Grondin <francois.grondin2@usherbrooke.ca>
    * \version  2.0
    * \date     2018-03-18
    * \copyright
    *
    * This program is free software: you can redistribute it and/or modify
    * it under the terms of the GNU General Public License as published by
    * the Free Software Foundation, either version 3 of the License, or
    * (at your option) any later version.
    *
    * This program is distributed in the hope that it will be useful,
    * but WITHOUT ANY WARRANTY; without even the implied warranty of
    * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    * GNU General Public License for more details.
    * 
    * You should have received a copy of the GNU General Public License
    * along with this program.  If not, see <http://www.gnu.org/licenses/>.
    *
    */
    
    #include <sink/snk_categories.h>

    snk_categories_obj * snk_categories_construct(const snk_categories_cfg * snk_categories_config, const msg_categories_cfg * msg_categories_config) {

        snk_categories_obj * obj;

        obj = (snk_categories_obj *) malloc(sizeof(snk_categories_obj));

        obj->timeStamp = 0;

        obj->nChannels = msg_categories_config->nChannels;
        obj->fS = snk_categories_config->fS;

        obj->format = format_clone(snk_categories_config->format);
        obj->interface = interface_clone(snk_categories_config->interface);

        
        obj->safe_buffer = (categories_obj *) malloc(sizeof(categories_obj));
        obj->safe_buffer->energy_array = (float *) malloc(sizeof(float) * msg_categories_config->nChannels);
        obj->safe_buffer->X_array = (float *) malloc(sizeof(float) * msg_categories_config->nChannels);
        obj->safe_buffer->Y_array = (float *) malloc(sizeof(float) * msg_categories_config->nChannels);
        obj->safe_buffer->freq_array = (float *) malloc(sizeof(float) * msg_categories_config->nChannels);

        obj->in = (msg_categories_obj *) NULL;

        return obj;

    }

    void snk_categories_destroy(snk_categories_obj * obj) {

        format_destroy(obj->format);
	printf("try destroy interface\n");
        interface_destroy(obj->interface);
	printf("successfuly destroyed interface\n");

        free((void *) obj->safe_buffer->energy_array);
        free((void *) obj->safe_buffer->X_array);
        free((void *) obj->safe_buffer->Y_array);
        free((void *) obj->safe_buffer->freq_array);
	printf("successfuly destroyed safe buffer\n");

    }

    void snk_categories_connect(snk_categories_obj * obj, msg_categories_obj * in) {

        obj->in = in;

    }

    void snk_categories_disconnect(snk_categories_obj * obj) {

        obj->in = (msg_categories_obj *) NULL;

    }

    void snk_categories_open(snk_categories_obj * obj) {

    
    
    }
 

    void snk_categories_close(snk_categories_obj * obj) {

        }

    int snk_categories_process(snk_categories_obj * obj) {

        int rtnValue;

        if (obj->in->timeStamp != 0) {

            if (obj->in->timeStamp%100 == 0) {

            memcpy(obj->safe_buffer->energy_array,obj->in->categories->energy_array, sizeof(float) * obj->nChannels);
            memcpy(obj->safe_buffer->X_array,obj->in->categories->X_array, sizeof(float) * obj->nChannels);
            memcpy(obj->safe_buffer->Y_array,obj->in->categories->Y_array, sizeof(float) * obj->nChannels);
            memcpy(obj->safe_buffer->freq_array,obj->in->categories->freq_array, sizeof(float) * obj->nChannels);

        }

    }

}
    snk_categories_cfg * snk_categories_cfg_construct(void) {

        snk_categories_cfg * cfg;

        cfg = (snk_categories_cfg *) malloc(sizeof(snk_categories_cfg));

        cfg->fS = 0;
        cfg->format = (format_obj *) NULL;
        cfg->interface = (interface_obj *) NULL;

        return cfg;

    }

    void snk_categories_cfg_destroy(snk_categories_cfg * snk_categories_config) {

        if (snk_categories_config->format != NULL) {
            format_destroy(snk_categories_config->format);
        }
        if (snk_categories_config->interface != NULL) {
            interface_destroy(snk_categories_config->interface);
        }

        free((void *) snk_categories_config);

    }
