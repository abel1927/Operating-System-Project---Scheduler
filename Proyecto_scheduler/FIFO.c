#include "schedulers.h"

void FIFO(list* procc_arrival, int verbose)
{
    int total_procc = procc_arrival->size;
    if(total_procc == 0) return;
    
    double ttTotal = 0, rtTotal = 0, ttProm, rtProm;
    int ttMax = -1, rtMax = -1, rtParcial, ttParcial;
    long long time = 0;

    while (procc_arrival->size > 0)
    {
        if(__TOPTIME >= time)
        {
            time += (__TOPTIME - time);
        }
        procces* cur = (procces*)(pop_front(procc_arrival)->value);
    
        rtParcial = time - cur->time_arrival;
        rtMax = MAX(rtMax, rtParcial);
        rtTotal += rtParcial;

        time += cur->procces_time;

        ttParcial = time - cur->time_arrival;
        ttMax = MAX(ttMax, ttParcial);
        ttTotal += ttParcial;
        if(verbose != -1)
        {
            printf("Finish procces, time: %lld\n", time);
            print_procces(cur);
        } 
    }
    ttProm = ttTotal/total_procc;
    rtProm = rtTotal/total_procc;
    __answer(ttProm, ttMax, rtProm, rtMax)
}