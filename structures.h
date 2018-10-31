/*Gestion des fichiers, des fonctions auxiliaires et des structures*/

#ifndef STRUCTURES_H_
#define STRUCTURES_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct
{
    int largeur,hauteur; 
} Dimension;


typedef struct
{
    int **img;
    Dimension *dim;
} Image;



Dimension *newDim(int largeur,int hauteur);

Image *newImage(int **img, Dimension *dim);

int **new2Dtab(int largeur,int hauteur);

Dimension * recupererDimensions(char * line);

Image * estPGMrawValide(char* path);

Image * lirePGMraw(char * chemin);

void ecrirePGMraw(Image *image, char* chemin);

bool estCommentaire(char * ligne);

float map(int a, int w1, int w2);

int get_pixel(Image *image, int i, int j);

void afficherAide(char * entete, char * utilisation, char * description);


#endif

