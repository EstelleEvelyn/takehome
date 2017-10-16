#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include "loungeTest.h"

#define OPEN 0
#define MATH 1
#define CS 2
#define DEAN 3

// thread methods are each type of individual
void* mathProf(void* t);
void* csProf(void* t);
void* dean(void* t);

// helper functions
void delay(int);
void shuffle(int*, int);

// SLG testing vars
pthread_mutex_t sherriLock;
int waiting[4];
int inlounge[4];
const char* names[] = {"open", "math prof", "cs prof", "dean"};

int main(){
  // seed the random number generator with the current time
  srand(time(NULL));

  // initialize synch constructs (in solution init), counts, and sign (starts at 0 for open)
  init();
  for (int i=0; i<4; i++) {
    waiting[i] = 0;
    inlounge[i] = 0;
  }
  pthread_mutex_init(&sherriLock, NULL);
  sign = 0;

  // set # of each type of thread
  int maths = 6;
  int css = 7;
  int deans = 4;
  int tot = maths+css+deans;


  // create shuffled list of 1's, 2's, & 3's
  int order[tot];
  int i;
  for (i=0; i<maths; i++) {
    order[i] = 1;
  }
  for (; i<maths+css; i++) {
    order[i] = 2;
  }
  for (; i<tot; i++) {
    order[i] = 3;
  }
  shuffle(order, tot);

  // now create threads in order indicated by shuffled order array
  pthread_t profs[tot];
  for (i=0; i<tot; i++) {
    if (order[i]==1) pthread_create(&profs[i], NULL, mathProf, NULL);
    else if (order[i]==2) pthread_create(&profs[i], NULL, csProf, NULL);
    else if (order[i]==3) pthread_create(&profs[i], NULL, dean, NULL);
    else printf("something went horribly wrong!!!\n");
    // create threads in batches, so that test case where all current
    // math & cs profs enter and exit lounge, then some more show up
    if (i%5==4) sleep(2);
  }

  // join all threads before letting main exit
  for (i=0; i<tot; i++) {
    pthread_join(profs[i], NULL);
  }

  return 0;
}

void atDoor(int prof) {
  waiting[prof]++;
  printf("%s at door, %d %ss waiting, sign=%s\n", names[prof], waiting[prof], names[prof], names[sign]);
  fflush(stdout);
}

void entered(int prof) {
  waiting[prof]--;
  inlounge[prof]++;
  printf("%s enters lounge, %d %ss in lounge, sign=%s\n", names[prof], inlounge[prof], names[prof], names[sign]);
  fflush(stdout);
  if (sign != prof) printf("*** INVALID SIGN, should be %s, instead is %s ***\n", names[prof], names[sign]);
  if (inlounge[prof%3+1] > 0) printf("*** INVALID OCCUPANTS, %ss and %ss in room ***\n", names[prof],names[prof%3+1]);
  if (inlounge[(prof+1)%3+1] > 0) printf("*** INVALID OCCUPANTS, %ss and %ss in room ***\n", names[prof], names[(prof+1)%3+1]);
  fflush(stdout);
}

void left(int prof) {
  inlounge[prof]--;
  printf("%s left, %d %ss still in lounge, sign=%s\n", names[prof], inlounge[prof], names[prof], names[sign]);
  fflush(stdout);
  if (inlounge[prof] > 0 && sign != prof) printf("*** INVALID SIGN, should be %s, instead is %s ***\n", names[prof], names[sign]);
  fflush(stdout);
}

void* dean(void* t){
  // arrive
  pthread_mutex_lock(&sherriLock);
  atDoor(DEAN);
  pthread_mutex_unlock(&sherriLock);

  deanArrive();

  pthread_mutex_lock(&sherriLock);
  entered(DEAN);
  pthread_mutex_unlock(&sherriLock);

  // should not get here until this prof is in lounge!
  // hang out for at least some time (delay arg between 4 & 8)
  delay(rand()%5+4);

  // leave
  pthread_mutex_lock(&sherriLock);
  deanLeave();
  left(DEAN);
  pthread_mutex_unlock(&sherriLock);

  return 0;
}

// cs prof - simply arrives, delays in lounge, leaves
void* csProf(void* t){
  // arrive
  pthread_mutex_lock(&sherriLock);
  atDoor(CS);
  pthread_mutex_unlock(&sherriLock);

  csProfArrive();

  pthread_mutex_lock(&sherriLock);
  entered(CS);
  pthread_mutex_unlock(&sherriLock);

  // should not get here until this prof is in lounge!
  // hang out for at least some time (delay arg between 4 & 8)
  delay(rand()%5+4);

  // leave
  pthread_mutex_lock(&sherriLock);
  csProfLeave();
  left(CS);
  pthread_mutex_unlock(&sherriLock);

  return 0;
}

void* mathProf(void* t){
  // arrive
  pthread_mutex_lock(&sherriLock);
  atDoor(MATH);
  pthread_mutex_unlock(&sherriLock);

  mathProfArrive();

  pthread_mutex_lock(&sherriLock);
  entered(MATH);
  pthread_mutex_unlock(&sherriLock);

  // should not get here until this prof is in lounge!
  // hang out for at least some time (delay arg between 4 & 8)
  delay(rand()%5+4);

  // leave
  pthread_mutex_lock(&sherriLock);
  mathProfLeave();
  left(MATH);
  pthread_mutex_unlock(&sherriLock);

  return 0;
}

// function to randomize list of integers
void shuffle(int* intArray, int arrayLen) {
  int i=0;
  for (i=0; i<arrayLen; i++) {
    int r = rand()%arrayLen;
    int temp = intArray[i];
    intArray[i] = intArray[r];
    intArray[r] = temp;
  }
}

/*
 * NOP function to simply use up CPU time
 * arg limit is number of times to run each loop, so runs limit^2 total loops
 */
void delay( int limit )
{
  int j, k;

  for( j=1; j < limit*1000; j++ )
    {
      for( k=1; k < limit*1000; k++ )
        {
	  int x = j*k/(k+j);
	  int y = x/j + k*x - (j+5)/k + (x*j*k);
        }
    }
}
