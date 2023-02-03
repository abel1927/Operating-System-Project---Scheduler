#include "list.h"

#define __SafeMemoryReserved(x) if(x == NULL) exit(1);
#define MIN(a,b) a > b? b : a;
#define MAX(a,b) a > b? a : b;

typedef struct  operationIO
{
    size_t begin;
    size_t duration;
} operationIO;


typedef struct  procces
{
    int pid;
    size_t time_arrival;
    size_t procces_time;
    size_t time_executed;
    size_t last_queue;
    size_t time_queue;
    list* io_operations;
} procces;

operationIO* init_operationIO(size_t, size_t);
procces* init_procces(size_t, size_t, int);
void set_IO_operation(procces*, int, int);
void freeProcces(procces*);

void sorted_insert(list*, procces*);

void print_list_io(list*);
void print_procces(procces*);
void print_list_procces(list*);