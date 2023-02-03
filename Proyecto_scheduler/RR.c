#include "schedulers.h"
#include <time.h>

#define QUANTUM 10
  

void RR(list* procc_arrival, int quantum, int verbose)
{
    int total_procc = procc_arrival->size;
    if(total_procc == 0) return;
    if(quantum == -1) quantum = QUANTUM;

    long long time_act = __TOPTIME;
    double ttTotal = 0, rtTotal = 0, ttProm, rtProm;
    int ttMax = -1, rtMax = -1, ttParcial, rtParcial;

    procces* pr = (procces*)pop_front(procc_arrival)->value;

    list* procc_exec = init_list(pr);

    list* procc_blocked = init_list(pr);
    pop_front(procc_blocked);
    
    while (procc_arrival->size > 0 || procc_exec->size > 0 || procc_blocked->size > 0)
    {// while --> ejecucion
        while (procc_arrival->size > 0 && __TOPTIME <= time_act)
        {
            procces* p = (procces*)(pop_front(procc_arrival)->value);
            push_back(procc_exec, p);
        }
        int min_time_to_end_next_io = __INT32_MAX__;
        for (node* cur = procc_blocked->head; cur !=NULL; cur = cur->next)
        {
            procces* p = (procces*)(cur->value);
            operationIO* io_temp = (operationIO*)(p->io_operations->head->value);
            int time_passed = time_act - io_temp->begin;
            if(time_passed >= io_temp->duration)  // la operacion io termino.
            {
                p->time_executed += io_temp->duration;
                p->procces_time -= io_temp->duration;
                pop_front(p->io_operations);   //le quito la operacion io
                removeNode(procc_blocked, cur);   //lo saco de la lista de bloqueados
                if(p->procces_time == 0)  //si termino el proceso
                {               
                    int time_end = time_act - (time_passed - io_temp->duration); //timepo de fin
                    ttParcial = time_end - (p->time_arrival);
                    ttMax = MAX(ttMax, ttParcial);
                    ttTotal += ttParcial;
                    if(verbose != -1)
                    {
                        printf("Finish procces, time: %lld\n", time_act);
                        print_procces(p);
                    }
                }
                else
                {
                    push_back(procc_exec, p);  
                }  
            }
            else
            {
                min_time_to_end_next_io = MIN(min_time_to_end_next_io, io_temp->duration - time_passed);
            }  
        }    
        if(procc_exec->size > 0)
        {// if--> procc en ejecucion
            int firstLap = 1, secondLap = 0;
            srand(time(NULL));
            int index = rand() % procc_exec->size;
            int i = index;
            node* cur_node = getAt(procc_exec, i);
            while ((firstLap) || (secondLap && i++ < index))
            {// while --> ronda
                procces* cur_procc = ((procces*)(cur_node->value)); 
                while (procc_arrival->size > 0 && __TOPTIME <= time_act)
                {
                    procces* p = (procces*)(pop_front(procc_arrival)->value);
                    push_back(procc_exec, p);
                }
                for (node* cur = procc_blocked->head; cur !=NULL; cur = cur->next)
                {
                    procces* p = (procces*)(cur->value);
                    operationIO* io_temp = (operationIO*)(p->io_operations->head->value);
                    int time_passed = time_act - io_temp->begin;
                    if(time_passed >= io_temp->duration)  // la operacion io termino.
                    {
                        p->time_executed += io_temp->duration;
                        p->procces_time -= io_temp->duration;
                        pop_front(p->io_operations);   //le quito la operacion io
                        removeNode(procc_blocked, cur);   //lo saco de la lista de bloqueados
                        if(p->procces_time == 0)  //si termino el proceso
                        {
                            int time_end = time_act - (time_passed - io_temp->duration); //timepo de fin
                            ttParcial = time_end - (p->time_arrival);
                            ttMax = MAX(ttMax, ttParcial);
                            ttTotal += ttParcial;
                            if(verbose != -1)
                            {
                                printf("Finish procces, time: %lld\n", time_act);
                                print_procces(p);
                            }
                        }
                        else
                        {
                            push_back(procc_exec, p);  
                        }  
                    }      
                }
                if(cur_procc->time_executed == 0)
                {   
                    rtParcial = time_act - cur_procc->time_arrival;
                    rtMax = MAX(rtMax, rtParcial);
                    rtTotal += rtParcial;
                }
                operationIO* next_io;
                next_io = cur_procc->io_operations!=NULL&&(cur_procc->io_operations->size>0)?
                (operationIO*)cur_procc->io_operations->head->value:NULL;
                //esta linea coje el pprimer proceso de lectura del 
                //proceso que se esta analizando en el momento, si existe
                if(next_io != NULL && next_io->begin < (cur_procc->time_executed + quantum))
                {
                    int time_to_next_io = (next_io->begin) - (cur_procc->time_executed);
                    cur_procc->time_executed += time_to_next_io;
                    cur_procc->procces_time -= time_to_next_io;
                    time_act += time_to_next_io;
                    next_io->begin = time_act;   // le pone el timepo en que comenzo a ejecutarse la io
                    push_back(procc_blocked, cur_procc);
                    removeNode(procc_exec, cur_node);
                }
                else
                {
                    int time_execution = MIN(quantum, cur_procc->procces_time);
                    time_act += time_execution;
                    cur_procc->procces_time -= time_execution;
                    if(cur_procc->procces_time == 0)
                    {
                        ttParcial = time_act - (cur_procc->time_arrival);
                        ttMax = MAX(ttMax, ttParcial);
                        ttTotal += ttParcial;
                        removeNode(procc_exec, cur_node);
                        if(verbose != -1)
                        {
                            printf("Finish procces, time: %lld\n", time_act);
                            print_procces(cur_procc);
                        }
                    }
                    else
                    {
                        cur_procc->time_executed += time_execution;
                    }                
                }
                if(cur_node->next == NULL && firstLap)
                { 
                    firstLap = 0;
                    cur_node = procc_exec->head;
                    secondLap = 1;
                    i = 0;
                }
                else
                {
                    cur_node = cur_node->next;
                }              
            }// end while --> ronda
        }///endif ---> procc ejecc
        else
        {
            int time_for_next_arrival = (procc_arrival->size==0)? __INT32_MAX__:__TOPTIME - time_act;
            time_act += MIN(time_for_next_arrival, min_time_to_end_next_io);
        }         
    }// end while ---> ejecucion

    ttProm = ttTotal/total_procc;
    rtProm = rtTotal/total_procc;
    __answer(ttProm, ttMax, rtProm, rtMax)
}