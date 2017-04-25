#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <commons/config.h>
#include <SocketLibrary.h>
#include <ConfigLibrary.h>


#define TRUE   1
#define FALSE  0
#define PORT 8888

typedef struct kernell_configuracion {
	int PUERTO_PROG;
	int PUERTO_CPU;
	char* IP_MEMORIA;
	int PUERTO_MEMORIA;
	char* IP_FS;
	int PUERTO_FS;
	int QUANTUM;
	int QUANTUM_SLEEP;
	char* ALGORITMO;
	int GRADO_MULTIPROG;
	int STACK_SIZE;
	char** SEM_IDS;
	char** SEM_INIT;
	char** SHARED_VARS;
} kernell_configuracion;


char get_campo_config_char(t_config* archivo_configuracion, char* nombre_campo) {
	char* valor;
	if(config_has_property(archivo_configuracion, nombre_campo)){
		valor = config_get_string_value(archivo_configuracion, nombre_campo);
		printf("El %s es: %s\n", nombre_campo, valor);
		return valor;
	}
	return NULL;
}

int get_campo_config_int(t_config* archivo_configuracion, char* nombre_campo) {
	int valor;
	if(config_has_property(archivo_configuracion, nombre_campo)){
		valor = config_get_int_value(archivo_configuracion, nombre_campo);
		printf("El %s es: %i\n", nombre_campo, valor);
		return valor;
	}
	return NULL;
}

int get_campo_config_array(t_config* archivo_configuracion, char* nombre_campo) {
	char** valor;
	if(config_has_property(archivo_configuracion, nombre_campo)){
		valor = config_get_array_value(archivo_configuracion, nombre_campo);
		printf("El %s es: %s\n", nombre_campo, valor);
		return valor;
	}
	return NULL;
}


kernell_configuracion get_configuracion() {
	puts("Inicializando proceso Kernel\n");
	kernell_configuracion configuracion;
	// Obtiene el archivo de configuracion
	char* path = "/home/utnso/workspace/tp-2017-1c-AsadoClash/Kernel/config-kernell.cfg";
	t_config* archivo_configuracion = config_create(path);

	configuracion.PUERTO_PROG = get_campo_config_int(archivo_configuracion, "PUERTO_PROG");
	configuracion.PUERTO_CPU = get_campo_config_int(archivo_configuracion, "PUERTO_CPU");
	configuracion.IP_MEMORIA = get_campo_config_char(archivo_configuracion, "IP_MEMORIA");
	configuracion.PUERTO_MEMORIA = get_campo_config_int(archivo_configuracion, "PUERTO_MEMORIA");
	configuracion.IP_FS = get_campo_config_char(archivo_configuracion, "IP_FS");
	configuracion.PUERTO_FS = get_campo_config_int(archivo_configuracion, "PUERTO_FS");
	configuracion.QUANTUM = get_campo_config_int(archivo_configuracion, "QUANTUM");
	configuracion.QUANTUM_SLEEP = get_campo_config_int(archivo_configuracion, "QUANTUM_SLEEP");
	configuracion.ALGORITMO = get_campo_config_char(archivo_configuracion, "ALGORITMO");
	configuracion.GRADO_MULTIPROG = get_campo_config_int(archivo_configuracion, "GRADO_MULTIPROG");
	configuracion.SEM_IDS = get_campo_config_array(archivo_configuracion, "SEM_IDS");
	configuracion.SEM_INIT = get_campo_config_array(archivo_configuracion, "SEM_INIT");
	configuracion.SHARED_VARS = get_campo_config_array(archivo_configuracion, "SHARED_VARS");
	configuracion.STACK_SIZE = get_campo_config_int(archivo_configuracion, "STACK_SIZE");
	return configuracion;
}


