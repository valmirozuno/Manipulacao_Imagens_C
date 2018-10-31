#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "structures.h"
#include "traitement.h"



int main(int argc, char *argv[])
{
	char entete []= "\npgmproc.c Propose des fonctionnalités permettant le traitement d'images au format PGM.\nLes fonctionnalités disponibles et leur manière d'utilisation sont résumées ci-dessous:\n";
	char utilisation []= "utilisation : ./pgmproc.c [Liste des options avec leurs arguments] [Chemin source] [Chemin destination]\nExemple : ./pgmproc.c -ndg 20 200 -mh canada.pgm canadaModifie.pgm";
	char description []="-h, -help : Affiche l'aide\n-n : Inverse les couleurs de l'image\n-p : Effectue une rotation de 90 degres dans le sens des aiguilles d'une montre\n-pi : Effectue une rotation de 90 degres dans le sens inverse des aiguilles d'une montre\n-sc [Pourcentage] : La taille de l'image est modifié en fonction du pourcentage spécifié\n-autoc : Applique un contraste automatique sur l'image\n-ndg [Gris minimum] [Gris maximum] : modifie les niveaux de gris de l'image en fonction des gris min/max spécifiés\n-mv : L'image est retournée verticalement\n-mh : L'image est retournée horizontalement\n\nChemin source et chemin destination : correspond au chemin relatif a partir duquel vous voulez que l'image soit chargée puis sauvegardée (Indiquez uniquement le nom de l'image si celle ci se trouve dans le meme répertoire que le script\n";

	if(argc==1)
		afficherAide(entete,utilisation,description);
	else if(strcmp (argv[1] , "-help") == 0 || strcmp ( argv[1] , "-h") == 0)
		afficherAide(entete,utilisation,description);
	else
	{
		int i = 1;
		int argmax = argc - 1;
		char * fichierSource = argv[argmax - 1];
		char * fichierDest = argv[argmax];

		/*On va regarder si l'utilisateur a rentré : - un fichier source et destination, - uniquement un fichier source (dans ce cas là, la destination sera la source), - aucun fichier*/
		printf("Ouverture de l'image\n");
		Image * imSource = lirePGMraw(fichierSource);
		if(imSource != NULL)
		{
			printf("Le fichier est valide\n");
		}
		else
		{
			imSource = lirePGMraw(fichierDest);
			if(imSource == NULL)
			{
				printf("Le fichier %s n'existe pas\n",fichierSource);
				return(1);
			}
			else
			{
				fichierSource = fichierDest;
				printf("Le fichier de destination correspondra au fichier source\n");
				argmax = argc;
			}
		}
		if(imSource != NULL)
		{
			printf("Lecture du fichier %s réussie\n",fichierSource);
			/*A ce stade, on a l'image source et le chemin de destination*/
			while (i < argmax - 1)
			{
				printf("Operation %d / %d\n",i,argmax - 2);
				if(strcmp ( argv[i] , "-n") == 0)
				{
					printf("Les couleurs de l'image vont être inversée\n");
					imSource = negatifPGMraw(imSource);
				}
				else if(strcmp ( argv[i] , "-p") == 0)
				{
					printf("L'image va pivoter de 90 degres dans le sens des aiguilles d'une montre\n");
					imSource = pivot90PGMraw(imSource);
				}
				else if(strcmp ( argv[i] , "-pi") == 0)
				{
					printf("L'image va pivoter de 90 degres dans le sens inverse des aiguilles d'une montre\n");
					imSource = pivotMoins90PGMraw(imSource);
				}
				else if(strcmp ( argv[i] , "-sc") == 0)
				{
					printf("Scale : La taille de l'image va être modifiée\n");
					float pourcentage = atof(argv[i+1]);
					if(pourcentage <= 0.0){
                        printf("Le pourcentage ne doit pas être négatif\n");
                        return(1);
                    }
					printf("Pourcentage : %f",(pourcentage*100));
					i = i + 1;
					imSource = scalePGMraw(imSource,pourcentage);
				}
				else if(strcmp ( argv[i] , "-autoc") == 0)
				{
					printf("Activation des contrastes automatiques\n");
					imSource = contrasteAutoPGMraw(imSource);
				}
				else if(strcmp ( argv[i] , "-ndg") == 0)
				{
					printf("Le niveau de gris sera changé avec les paramètres suivants : \n");
					int grisMin = atoi(argv[i+1]);
					i = i + 1;
					int grisMax = atoi(argv[i+1]);
					i = i + 1;
					/*Prendre en compte la gestion de l'erreur : l'utilisateur ne rentre pas des chiffres*/
					/*print "Les valeurs entrées sont incorrectes. Syntaxe de la fonction : -ndg [min] [max] ..."
					return(1)*/
					printf("Gris minimal : %d, gris maximal : %d\n",grisMin,grisMax);
					imSource = niveauDeGrisPGMraw(imSource,grisMin,grisMax);
				}
				else if(strcmp ( argv[i] , "-mv") == 0)
				{
					printf("Miroir vertical de l'image\n");
					imSource = miroirVerticalPGMraw(imSource);
				}
				else if(strcmp ( argv[i] , "-mh") == 0)
				{
					printf("Miroir horizontal de l'image\n");
					imSource = miroirHorizontalPGMraw(imSource);
				}
				else
				{
					printf("L'argument \"%s\" est invalide. Tapez \"python python.py -h\" pour plus de détail\n",argv[i]);
				}
				i = i + 1;
			}
			ecrirePGMraw(imSource,fichierDest);
			printf("Le fichier %s a ete enregistre\n",fichierDest);
		}
	}
	return 0;
}