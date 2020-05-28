#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int  argc, char* argv[])
{
	char commande[80];
	setuid(0);
	sprintf(commande,"./envoyerSignal.sh %s",  argv[1]);
	system(commande);
	return 0;
}
