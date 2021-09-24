#ifndef meetpie_h
#define meetpie_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <math.h>
#include <time.h>
#include "/odas/include/odas.h" 

#define INPORT 9000
#define MAXLINE 1024
#define MAXPART 8
#define MAXSILENCE 500
#define NUMCHANNELS 3
#define ANGLESPREAD 10
#define MINTURNSILENCE 30
#define MINENERGY 0.2
#define MINTALKTIME 10
