#ifndef __ODAS_SIGNAL_PITCH
#define __ODAS_SIGNAL_PITCH

   /**
    * \file     pitch.h
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

    typedef struct pitches_obj {

        unsigned int nSignals;
        unsigned int * array;
        float * harmonicAcorr;

    } pitches_obj;

    pitches_obj * pitches_construct_zero(const unsigned int nSignals);

    void pitches_destroy(pitches_obj * obj);

    void pitches_copy(pitches_obj * dest, const pitches_obj * src);

    void pitches_zero(pitches_obj * obj);

    void pitches_printf(const pitches_obj * obj);

#endif