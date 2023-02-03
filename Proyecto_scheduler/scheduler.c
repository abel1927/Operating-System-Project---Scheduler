//#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "schedulers.h"

list* read_procces(char* path);

int main(int argc, char** argv)
{
	int params = argc-1;
	if(params<4)
	{
		printf("Cantidad de parametros incorrecta!\n");
		exit(1);
	}
	char  *alg, *file;
	int quantum = -1, ttor = -1, queues = -1, *allot, verbose = -1; 
	size_t i = 1;
	while (i <= params)
	{
		if(strcmp(argv[i], "-f") == 0)
		{
			file = argv[++i];
		}
		else if (strcmp(argv[i], "-alg") == 0)
		{
			alg = argv[++i];
		}
		else if (strcmp(argv[i], "-q") == 0)
		{
			quantum = atoi(argv[++i]);
		}
		else if(strcmp(argv[i], "-timetoreset") == 0)
		{
			ttor = atoi(argv[++i]);
		}
		else if (strcmp(argv[i], "-queue") == 0)
		{
			queues = atoi(argv[++i]);
		}
		else if (strcmp(argv[i], "-allotatment") == 0)
		{
			allot = (int*)malloc(queues*sizeof(int));
			for(size_t j = i + 1, k = 0; k < queues; k++,j++, i++)
			{
				allot[k] = atoi(argv[j]);
			}
		}
		else if (strcmp(argv[i], "-v") == 0)
		{
			verbose = 1;
		}
		else
		{
			printf("%s\n", argv[i]);
			printf("Error en los parametros de entrada!\n");
			exit(1);
		}
	    ++i;	
	}
	list* procc_arrival = read_procces(file);
	if(alg == NULL)
	{
		printf("No se selecciono un tipo de algoritmo!");
		exit(1);
	}
	if(strcmp(alg,"FIFO") == 0)
	{
        FIFO(procc_arrival, verbose);
	}
	else if (strcmp(alg,"STF") == 0)
	{
		STF(procc_arrival, verbose);
	}
	else if (strcmp(alg,"STCF") == 0)
	{
		STCF(procc_arrival, quantum, verbose);
	}
	else if (strcmp(alg,"RR") == 0)
	{
		RR(procc_arrival, quantum, verbose);
	}
	else if(strcmp(alg,"MLFQ") == 0)
	{
		MLFQ(procc_arrival, quantum, queues, ttor, allot, verbose);
	}
	else
	{
		printf("Error, no se reconoce el algoritmo seleccionado!\n");
		exit(1);
	}
	return 0;
}

list* read_procces(char* path)
{
	FILE* file = fopen(path, "r");
	if(file == NULL)
	{
		printf("Error al abrir el archivo\n");
		exit(1);
	}
	procces* p = init_procces(3,2,-1);
	list* l = init_list(p);
	pop_front(l);
	size_t a, b;
	char c;
	int i = 0;
	while (!feof(file))
	{
		fscanf(file,"%ld%ld", &a, &b);       
		if(!feof(file))
        {
		    p = init_procces(a, b, i++);
            fscanf(file, "%c", &c);        
            while (c == ' ' && !feof(file))
            {            
                fscanf(file,"%ld%ld", &a, &b);
                set_IO_operation(p, a, b);
                fscanf(file, "%c", &c);
            }
            push_back(l, p);
        }
	}
	return l;
}
