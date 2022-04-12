
   /**
    * \file     category.c
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
    
    #include <signal/category.h>

    categories_obj * categories_construct_zero(const unsigned int nSignals) {

        categories_obj * obj;

        obj = (categories_obj *) malloc(sizeof(categories_obj));

        obj->nSignals = nSignals;

        obj->track_id = (unsigned long *) malloc(sizeof(unsigned long) * nSignals);
        memset(obj->track_id, 0x00, sizeof(unsigned long) * nSignals);

        obj->X_array = (float *) malloc(sizeof(float) * nSignals);
        memset(obj->X_array, 0x00, sizeof(float) * nSignals);

        obj->Y_array = (float *) malloc(sizeof(float) * nSignals);
        memset(obj->Y_array, 0x00, sizeof(float) * nSignals);

        obj->freq_array = (float *) malloc(sizeof(float) * nSignals);
        memset(obj->freq_array, 0x00, sizeof(float) * nSignals);



        return obj;

    }

    void categories_destroy(categories_obj * obj) {

        free((void *) obj->track_id);
        free((void *) obj->X_array);
        free((void *) obj->Y_array);
        free((void *) obj->freq_array);
        free((void *) obj);

    }

    void categories_copy(categories_obj * dest, const categories_obj * src) {

        memcpy(dest->track_id, src->track_id, sizeof(unsigned long) * src->nSignals);
        memcpy(dest->X_array, src->X_array, sizeof(float) * src->nSignals);
        memcpy(dest->Y_array, src->Y_array, sizeof(float) * src->nSignals);
        memcpy(dest->freq_array, src->freq_array, sizeof(float) * src->nSignals);

    }

    void categories_zero(categories_obj * obj) {

        memset(obj->track_id, 0x00, sizeof(unsigned long) * obj->nSignals);
        memset(obj->X_array, 0x00, sizeof(float) * obj->nSignals);
        memset(obj->Y_array, 0x00, sizeof(float) * obj->nSignals);
        memset(obj->freq_array, 0x00, sizeof(float) * obj->nSignals);

    }

    void categories_printf(const categories_obj * obj) {

        unsigned int iSignal;

        for (iSignal = 0; iSignal < obj->nSignals; iSignal++) {

            printf("(%02u): %3.2f\n",iSignal,obj->X_array[iSignal]);
            printf("(%02u): %3.2f\n",iSignal,obj->Y_array[iSignal]);
            printf("(%02u): %llu\n",iSignal,obj->track_id[iSignal]);
            printf("(%02u): %3.2f\n",iSignal,obj->freq_array[iSignal]);

        }

    }
