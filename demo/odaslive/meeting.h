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
#define MAXSILENCE 500
#define NUMCHANNELS 3
#define ANGLESPREAD 10
#define MINTURNSILENCE 30
#define MINENERGY 0.2
#define MINTALKTIME 10

extern char debug_mode;

class MEETING
{
public:
MEETING ();
~MEETING ();
void Initialize();
void ProcessLatestData(AUDIO);


struct participant_data{
    int angle;
    int is_talking;
    int silent_time;
    int total_talk_time;
    int num_turns;
    float frequency;
 };

    int participant_at_angle[360];
    int num_participants;
    int total_silence;
    int total_meeting_time;
    int last_talker;
    int num_talking;
    struct participant_data participant[MAXPART];

};

#endif 
