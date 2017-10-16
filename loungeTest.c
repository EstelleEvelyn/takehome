#include "loungeTest.h"

// thread methods are each type of prof
void* mathProf(void* t);
void* csProf(void* t);
void* dean(void* t);

// helper functions
void delay(int);
void shuffle(int*, int);

int main(){
  // seed the random number generator with the current time
  srand(time(NULL));

  // call init to set up synch tools, sign starts at 0 for open
  init();
  sign = 0;
  
  // set # of each type of student
  int maths = 6;
  int css = 7;
  int deans = 3;


  // create shuffled list of 1's, 2's, & 3's
  int order[maths + css + deans];
  int i;
  for (i=0; i<maths; i++) {
    order[i] = 1;
  }
  for (; i<maths+css; i++) {
    order[i] = 2;
  }
  for (; i<maths+css+deans; i++) {
    order[i] = 3;
  }
  shuffle(order, maths+css+deans);

  // now create threads in order indicated by shuffled order array
  pthread_t profs[maths+css+deans];
  for (i=0; i<maths+css+deans; i++) {
    if (order[i]==1) pthread_create(&profs[i], NULL, mathProf, NULL);
    else if (order[i]==2) pthread_create(&profs[i], NULL, csProf, NULL);
    else if (order[i]==3) pthread_create(&profs[i], NULL, dean, NULL);
    else printf("something went horribly wrong!!!\n");
  }

  // join all threads before letting main exit
  for (i=0; i<maths+css+deans; i++) {
    pthread_join(profs[i], NULL);
  }

  return 0;
}

// dean arrives, hangs out, leaves
void* dean(void* t){
  // arrive
  printf("dean at door, sign=%d\n", sign);
  deanArrive();
  printf("dean in lounge, sign=%d\n", sign);
	
  // hang out for at least some time (delay arg between 4 & 8)
  delay(rand()%5+4);

  // leave
  deanLeave();
  printf("dean left, sign=%d\n", sign);

  return 0;
}

// cs prof - simply arrives, delays in lounge, leaves
void* csProf(void* t){
  // arrive
  printf("cs prof at door, sign=%d\n", sign);
  csProfArrive();
  printf("cs prof in lounge, sign=%d\n", sign);
	
  // hang out for at least some time (delay arg between 4 & 8)
  delay(rand()%5+4);

  // leave
  csProfLeave();
  printf("cs prof left, sign=%d\n", sign);

  return 0;
}

// math prof - arrives, delays, leaves
void* mathProf(void* t){
  // arrive
  printf("math prof at door, sign=%d\n", sign);
  mathProfArrive();
  printf("math prof in lounge, sign=%d\n", sign);
	
  // hang out for at least some time (delay arg between 4 & 8)
  delay(rand()%5+4);

  // leave
  mathProfLeave();
  printf("math prof left, sign=%d\n", sign);

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
