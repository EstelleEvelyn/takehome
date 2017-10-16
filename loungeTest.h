#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>

// 0 open, 1 math, 2 CS, 3 dean
int sign;

void init();
void mathProfArrive();
void csProfArrive();
void mathProfLeave();
void csProfLeave();
void deanLeave();
void deanArrive();
