
/**
 * \file     acorr2pitch.c
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

#include <system/acorr2pitch.h>

acorr2pitch_obj *acorr2pitch_construct_zero(const unsigned int nSignals, const unsigned int halfFrameSize, const unsigned int winSize)
{

    acorr2pitch_obj *obj;

    obj = (acorr2pitch_obj *)malloc(sizeof(acorr2pitch_obj));

    obj->nSignals = nSignals;
    obj->halfFrameSize = halfFrameSize;
    obj->winSize = winSize;

    return obj;
}

void acorr2pitch_destroy(acorr2pitch_obj *obj)
{

    free((void *)obj);
}

void acorr2pitch_process(acorr2pitch_obj *obj, const acorrs_obj *acorrs, pitches_obj *pitches)
{

    unsigned int iSignal;

    signed int iBin;
    signed int iWin;
    signed int iSample;

    float peakValue;
    float nextValue;

    float maxValue;
    unsigned int maxIndex;
    char isPeak;
    char peakFound;

    unsigned int peakPrev;
    unsigned int peakNow;
    unsigned int peakNext;
    float RPrev;
    float RNow;
    float RNext;

    maxValue = -INFINITY;
    peakFound = 0x00;

    for (iBin = 0; iBin < obj->halfFrameSize; iBin++)
    {
        printf("%1.3f, ", acorrs->array[0][iBin]*100);
    }
    printf("\n");

    pitches->array[0] = 0.0f;

}