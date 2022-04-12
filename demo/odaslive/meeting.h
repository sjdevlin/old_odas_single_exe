#ifndef meeting_h
#define meeting_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#include <math.h>
#include <time.h>
#include "audio.h"

#define MAXPART 8
#define MAXSILENCE 300 // temp small for debug
#define NUMCHANNELS 3
#define ANGLESPREAD 30
#define MINTURNSILENCE 30
#define MINENERGY 0.2
#define MINTALKTIME 5

extern char debug_mode;

class MEETING
{
public:
MEETING ();
void initialize();
void process_latest_data(AUDIO);


struct participant_data{
    int last_track_id;
    int angle;
    int is_talking;
    int silent_time;
    int total_talk_time;
    int num_turns;
    int share_of_voice;  // as inetger percentage
    float frequency;  // should this be int ?
 };

    int participant_at_angle[360];
    int num_participants;
    int total_silence;
    int total_meeting_time;
    int total_talk_time;
    int last_talker;
    int num_talking;
    time_t duration;
    int participant_clock_order[MAXPART];
    struct participant_data participant[MAXPART];

private:
    time_t start_time;
    int prospective_source[NUMCHANNELS];
};

#endif 
