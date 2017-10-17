#include "loungeTest.h"

#define EMPTY 0
#define MATH 1
#define CS 2
#define DEAN 3

pthread_cond_t enterLounge;
pthread_mutex_t lock;
int occupancy;
int waitingDeans;

void init() {
  pthread_mutex_init(&lock, NULL);
  pthread_cond_init(&enterLounge, NULL);
  occupancy = 0;
  waitingDeans = 0;
  sign = 0;
  return;
}

void mathProfArrive() {
  pthread_mutex_lock(&lock);
  while((sign != MATH && sign != EMPTY) || waitingDeans != 0) {
    pthread_cond_wait(&enterLounge, &lock);
  }
  if (sign == EMPTY) {
    sign = MATH;
  }
  occupancy++;
  pthread_cond_signal(&enterLounge);
  pthread_mutex_unlock(&lock);
  return;
}

void csProfArrive() {
  pthread_mutex_lock(&lock);
  while((sign != CS && sign != EMPTY) || waitingDeans != 0) {
    printf("waitin for a cs");
    pthread_cond_wait(&enterLounge, &lock);
  }
  if (sign == EMPTY) {
    sign = CS;
  }
  occupancy++;
  pthread_cond_signal(&enterLounge);
  pthread_mutex_unlock(&lock);
  return;
}

void deanArrive() {
  pthread_mutex_lock(&lock);
  waitingDeans++;
  // pthread_mutex_unlock(&lock);
  while(sign != DEAN && sign != EMPTY) {
    printf("waiting on a dean");
    pthread_cond_wait(&enterLounge, &lock);
  }
  if (sign == EMPTY) {
    sign = DEAN;
  }
  occupancy++;
  waitingDeans--;
  pthread_cond_signal(&enterLounge);
  pthread_mutex_unlock(&lock);
  return;
}

void mathProfLeave() {
  pthread_mutex_lock(&lock);
  occupancy--;
  if(occupancy == 0) {
    sign = EMPTY;
  }
  pthread_cond_signal(&enterLounge);
  pthread_mutex_unlock(&lock);
  return;
}

void csProfLeave() {
  pthread_mutex_lock(&lock);
  occupancy--;
  if(occupancy == 0) {
    sign = EMPTY;
  }
  pthread_cond_signal(&enterLounge);
  pthread_mutex_unlock(&lock);
  return;
}

void deanLeave() {
  pthread_mutex_lock(&lock);
  occupancy--;
  if(occupancy == 0) {
    sign = EMPTY;
  }
  pthread_cond_signal(&enterLounge);
  pthread_mutex_unlock(&lock);
  return;
}
