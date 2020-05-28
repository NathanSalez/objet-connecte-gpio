/**
 * TODO  : CHECK A FAIRE
 * Ce programme réalise les fonctionnalités suivantes :
 * - lorsque le bouton poussoir est pressé, la led allumée s'éteint 
 * 	  et une autre s'allume.
 * - Le cycle d'allume/extinction des LEDs est le suivant :
 *     VERTE => ROUGE => ORANGE => JAUNE
 * - Lorsque le signal SIGINT est envoyé, la LED allumée s'éteint 
 *  et sa valeur est sauvegardée dans le fichier resultat.
 * - Lorsque le signal SIGTERM est envoyé, la LED allumée s'éteint 
 * et le programme s'arrête.
 * On doit s'assurer qu'il n'y ait qu'une seule instance du programme 
 * qui s'execute dans le système pendant le jeu de cache numérique.
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include "gpio_utils.h"
#include "utils.h"

clock_t horlogeAntiSpam = 0;
#define NB_CYCLES_ANTI_SPAM 250000 //0.25 seconde

#define CHEMIN_FICHIER_RESULTAT "resultat"
#define CHEMIN_FICHIER_PID "pid"

/* 
 * Etant donné qu'on considère qu'un seul processus manipule les LED,
 * une variable globale donnant la valeur de la LED allumée
 * est largement suffisante.
 */
octet LEDAllumee = PIN_LED_DEFAUT;

octet obtenirLEDSuivant(octet LED);
void changerLEDAllumee();
/**
 * Retourne true si le résultat a été sauvegardé, false sinon.
 */ 
bool sauvegarderResultat(char * resultat, char * nomFichier);
bool inscrirePID(char * nomFichier);
void gererSignal(int numSignal);
/**
 * Le programme ne peut être interrompu que par 1 signal : SIGINT et SIGTERM
 * Lorsque l'interruption est provoquée, cette dernière ne peut pas être interrompue.
 */ 
void initialiserGestionnaireSignaux();
/**
 * S'il y a déjà une instance de programme en cours, alors on l'arrête à l'aide de SIGTERM
 */ 
void verifierUniciteExecution();

int main()
{
	verifierUniciteExecution();
	inscrirePID(CHEMIN_FICHIER_PID);
	fprintf(stdout,"Inscription du PID réalisé.\n");
	wiringPiSetup();
	fprintf(stdout,"wiring PI !\n");
	initGPIO();
	allumerLED(PIN_LED_DEFAUT);
	fprintf(stdout,"Ports GPIO initialisés.\n");
	initialiserGestionnaireSignaux();
	fprintf(stdout,"Gestionnaire de signaux initialisé.\n");
	
	wiringPiISR(PIN_POUSSOIR,INT_EDGE_FALLING,changerLEDAllumee);
	fprintf(stdout,"Mise en relation du bouton avec le programme réalisé.\n");
	fprintf(stdout,"En attente du choix de l'utilisateur ..\n");
	
	for(;;){}
	
	return 0;
}

bool inscrirePID(char * nomFichier)
{
	char pidString[10];
	sprintf(pidString,"%d",getpid());
	return ecrireFichier(pidString,nomFichier);
}

octet obtenirLEDSuivant(octet LED)
{
	switch(LED)
	{
		case PIN_LED_ROUGE : return PIN_LED_ORANGE;
		case PIN_LED_VERTE : return PIN_LED_ROUGE;
		case PIN_LED_ORANGE : return PIN_LED_JAUNE;
		case PIN_LED_JAUNE : return PIN_LED_VERTE;
		default : return PIN_LED_DEFAUT;
	}
}

void changerLEDAllumee()
{
	if( clock() > horlogeAntiSpam + NB_CYCLES_ANTI_SPAM)
	{
		horlogeAntiSpam = clock();
		printf("On change  de diode !\n");
		octet LEDSuivant =- 1;
		fprintf(stdout,"LED à éteindre : %d\n",LEDAllumee);
		LEDSuivant = obtenirLEDSuivant(LEDAllumee);
		fprintf(stdout,"LED à allumer : %d\n",LEDSuivant);
		eteindreLED(LEDAllumee);
		allumerLED(LEDSuivant);
		LEDAllumee = LEDSuivant;
	} else
	{
		printf("Spam détecté\n");
	}
}

void gererSignal(int numsignal)
{
	// https://stackoverflow.com/questions/8257714/how-to-convert-an-int-to-string-in-c
	int longueurNombre = snprintf(NULL, 0,"%d",LEDAllumee);
	char * resultat;
	switch(numsignal)
	{
		case SIGINT:
			fprintf(stdout,"\nSignal SIGINT détecté !\nRécupération et sauvegarde du résultat en cours ..\n"); 
			eteindreLED(LEDAllumee);
			resultat = malloc( longueurNombre * sizeof(char) ) ;
			sprintf(resultat,"%d",(int)LEDAllumee);
			if( ecrireFichier(resultat,CHEMIN_FICHIER_RESULTAT) == false )
			{
				fprintf(stderr,"Impossible de sauvegarder le résultat.\n");
				exit(EXIT_FAILURE);
			}
			free(resultat);
			remove(CHEMIN_FICHIER_PID);
			fprintf(stdout,"Sauvegarde du résultat terminée !\n");
			exit(EXIT_SUCCESS);
			
		case SIGTERM:
			eteindreLED(LEDAllumee);
			fprintf(stdout,"Programme arrêté prématurément\n.");
			remove(CHEMIN_FICHIER_PID);
			exit(EXIT_SUCCESS);
	}
}

void initialiserGestionnaireSignaux()
{
	sigset_t masque;
	struct sigaction interruption;
	sigfillset(&masque);
	sigdelset(&masque,SIGINT);
	sigdelset(&masque,SIGTERM);
	
	sigprocmask(SIG_SETMASK,&masque,NULL);
	interruption.sa_handler = gererSignal;
	sigfillset(&interruption.sa_mask);
	sigaction(SIGINT,&interruption,NULL);
	sigaction(SIGTERM,&interruption,NULL);
}

void verifierUniciteExecution()
{
	if( verifierPresenceFichier(CHEMIN_FICHIER_PID) )
	{
		FILE * pFichier = fopen(CHEMIN_FICHIER_PID,"r");
		if( pFichier != NULL )
		{
			int pid;
			fscanf(pFichier,"%d",&pid);
			fclose(pFichier);
			kill(pid,SIGTERM);
			fprintf(stdout,"En attente de la fermeture du programme précédent..\n");
			delay(500);
			fprintf(stdout,"Programme précédent (normalement) fermé !\n");
		} else
		{
			fprintf(stderr,"Impossible de créer le fichier %s.",CHEMIN_FICHIER_PID);
			fprintf(stderr,"Fermeture du programme de pid %d\n.",getpid());
			exit(EXIT_FAILURE);
		}
	}
}
