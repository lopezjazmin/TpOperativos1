/*
 * ConfigLibrary.h
 *
 *  Created on: 24/4/2017
 *      Author: utnso
 */

#ifndef CONFIGLIBRARY_H_
#define CONFIGLIBRARY_H_

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdint.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <commons/string.h>
#include <commons/error.h>

int MAXIMO_TAMANO_DATOS = 100;

// no toma las commons fijarse como corregirlo
/*
struct hash_element{
		char *key;
		unsigned int hashcode;
		void *data;
		struct hash_element *next;
	};
	typedef struct hash_element t_hash_element;


typedef struct {
		t_hash_element **elements;
		int table_max_size;
		int table_current_size;
		int elements_amount;
	} t_dictionary;


typedef struct {
		char *path;
		t_dictionary *properties;
	} t_config;
*/


#endif /* CONFIGLIBRARY_H_ */
