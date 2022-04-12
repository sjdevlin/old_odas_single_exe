/**
 * \file     pitch2category.c
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

#include <system/pitch2category.h>

pitch2category_obj *pitch2category_construct_zero(const unsigned int nSeps, const unsigned int tauMin, const unsigned int tauMax, const float tauVarMin, const float tauVarMax, const float activityVarMin, const float activityVarMax, const float harmonicMin, const float harmonicMax, const unsigned int samplesMin, const unsigned int samplesMax, const float activityMin, const float acorrMin, const unsigned int classificationPeriod)
{

    pitch2category_obj *obj;

    unsigned int iSep;

    obj = (pitch2category_obj *)malloc(sizeof(pitch2category_obj));

    obj->nSeps = nSeps;

    obj->tauMin = tauMin;
    obj->tauMax = tauMax;

    obj->tauVarMin = tauVarMin;
    obj->tauVarMax = tauVarMax;
    obj->activityVarMin = activityVarMin;
    obj->activityVarMax = activityVarMax;
    obj->harmonicMin = harmonicMin;
    obj->harmonicMax = harmonicMax;
    obj->samplesMin = samplesMin;
    obj->samplesMax = samplesMax;
    obj->activityMin = activityMin;
    obj->acorrMin = acorrMin;

    // sd changed to improve classification
    obj->classificationPeriod = classificationPeriod;

    obj->categories = (char *)malloc(sizeof(char) * nSeps);
    memset(obj->categories, 0x00, sizeof(char) * nSeps);

    // sd changed to improve classification
    obj->processingTime = (unsigned int *)malloc(sizeof(unsigned int) * nSeps);
    memset(obj->processingTime, 0x00, sizeof(unsigned int) * nSeps);

    obj->numPitchValues = (unsigned int *)malloc(sizeof(unsigned int) * nSeps);
    memset(obj->numPitchValues, 0x00, sizeof(unsigned int) * nSeps);

    obj->pitchArray = (unsigned int **)malloc(sizeof(unsigned int *) * nSeps);
    obj->activityArray = (unsigned int **)malloc(sizeof(unsigned int *) * nSeps);

    for (iSep = 0; iSep < nSeps; iSep++)
    {
        obj->pitchArray[iSep] = (unsigned int *)malloc(sizeof(unsigned int) * classificationPeriod);
        memset(obj->pitchArray[iSep], 0x00, sizeof(unsigned int) * classificationPeriod);
        obj->activityArray[iSep] = (unsigned int *)malloc(sizeof(unsigned int) * classificationPeriod);
        memset(obj->activityArray[iSep], 0x00, sizeof(unsigned int) * classificationPeriod);
    }

    obj->activityTotal = (unsigned int *)malloc(sizeof(unsigned int) * nSeps);
    memset(obj->activityTotal, 0x00, sizeof(unsigned int) * nSeps);
    obj->pitchTotal = (unsigned int *)malloc(sizeof(unsigned int) * nSeps);
    memset(obj->pitchTotal, 0x00, sizeof(unsigned int) * nSeps);
    obj->harmonicAcorrTotal = (float *)malloc(sizeof(float) * nSeps);
    memset(obj->harmonicAcorrTotal, 0x00, sizeof(float) * nSeps);

    obj->meanPitch = (float *)malloc(sizeof(unsigned int) * nSeps);
    memset(obj->meanPitch, 0x00, sizeof(float) * nSeps);


    return obj;
}

void pitch2category_destroy(pitch2category_obj *obj)
{
    unsigned int iSep;

    free((void *)obj->categories);

    for (iSep = 0; iSep < obj->nSeps; iSep++)
    {
        free((void *)obj->activityArray[iSep]);
        free((void *)obj->pitchArray[iSep]);
    }

    free((void *)obj->pitchArray);
    free((void *)obj->activityArray);

    free((void *)obj->harmonicAcorrTotal);

    free((void *)obj->pitchTotal);

    free((void *)obj->numPitchValues);
    free((void *)obj->activityTotal);
    free((void *)obj->processingTime);
    free((void *)obj->meanPitch);

    free((void *)obj);
}

void pitch2category_process(pitch2category_obj *obj, const pitches_obj *pitches, const tracks_obj *tracks, categories_obj *categories, const unsigned int iSep)
{
    int i;
    float deltaPitch;
    float rmsMean, activityMean, pitchMean, harmonicAcorrMean;
    float activityDiff, rmsDiff, pitchDiff;
    float totalRmsDiffSquared, totalActivityDiffSquared, totalPitchDiffSquared;
    float relActivityVariance, relPitchVariance;

    // add activity to array
    if (obj->processingTime[iSep] < obj->classificationPeriod)
    {
        obj->activityArray[iSep][obj->processingTime[iSep]] = (unsigned int)(tracks->activity[iSep] * 100.0);
        //    printf ("(A) %d,  %3.3f, %d \n", obj->processingTime[iSep], tracks->activity[iSep], obj->activityArray[iSep][obj->processingTime[iSep]]);
        // also calculate activity total for variance analysis
        obj->activityTotal[iSep] += obj->activityArray[iSep][obj->processingTime[iSep]]; // to avoid iterating a second time

        if (pitches->array[iSep] > 0) // skip analysis when no peak found
        {
            obj->pitchArray[iSep][obj->numPitchValues[iSep]] = pitches->array[iSep];
            obj->pitchTotal[iSep] += obj->pitchArray[iSep][obj->numPitchValues[iSep]];
            obj->harmonicAcorrTotal[iSep] += pitches->harmonicAcorr[iSep];
            ++obj->numPitchValues[iSep];
        }

        // when classification period is ended do all the processing
    }
    else if (obj->processingTime[iSep] == obj->classificationPeriod) // we have reached time to classify as speech
    {

        // calculate mean of activity
        activityMean = (float)obj->activityTotal[iSep] / (float)obj->classificationPeriod;
        // calculate mean of rms

        if (obj->numPitchValues[iSep] != 0)
        {
            pitchMean = (float)obj->pitchTotal[iSep] / (float)obj->numPitchValues[iSep]; // consider changing to int for spped
            harmonicAcorrMean = obj->harmonicAcorrTotal[iSep] / (float)obj->numPitchValues[iSep];
        }
        else
        {
            pitchMean = 0.0f;
            harmonicAcorrMean = 0.0f;
        }

        totalActivityDiffSquared = 0.0f;
        totalRmsDiffSquared = 0.0f;
        totalPitchDiffSquared = 0.0f;

        // relative variance of amplitide and E (for comparison)

        for (i = 0; i < obj->classificationPeriod; i++)
        {
            activityDiff = (float)obj->activityArray[iSep][i] - activityMean;
            totalActivityDiffSquared += (activityDiff * activityDiff); // consider changing to powf
                                                                       //            printf ("(B) %d , %d  \n", i, obj->activityArray[iSep][obj->processingTime[iSep]]);
        }

        // relative variance of pitch

        for (i = 0; i < obj->numPitchValues[iSep]; i++)
        {
            pitchDiff = (float)obj->pitchArray[iSep][i] - pitchMean; // consider changing to int
            totalPitchDiffSquared += (pitchDiff * pitchDiff);        // consider powf?
        }

        // rel variance is variance / mean ^2

        relActivityVariance = (totalActivityDiffSquared / (float)obj->classificationPeriod) / (activityMean * activityMean);

        if (obj->numPitchValues[iSep] != 0)
        {
            relPitchVariance = (totalPitchDiffSquared / (float)obj->numPitchValues[iSep]) / (pitchMean * pitchMean);
        }
        else
        {
            relPitchVariance = 0.0f;
        }

        // decide if it is speech or not !
        if (pitchMean > obj->tauMax || pitchMean < obj->tauMin) printf ("failed pitch\n");
        if (relPitchVariance  > obj->tauVarMax || relPitchVariance  < obj->tauVarMin) printf ("failed pitch var\n");
        if (relActivityVariance  > obj->activityVarMax || relActivityVariance  < obj->activityVarMin) printf ("failed act var \n");
        if (harmonicAcorrMean > obj->harmonicMax || harmonicAcorrMean < obj->harmonicMin ) printf ("failed harmonic\n");
        if (obj->numPitchValues[iSep] < obj->samplesMin || obj->numPitchValues[iSep] > obj->samplesMax) printf ("failed samples\n");


        if (pitchMean <= obj->tauMax && pitchMean >= obj->tauMin &&
            relPitchVariance <= obj->tauVarMax && relPitchVariance >= obj->tauVarMin &&
            relActivityVariance <= obj->activityVarMax && relActivityVariance >= obj->activityVarMin &&
            harmonicAcorrMean <= obj->harmonicMax && harmonicAcorrMean >= obj->harmonicMin &&
            obj->numPitchValues[iSep] <= obj->samplesMax && obj->numPitchValues[iSep] >= obj->samplesMin)
        {
            obj->categories[iSep] = 0x02; // speech
            obj->meanPitch[iSep] = pitchMean;
        }
        else
        {
            obj->categories[iSep] = 0x01;
            printf("Non Speech%llu, %3.2f, %2.2f, %2.2f, %2.2f, %d \n", tracks->ids[iSep], pitchMean, relPitchVariance, relActivityVariance, harmonicAcorrMean, obj->numPitchValues[iSep]);
        }

    }
}