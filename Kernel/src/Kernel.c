#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <commons/config.h>


typedef struct {
  int PUERTO_PROG;
  int PUERTO_CPU;
  char* IP_MEMORIA;    //AVERIGUAR TIPO DE DATO
  int PUERTO_MEMORIA;
  char* IP_FS;    //AVERIGUAR TIPO DE DATO
  int PUERTO_FS;
  int QUANTUM;
  int QUANTUM_SLEEP;
  char* ALGORITMO;
  int GRADO_MULTIPROG;
  char** SEM_IDS;
  char** SEM_INIT;
  char** SHARED_VARS;
  int STACK_SIZE;
} CONFIGURACION_KERNEL;


void leerArchivoConfiguracion();

CONFIGURACION_KERNEL *config_kernel;


int main(void) {

	printf("hola");

	leerArchivoConfiguracion();


	return 0;
}



void leerArchivoConfiguracion (){
	char* path = "./kernel.cfg";
	t_config* fileKernel = config_create(path);

	printf(config_get_int_value(fileKernel, "PUERTO_PROG"));

	config_kernel->PUERTO_PROG = config_get_int_value(fileKernel, "PUERTO_PROG");
	config_kernel->PUERTO_CPU = config_get_int_value(fileKernel, "PUERTO_CPU");
	config_kernel->IP_MEMORIA = config_get_string_value(fileKernel, "IP_MEMORIA");
	config_kernel->PUERTO_MEMORIA = config_get_int_value(fileKernel, "PUERTO_MEMORIA");
	config_kernel->IP_FS = config_get_string_value(fileKernel, "IP_FS");
	config_kernel->PUERTO_FS = config_get_int_value(fileKernel, "PUERTO_FS");
	config_kernel->QUANTUM = config_get_int_value(fileKernel, "QUANTUM");
	config_kernel->QUANTUM_SLEEP = config_get_int_value(fileKernel, "QUANTUM_SLEEP");
	config_kernel->ALGORITMO = config_get_string_value(fileKernel, "ALGORITMO");
	config_kernel->GRADO_MULTIPROG = config_get_int_value(fileKernel, "GRADO_MULTIPROG");
	config_kernel->SEM_IDS = config_get_array_value(fileKernel, "SEM_IDS");
	config_kernel->SEM_INIT = config_get_array_value(fileKernel, "SEM_INIT");
	config_kernel->SHARED_VARS = config_get_array_value(fileKernel, "SHARED_VARS");
	config_kernel->STACK_SIZE = config_get_int_value(fileKernel, "STACK_SIZE");


	printf(config_kernel->PUERTO_CPU);



}



