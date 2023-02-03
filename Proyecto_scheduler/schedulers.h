//#include <stdio.h>
//#include <stdlib.h>
//#include "list.h"
#include "procces.h"

#define __answer(ttp,ttm,rtp,rtm) printf("Tt Prom:%.3f\nTt Max:%d\nRt Prom:%.3f\nRt Max:%d\n", ttp,ttm,rtp,rtm);
#define __TOPTIME ((procces*)(procc_arrival->head->value))->time_arrival

void FIFO(list*, int);
void STF(list*, int);
void STCF(list*, int, int);
void RR(list*, int, int);
void MLFQ(list*, int, int, int, int*, int);