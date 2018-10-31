#include "structures.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


Dimension *newDim(int largeur,int hauteur)
{
    Dimension *dim;
    dim = malloc(sizeof(Dimension));
    dim->largeur = largeur;
    dim->hauteur = hauteur;
    return dim;
}

Image *newImage(int **img, Dimension *dim)
{
	Image *image;
	image = malloc(sizeof(Image));
	image->img = img;
	image->dim = dim;
	return image;
}

int **new2Dtab(int largeur,int hauteur)
{
	/*Création de la premiere dimension du tableau*/
	int **tab=(int **)malloc(hauteur * sizeof(int *));

	int i,j;
	/*Création de la deuxieme dimension du tableau*/
	for(i = 0 ; i < hauteur ; i++)
	{
		tab[i] = (int *)malloc(largeur * sizeof(int));
		for(j = 0 ; j < largeur ; j++)
		{
			tab[i][j] = 0;
		}
	}
	return tab;
}



Dimension * recupererDimensions(char * line)
{
	int i = 0, j = 0;
	char *largeur, *hauteur;
	/*Etape 1 : Largeur*/
	while(line[i] != ' ')
	{
		i++;
	}
	largeur = (char *) malloc (i+1);
	i = 0;
	while(line[i] != ' ')
	{
		largeur[i] = line[i];
		i++;
	}
	i++;
	largeur[i] = '\0';

	/*Etape 2 : Hauteur*/
	while(line[i + j] != ' ')
	{
		j++;
	}
	hauteur = (char *) malloc (j+1);
	j = 0;
	while(line[i + j] != ' ')
	{
		hauteur[j] = line[i + j];
		j++;
	}
	j++;
	hauteur[j] = '\0';

	/*Etape 3 : création du type*/
	i = atoi(largeur);
	j = atoi(hauteur);
	return newDim(i, j);
}

Image * estPGMrawValide(char* path)
{
	/*On ouvre le fichier*/
	FILE* file = fopen(path, "r");

	char line[256];
	fgets(line, sizeof(line), file);

	/*Vérification de la premiere ligne*/
	if (!(line[0] == 'P' && line[1] == '5' && line[2] == '\n'))
	{
		return NULL;
	}

	/*On recherche la ligne des dimensions*/
	fgets(line, sizeof(line), file);
	while(line[0] == '#')
	{
		fgets(line, sizeof(line), file);
	}

	/*On récupère les dimensions*/
	Dimension *dim = recupererDimensions(line);

	/*Vérification de la troisieme ligne*/
	fgets(line, sizeof(line), file);
	if (!(line[0] == '2' && line[1] == '5' && line[2] == '5' && line[3] == '\n'))
	{
		return NULL;
	}

	/*Création du tableau à deux dimensions*/
	int** tab = new2Dtab(dim->largeur,dim->hauteur);

	/*Remplissage du tableau*/
	int i = 0;
	int couleur= fgetc(file);
	while(couleur != EOF)
	{
		tab[i / dim->largeur][i % dim->largeur] = couleur;
		couleur = fgetc(file);
		i++;
	}
	if (i != dim->largeur * dim->hauteur)
	{
		return NULL;
	}
	fclose(file);
	Image * img = newImage(tab,dim);
	return img;
}


Image * lirePGMraw(char * chemin)
{
	return estPGMrawValide(chemin);
}

void ecrirePGMraw(Image *image, char* chemin)
{
	FILE *fichier = fopen(chemin, "w");
	int i, j;
	fprintf(fichier, "P5\n");
	fprintf(fichier, "%d %d\n", image->dim->largeur,image->dim->hauteur);
	fprintf(fichier, "255\n");
	for (i = 0; i < image->dim->hauteur; ++i)
	{
		for (j = 0; j < image->dim->largeur; ++j)
		{
			fprintf(fichier, "%c", image->img[i][j]);
		}
	}
	fclose(fichier);
}



bool estCommentaire(char * ligne)
{
	return ligne[0] == '#';
}


//Fonction pour connaitre qui pixel d'une image donnee  aura une correspondance avec une pixel d'une autre image avec une taille different
//Complexite O(1)
//w1 est l'hauteur ou la largeur de l'image original, w2 est l'hauteur ou la largeur de la nouvelle image et aux une position pour choisir le pixel correct
float map(int a, int w1, int w2) {
    //Données : aux : la position (x ou y) de base d’un pixel, w1 : une longueur (largeur ou hauteur) originale, w2 : une longeur (largeur ou hauteur) de destination
    //Résultat : res de type float, la position (x ou y) finale d’un pixel
    return a * w1 / (float) w2;
}


int get_pixel(Image *image, int i, int j){
    //Donnée : image: l'image, i : la position x du pixel, j : la position y du pixel
    //Résultat : le pixel correspondant au (x,y) spécifié
    if (i < 0)
        i = 0;
    if (i >= image->dim->hauteur)
        i = image->dim->hauteur - 1;
    if (j < 0)
        j = 0;
    if (j >= image->dim->largeur)
        j = image->dim->largeur - 1;

    return image->img[i][j];  
}



void afficherAide(char * entete, char * utilisation, char * description)
{
	printf("%s",entete);
	printf("%s",utilisation);
	printf("%s",description);
}