
#include "meeting.h"

MEETING::MEETING() {}

void MEETING::initialize()
{
    int i;
    // initialise meeting array
    for (i = 0; i < MAXPART; i++)
    {
        participant[i].angle = 0;
        participant[i].is_talking = 0;
        participant[i].silent_time = 0;
        participant[i].total_talk_time = 0;
        participant[i].num_turns = 0;
        participant[i].frequency = 0.0;
    }

    for (i = 0; i < 360; i++)
    {
        participant_at_angle[i] = 0;
    }

    total_silence = 0;
    total_meeting_time = 0;
    num_participants = 0;
    last_talker = 0;
    num_talking = 0;
}

void MEETING::process_latest_data(AUDIO odas_obj)
{

    int target_angle, iChannel, iAngle;
    static int prospective_source[4] = {0, 0, 0, 0}; // not pretty but will work for now - assumes NUMCHANNELS <= 4

    num_talking = 0;
    total_meeting_time++;

    for (iChannel = 0; iChannel < NUMCHANNELS; iChannel++)
    {
        //  dont use energy to check if track is active otherwise you miss the ending of the speech and
        //  participant talking is never set to false
        if (odas_obj.x_array[iChannel] != 0.0 && odas_obj.y_array[iChannel] != 0.0)
        {
            total_silence = 0; // consider moving this
            target_angle = 180 - (atan2(odas_obj.x_array[iChannel], odas_obj.y_array[iChannel]) * 57.3);

            // participant_at_angle holds a int for every angle position.  Once an angle is set to true a person is registered there
            // so if tracked source is picked up we check to see if it is coming from a known participant
            // if it is not yet known then we also check that we havent reached max particpants before trying to add a new one

            //max_num_participants -1 so that we dont go out of bounds - means 0 is never used so will need to optimise

            if (participant_at_angle[target_angle] == 0x00 && num_participants < (MAXPART - 1))
            {

                if (++prospective_source[iChannel] > MINTALKTIME) // once they have talked for X secs we are more certain they are a member
                {

                    num_participants++;
                    ++num_talking; // another person is talking in this session
                    participant_at_angle[target_angle] = num_participants;
                    participant[num_participants].angle = target_angle;

                    participant[num_participants].frequency = odas_obj.freq_array[iChannel];
                    // write a buffer around them

                    for (iAngle = 1; iAngle < ANGLESPREAD; iAngle++)
                    {
                        if (target_angle + iAngle < 360)
                        {
                            // could check if already set here - but for now will just overwrite
                            // 360 is for going round the clock face

                            participant_at_angle[target_angle + iAngle] = num_participants;
                        }
                        else
                        {
                            participant_at_angle[iAngle - 1] = num_participants;
                        }
                        if (target_angle - iAngle >= 0)
                        {
                            // could check if already set here - but for now will just overwrite
                            // 360 is for going round the clock face
                            participant_at_angle[target_angle - iAngle] = num_participants;
                        }
                        else
                        {
                            participant_at_angle[361 - iAngle] = num_participants;
                        }
                    }

                    participant[num_participants].is_talking = iChannel;
                }
            }
            else // its an existing talker we're hearing
            {
                // could put logic in here to count turns
                participant[participant_at_angle[target_angle]].is_talking = 1;
                participant[participant_at_angle[target_angle]].total_talk_time++;
                ++num_talking; // another person is talking in this session

            }
        }
        else
        {
            prospective_source[iChannel] = 0;
            total_silence++;
        }
    }
}