int main(void){
	kernell_configuracion configuracion = get_configuracion();
	int opt = TRUE;
	    int master_socket , addrlen , new_socket , client_socket[30] ,
	          max_clients = 30 , activity, i , valread , sd;
	    int max_sd;
	    struct sockaddr_in address;

	    char buffer[1025];  //data buffer of 1K

	    //set of socket descriptors
	    fd_set readfds;

	    //initialise all client_socket[] to 0 so not checked
	    for (i = 0; i < max_clients; i++)
	    {
	        client_socket[i] = 0;
	    }

	    //create a master socket
	    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
	    {
	        perror("socket failed");
	        exit(EXIT_FAILURE);
	    }

	    //set master socket to allow multiple connections ,
	    //this is just a good habit, it will work without this
	    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
	          sizeof(opt)) < 0 )
	    {
	        perror("setsockopt");
	        exit(EXIT_FAILURE);
	    }

	    //type of socket created
	    address.sin_family = AF_INET;
	    address.sin_addr.s_addr = INADDR_ANY;
	    address.sin_port = htons( PORT );

	    //bind the socket to localhost port 8888
	    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
	    {
	        perror("bind failed");
	        exit(EXIT_FAILURE);
	    }
	    printf("Listener on port %d \n", PORT);

	    //try to specify maximum of 3 pending connections for the master socket
	    if (listen(master_socket, 3) < 0)
	    {
	        perror("listen");
	        exit(EXIT_FAILURE);
	    }

	    //accept the incoming connection
	    addrlen = sizeof(address);
	    puts("Waiting for connections ...");

	    while(TRUE)
	    {
	        //clear the socket set
	        FD_ZERO(&readfds);

	        //add master socket to set
	        FD_SET(master_socket, &readfds);
	        max_sd = master_socket;

	        //add child sockets to set
	        for ( i = 0 ; i < max_clients ; i++)
	        {
	            //socket descriptor
	            sd = client_socket[i];

	            //if valid socket descriptor then add to read list
	            if(sd > 0)
	                FD_SET( sd , &readfds);

	            //highest file descriptor number, need it for the select function
	            if(sd > max_sd)
	                max_sd = sd;
	        }

	        //wait for an activity on one of the sockets , timeout is NULL ,
	        //so wait indefinitely
	        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);

	        if ((activity < 0) && (errno!=EINTR))
	        {
	            printf("select error");
	        }

	        //If something happened on the master socket ,
	        //then its an incoming connection
	        if (FD_ISSET(master_socket, &readfds))
	        {
	            if ((new_socket = accept(master_socket,
	                    (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
	            {
	                perror("accept");
	                exit(EXIT_FAILURE);
	            }

	            //inform user of socket number - used in send and receive commands
	            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));


	            //a message
	            char *message = "hola";

	            //memcpy(&message, address.sin_port, 5);

	            //send new connection greeting message
	            if( send(new_socket, message, strlen(message), 0) != strlen(message) )
	            {
	                perror("send");
	            }

	            puts("Welcome message sent successfully");

	            //
	            char* buffer1 = malloc(1000);

	            int bytesRecibidos = recv(new_socket, buffer1, 1000, 0);
	            buffer[bytesRecibidos] = '\0';
	            printf("Me llegaron %d bytes con %s\n", bytesRecibidos, buffer1);

	            free(buffer1);
	            //

	            //add new socket to array of sockets
	            for (i = 0; i < max_clients; i++)
	            {
	                //if position is empty
	                if( client_socket[i] == 0 )
	                {
	                    client_socket[i] = new_socket;
	                    printf("Adding to list of sockets as %d\n" , i);

	                    break;
	                }
	            }
	        }

	        //else its some IO operation on some other socket
	        for (i = 0; i < max_clients; i++)
	        {
	            sd = client_socket[i];

	            if (FD_ISSET( sd , &readfds))
	            {
	                //Check if it was for closing , and also read the
	                //incoming message
	                if ((valread = read( sd , buffer, 1024)) == 0)
	                {
	                    //Somebody disconnected , get his details and print
	                    getpeername(sd , (struct sockaddr*)&address , \
	                        (socklen_t*)&addrlen);
	                    printf("Host disconnected , ip %s , port %d \n" ,
	                          inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

	                    //Close the socket and mark as 0 in list for reuse
	                    close( sd );
	                    client_socket[i] = 0;
	                }

	                //Echo back the message that came in
	                else
	                {
	                    //set the string terminating NULL byte on the end
	                    //of the data read
	                    buffer[valread] = '\0';
	                    send(sd , buffer , strlen(buffer) , 0 );
	                }
	            }
	        }
	    }

	    return 0;
}


/*
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

*/

