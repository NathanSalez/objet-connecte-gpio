#ifndef UTILS
#define UTILS

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#define CHECK(sts, msg)				\
	if (-1 == (sts) && errno != 0) 	\
	{								\
		perror(msg);				\
		exit(EXIT_FAILURE);			\
	}

bool ecrireFichier(char* chaine, char* nomFichier);

/**
 * Retourne true si le fichier dont le chemin est spécifié en entrée, est présent.
 */ 
bool verifierPresenceFichier(char* nomFichier);

#endif
