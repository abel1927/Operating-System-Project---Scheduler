# Operating-System-Project---Scheduler

Program in C language in which 5 different scheduling algorithms are implemented: FIFO, STF, STCF, RR, MLFQ 

##

Course 

 - Operating System, second semester of second year(2020), Computer Sciences.

##

Team

- Marcos Pérez Labrada (@MarcosPerezLabrada)
- Abel Molina Sánchez (@abel1927)

##
Project specifications:

- parameter format:

    -f file -alg "ALGORITHM" -q 10 -queque 3 -timetoreset 200 -allotatment 20 30 10 -v
    
  (optional parameter -v to give more information about the execution.)


- dependencies: (they are in the Makefile)

    process.c ---> list.c
    
    (FIFO, STF, STCF, RR, MLFQ).c ---> procces.c
    
    scheduler.c ----> (idem).c
    
    ex: gcc list.c procces.c FIFO.c STF.c STCF.c RR.c MLFQ.c scheduler.c -o EXE
