
OBJS = scheduler.o list.o procces.o FIFO.o STF.o STCF.o RR.o MLFQ.o
CFLAGS= -g -Wall

all: MA-scheduler

MA-scheduler: $(OBJS)
	gcc -o MA-scheduler $(OBJS)

scheduler.o: scheduler.c schedulers.h procces.h list.h
	gcc $(CFLAGS) -c scheduler.c

list.o: list.c list.h
	gcc $(CFLAGS) -c list.c

procces.o: procces.c procces.h list.h
	gcc $(CFLAGS) -c procces.c

FIFO.o: FIFO.c schedulers.h procces.h list.h
	gcc $(CFLAGS) -c FIFO.c

STF.o: STF.c schedulers.h procces.h list.h
	gcc $(CFLAGS) -c STF.c

STCF.o: STCF.c schedulers.h procces.h list.h
	gcc $(CFLAGS) -c STCF.c

RR.o: RR.c schedulers.h procces.h list.h
	gcc $(CFLAGS) -c RR.c

MLFQ.o: MLFQ.c schedulers.h procces.h list.h
	gcc $(CFLAGS) -c MLFQ.c

cleanO:
	rm -f $(OBJS)

cleanAll:
	rm -f MA-scheduler $(OBJS)




