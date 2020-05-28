#include  "utils.h"

bool ecrireFichier(char* chaine,  char* nomFichier)
{
	if( nomFichier == NULL || chaine  == NULL)
		return false;
		
	FILE * pFichier = fopen(nomFichier,"w");
	if( pFichier != NULL  )
	{
		fprintf(pFichier,chaine);
		fclose(pFichier);
		return true;
	} else
	{
		fprintf(stderr,"Impossible de créer le fichier %s.",nomFichier);
		return false;
	}
}

//https://stackoverflow.com/questions/230062/whats-the-best-way-to-check-if-a-file-exists-in-c
bool verifierPresenceFichier(char* nomFichier)
{
	return ( access( nomFichier, F_OK ) == 0 );
}
