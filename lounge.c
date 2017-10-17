/* Estelle Bayer
CS 332 Take Home Exam

A thread-based solution to making professors and deans share space.
*/

#include "loungeTest.h"

//to avoid typos and confusion
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
  //how many people are in the lounge
  occupancy = 0;
  //how many deans are at the door
  waitingDeans = 0;
  //what does the sign say
  sign = 0;
  return;
}

void mathProfArrive() {
  pthread_mutex_lock(&lock);
  //math prof must wait while there are CS profs or deans in the lounge,
  //or if there are deans waiting at the door with them
  while((sign != MATH && sign != EMPTY) || waitingDeans != 0) {
    pthread_cond_wait(&enterLounge, &lock);
  }
  //first prof in should change sign
  if (sign == EMPTY) {
    sign = MATH;
  }
  //update occupancy from inside the lounge
  occupancy++;
  //signal so other math profs can enter
  pthread_cond_broadcast(&enterLounge);
  pthread_mutex_unlock(&lock);
  return;
}

void csProfArrive() {
  pthread_mutex_lock(&lock);
  //math prof waits while math profs or deans occupy lounge
  //or if deans are visible waiting at door
  while((sign != CS && sign != EMPTY) || waitingDeans != 0) {
    pthread_cond_wait(&enterLounge, &lock);
  }
  //first prof in changes sign
  if (sign == EMPTY) {
    sign = CS;
  }
  //update occupancy in lounge
  occupancy++;
  //signal to let other CS profs enter
  pthread_cond_broadcast(&enterLounge);
  pthread_mutex_unlock(&lock);
  return;
}

void deanArrive() {
  pthread_mutex_lock(&lock);
  //there are deans milling about outside
  waitingDeans++;
  //wait to enter while there are profs in the room
  while(sign != DEAN && sign != EMPTY) {
    pthread_cond_wait(&enterLounge, &lock);
  }
  //first dean in changes sign
  if (sign == EMPTY) {
    sign = DEAN;
  }
  //there is one more person in the lounge and one fewer dean milling outside
  occupancy++;
  waitingDeans--;
  //let other deans enter
  pthread_cond_broadcast(&enterLounge);
  pthread_mutex_unlock(&lock);
  return;
}

void mathProfLeave() {
  pthread_mutex_lock(&lock);
  //there's one fewer person in the lounge
  occupancy--;
  //last one out indicates that lounge is empty
  if(occupancy == 0) {
    sign = EMPTY;
  }
  //let other people use the lounge
  pthread_cond_broadcast(&enterLounge);
  pthread_mutex_unlock(&lock);
  return;
}

void csProfLeave() {
  pthread_mutex_lock(&lock);
  //one fewer person in lounge
  occupancy--;
  //last out changes sign to empty
  if(occupancy == 0) {
    sign = EMPTY;
  }
  //let others use lounge
  pthread_cond_broadcast(&enterLounge);
  pthread_mutex_unlock(&lock);
  return;
}

void deanLeave() {
  pthread_mutex_lock(&lock);
  //one fewer dean
  occupancy--;
  //last out changes sign
  if(occupancy == 0) {
    sign = EMPTY;
  }
  //let others in
  pthread_cond_broadcast(&enterLounge);
  pthread_mutex_unlock(&lock);
  return;
}
