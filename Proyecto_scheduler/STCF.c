#include "schedulers.h"

#define QUANTUM 10

void STCF(list* procc_arrival, int quantum, int verbose)
{
    if(quantum == -1) quantum = QUANTUM;
    int total_procc = procc_arrival->size;

    if(total_procc == 0) return;
    double ttTotal = 0, rtTotal = 0, ttProm, rtProm;
    int ttMax = -1, rtMax = -1, rtParcial, ttParcial;
    long long time = __TOPTIME;

    procces* pr = (procces*)(pop_front(procc_arrival)->value);
    list* procc_exec = init_list(pr);
    list* procc_blocked = init_list(pr);
    pop_front(procc_blocked);
    while (procc_arrival->size > 0 || procc_exec->size > 0 || procc_blocked->size > 0)
    {// while --> ejecucion
        while (procc_arrival->size > 0 && __TOPTIME <= time)
        {
            procces* p = (procces*)((pop_front(procc_arrival)->value));
            sorted_insert(procc_exec, p);
        }       
        int min_time_to_end_next_io = __INT32_MAX__;
        for (node* cur = procc_blocked->head; cur !=NULL; cur = cur->next)
        {
            procces* p = (procces*)(cur->value);
            operationIO* io_temp = (operationIO*)(p->io_operations->head->value);
            int time_passed = time - io_temp->begin;
            if(time_passed >= io_temp->duration)  // la operacion io termino.
            {
                p->time_executed += io_temp->duration;
                p->procces_time -= io_temp->duration;
                pop_front(p->io_operations);   //le quito la operacion io
                removeNode(procc_blocked, cur);   //lo saco de la lista de bloqueados
                if(p->procces_time == 0)  //si termino el proceso
                {
                    int time_end = time - (time_passed - io_temp->duration); //timepo de fin
                    ttParcial = time_end - (p->time_arrival);
                    ttMax = MAX(ttMax, ttParcial);
                    ttTotal += ttParcial;
                    if(verbose != -1)
                    {
                        printf("Finish procces, time: %lld\n", time);
                        print_procces(p);
                    }
                }
                else
                {
                    sorted_insert(procc_exec, p);    
                }  
            }
            else
            {
                min_time_to_end_next_io = MIN(min_time_to_end_next_io, io_temp->duration - time_passed);
            }        
        }
        if(procc_exec->size > 0)
        {// if ---> procc ejec
            procces* p = (procces*)(procc_exec->head->value);
            if(p->time_executed == 0)
            {
                rtParcial = time - p->time_arrival;
                rtMax = MAX(rtMax, rtParcial);
                rtTotal += rtParcial;
            }
            operationIO* next_io;
            next_io = (p->io_operations!=NULL&&p->io_operations->size>0)?(operationIO*)p->io_operations->head->value:NULL;
            if(next_io != NULL && next_io->begin < (p->time_executed + quantum))
            {  
                int time_to_next_io = next_io->begin - p->time_executed;
                p->time_executed += time_to_next_io;
                p->procces_time -= time_to_next_io;
                time += time_to_next_io;
                next_io->begin = time;   // le pone el timepo en que comenzo a ejecutarse la io
                push_back(procc_blocked, p);
                pop_front(procc_exec);     
            }
            else
            {
                int time_execution = MIN(quantum, p->procces_time);
                time += time_execution;
                p->time_executed += time_execution;
                p->procces_time -= time_execution;
                if(p->procces_time == 0)
                {
                    ttParcial = time - (p->time_arrival);
                    ttMax = MAX(ttMax, ttParcial);
                    ttTotal += ttParcial;
                    pop_front(procc_exec);
                    if(verbose != -1)
                    {
                        printf("Finish procces, time: %lld\n", time);
                        print_procces(p);
                    }
                }
            }             
        }// endif --> procc ejec
        else
        {
            int time_for_next_arrival = (procc_arrival->size==0)? __INT32_MAX__:__TOPTIME - time;
            time += MIN(time_for_next_arrival, min_time_to_end_next_io);
        } 
    }// end while --> ejecucion
    ttProm = ttTotal/total_procc;
    rtProm = rtTotal/total_procc;
    __answer(ttProm, ttMax, rtProm, rtMax)
}