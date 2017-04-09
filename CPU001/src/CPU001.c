#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

int main(void) {
	struct sockaddr_in direccionServidor;

	char basura[15];
	char conf[15];
	char ip[20];
	char port[4];
	int host;

	FILE *archivoConfig;
	archivoConfig= fopen("/home/utnso/workspace/tp-2017-1c-AsadoClash/Console001/src/configuracionConsole001", "r");
	fgets(&basura ,sizeof(basura)+1,archivoConfig);
	fgets(&conf, sizeof(conf)+1, archivoConfig);
	fgets(&ip, sizeof(ip)+1, archivoConfig);
	fgets(&port, sizeof(port)+1, archivoConfig);


	//printf(conf);
	//printf(ip);
	//printf(port);

	fclose(archivoConfig);

	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr = inet_addr(ip);
	direccionServidor.sin_port = htons(8888);

	int cliente = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(cliente, (void*) &direccionServidor, sizeof(direccionServidor)) != 0) {
		perror("No se pudo conectar");
		return 1;
	}

	while (1) {
		char mensaje[1000];

		recv(cliente, mensaje, 100, 0);
		printf(mensaje);

		scanf("%s", mensaje);
		send(cliente, mensaje, strlen(mensaje), 0);
	}


	return 0;
}
