#include "schedulers.h"
#include <time.h>

#define QUANTUM 10
#define QUEUES 5
#define TIMETORESET 210

void MLFQ(list* procc_arrival, int quantum, int queues, int ttor, int* allot, int verbose)
{
    int total_procc = procc_arrival->size;
    if(total_procc == 0) return;
    if(queues==1)
    {
        RR(procc_arrival,quantum, verbose);
        return;
    }
    if(quantum == -1) quantum = QUANTUM;
    if(queues == -1) 
    {
        queues = QUEUES;
        allot = (int*)(malloc(queues*sizeof(int)));
        allot[0] = 2*quantum, allot[1] = 1*quantum, allot[2] = 3*quantum;
        allot[3] = 2*quantum, allot[4] = 5*quantum;
    }
    if(ttor == -1) ttor = TIMETORESET; 
    
    int last_time_ttor=-1, count_reset = 0;
    
    long long time_act = __TOPTIME;

    double ttTotal = 0, rtTotal = 0, ttProm, rtProm;
    int ttMax = -1, rtMax = -1, ttParcial, rtParcial;
     
    procces* pr = (procces*)(pop_front(procc_arrival)->value);
    pr->last_queue=0;
    pr->time_queue=0;
    list* p_queues[queues];
    for (size_t i = 0; i < queues; i++)
    {
        p_queues[i] = init_list(pr);
        if(i != 0) pop_front(p_queues[i]);
    }
    list* procc_blocked = init_list(pr);
    pop_front(procc_blocked);
    int total_current =1;
    while (total_current)
    {
        while (procc_arrival->size>0 && time_act >=__TOPTIME)
        {
            procces* p = ((procces*)(pop_front(procc_arrival)->value));
            p->last_queue=0;
            p->time_queue=0;
            push_back(p_queues[0],p);      
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
                    if (p->io_operations!=NULL&&(p->io_operations->size>0)&&last_time_ttor>=((operationIO*)(p->io_operations->head->value))->begin)
                    {
                        push_back(p_queues[0],p);
                    }
                    else
                    {
                        push_back(p_queues[p->last_queue], p);  
                    }
                }  
            }
            else
            {
                min_time_to_end_next_io = MIN(min_time_to_end_next_io, io_temp->duration - time_passed);
            }  
        }   
        int first_queue =-1;
        for ( int j=0; j< queues; j++)
        {
            if (p_queues[j]->size!=0)
            {
                first_queue=j;
                break;
            }
        }
        if (first_queue != -1)
        {
            srand(time(NULL));
            int index = rand() % p_queues[first_queue]->size;
            node* cur_node = getAt(p_queues[first_queue],index);
            procces* cur_procc = ((procces*)(cur_node->value));
            if(first_queue == 0 && cur_procc->last_queue!=first_queue)
            {
                cur_procc->last_queue = first_queue;
                cur_procc->time_queue = 0;
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
            //proceso que se esta analizando en el momento si existe
            if(next_io != NULL && next_io->begin < (cur_procc->time_executed + quantum))
            {
                int time_to_next_io = (next_io->begin) - (cur_procc->time_executed);
                cur_procc->time_executed += time_to_next_io;
                cur_procc->procces_time -= time_to_next_io;
                time_act += time_to_next_io;
                cur_procc->time_queue+= time_to_next_io;
                if (first_queue<queues-1 && cur_procc->time_queue >= allot[first_queue])
                {
                    cur_procc->time_queue=0;
                    cur_procc->last_queue=first_queue+1;
                }
                next_io->begin = time_act;   // le pone el timepo en que comenzo a ejecutarse la io
                push_back(procc_blocked, cur_procc);
                removeNode(p_queues[first_queue], cur_node);
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
                    removeNode(p_queues[first_queue], cur_node);
                    if(verbose != -1)
                    {
                        printf("Finish procces, time: %lld\n", time_act);
                        print_procces(cur_procc);
                    }
                }
                else
                {
                    cur_procc->time_queue+=time_execution;
                    cur_procc->time_executed += time_execution;
                    if (first_queue <queues -1 && cur_procc->time_queue>=allot[first_queue])
                    {
                        cur_procc->time_queue = 0;
                        cur_procc->last_queue = first_queue+1;
                        removeNode(p_queues[first_queue],cur_node);
                        push_back(p_queues[first_queue+1],cur_procc);
                    }     
                }                
            }      
        }
        else
        {
            int time_for_next_arrival = (procc_arrival->size==0)? __INT32_MAX__:__TOPTIME - time_act;
            time_act += MIN(time_for_next_arrival, min_time_to_end_next_io);
        }  
        if(count_reset<time_act/ttor)
        {
            int p_i = 0, p_j = 1;
            int fict = p_queues[0]->size==0?1:0;
            if(fict) push_back(p_queues[0], init_procces(2, 3, -1));
            while (p_j < queues)
            {
                if(p_queues[p_j]->size > 0)
                {
                    node* last = p_queues[p_i]->tail;
                    node* first = p_queues[p_j]->head;
                    last->next = first;
                    first->previous = last;
                    p_queues[p_j]=init_list(init_procces(2, 3, -1));
                    pop_front(p_queues[p_j]);
                    p_i = p_j;
                }
                p_j++;
            }
            if(fict) pop_front(p_queues[0]);
            count_reset = time_act/ttor;
        }
        total_current = procc_arrival->size;
        for (int i = 0; i < queues; i++)
        {
            total_current+=(p_queues[i])->size;
        }
        total_current+=procc_blocked->size;
    }
    ttProm = ttTotal/total_procc;
    rtProm = rtTotal/total_procc;
    __answer(ttProm, ttMax, rtProm, rtMax)   
}