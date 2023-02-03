#include "procces.h"

operationIO* init_operationIO(size_t t_begin, size_t t_duration)
{
    operationIO* op = (operationIO*)malloc(sizeof(operationIO));
    __SafeMemoryReserved(op)
    op->begin = t_begin;
    op->duration = t_duration;
    return op;
}
procces* init_procces(size_t t_arrival, size_t t_procces, int pid)
{
    procces* p = (procces*)malloc(sizeof(procces));
    __SafeMemoryReserved(p)
    p->time_arrival = t_arrival;
    p->procces_time = t_procces;
    p->time_executed = 0;
    p->pid = pid;
    return p;
}
void set_IO_operation(procces* p, int t_begin, int t_duration)
{
    operationIO* op = init_operationIO(t_begin, t_duration);
    if(p->io_operations == NULL)
    {
        p->io_operations = init_list(op);
    }
    else
    { 
        push_back(p->io_operations, op);
    }
}
void freeProcces(procces* p)
{
    __SafeMemoryFree(p->io_operations)
}
void sorted_insert(list* l, procces* p)
{
    if(l->size==0)
    {
        push_back(l, p);
        return;
    }   
    if(l->size==1)
    {
        node* cur = l->head;
        if(p->procces_time > ((procces*)cur->value)->procces_time)
        {
            push_back(l, p);
        }
        else
        {
            push_front(l,p);
        }
        return;
    }
    if(p->procces_time < ((procces*)l->head->value)->procces_time)
    {
        push_front(l,p);
        return;
    }
    if (p->procces_time > ((procces*)l->tail->value)->procces_time)
    {
        push_back(l, p);
        return;
    }        
    node* lizq = l->head;
    node* lder = l->head->next;
    while (lder!=NULL)
    {
        if(p->procces_time < ((procces*)lder->value)->procces_time)
        {
            node* insert = __ReservedMemoryforNode
            insert->value = p;
            insert->previous = lizq;
            lizq->next = insert;
            lder->previous = insert;
            insert->next = lder;
            l->size++;
            return;
        }
        else
        {
            lizq = lder;
            lder = lder->next;
        }
    }
}

void print_list_io(list* l)
{
    if(l == NULL) return;
    if(l->size==0) return;
    for (node* cur = l->head; cur!=NULL; cur=cur->next)
    {
        printf("    --io-op---, begin:%ld, duration:%ld\n", ((operationIO*)cur->value)->begin, ((operationIO*)cur->value)->duration);
    }
    printf("\n");
}

void print_procces(procces* p)
{
    printf("--poceso:%d---: time_arrival:%ld\n", p->pid, p->time_arrival);
    //print_list_io(p->io_operations);
}

void print_list_procces(list* l)
{
    if(l->size==0) return;
    for (node* cur = l->head; cur!=NULL; cur=cur->next)
    {
        print_procces((procces*)cur->value);
    }
    printf("\n");
}