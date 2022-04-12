#ifndef __ODAS_SYSTEM_PITCH2CATEGORY
#define __ODAS_SYSTEM_PITCH2CATEGORY

   /**
    * \file     pitch2category.h
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

    #include <stdlib.h>
    #include <string.h>
    #include <stdio.h>
    #include <math.h>

    #include <signal/category.h>
    #include <signal/pitch.h>
    #include <signal/track.h>
    
    typedef struct pitch2category_obj {

        unsigned int nSeps;

        float tauMin;
        float tauMax;
        float tauVarMin;
        float tauVarMax;
        float activityVarMin;
        float activityVarMax;
        float harmonicMin;
        float harmonicMax;
        unsigned int samplesMin;
        unsigned int samplesMax;
        float activityMin;
        float acorrMin;

        unsigned int classificationPeriod;
        unsigned int * processingTime;

        unsigned int * numPitchValues;
        unsigned int ** activityArray; // 2D array
        unsigned int * activityTotal; // 1D array
        unsigned int ** pitchArray; // 2D array
        unsigned int * pitchTotal; // 1D array
        float        * harmonicAcorrTotal; // 1D array

        float * meanPitch;
        char * categories;

    } pitch2category_obj;

    pitch2category_obj * pitch2category_construct_zero(const unsigned int nSeps, const unsigned int tauMin, 
    const unsigned int tauMax, const float varTauMax, const float varTauMin, const float activityVarMin, 
    const float activityVarMax,const float harmonicMin, const float harmonicMax, const unsigned int minSamples, 
    const unsigned int maxSamples, const float activityMin, const float acorrMin, const unsigned int classificationPeriod);

    void pitch2category_destroy(pitch2category_obj * obj);

    void pitch2category_process(pitch2category_obj * obj, const pitches_obj * pitches, const tracks_obj * tracks, categories_obj * categories, const unsigned int iSep);

#endif
