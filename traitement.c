#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "structures.h"
#include "traitement.h"



Image *niveauDeGrisPGMraw(Image *image, int grisMin, int grisMax)
{
	/*Données : image, l’image originale, grisMin : le niveau de gris minimal de l’image à remettre à zéro, grisMax : le niveau de gris maximal à remettre à 255
	Résultat : une copie de l’image avec le nouveau niveau de gris spécifié
	Complexite d'ordre n^2
	On recopie la matrice sans les valeurs*/
	int** tab = new2Dtab(image->dim->largeur,image->dim->hauteur);
	Dimension *dim = newDim(image->dim->largeur,image->dim->hauteur);
	Image *image2 = newImage(tab,dim);

	if(grisMin>grisMax)
	{
		/*Si erreur, on renvoie une copie de l'image actuelle*/
		printf("La valeur minimale du niveau de gris doit etre inferieure a la valeure maximale. L'image ne sera pas modifiee\n"); 
		/*Invariant : image2->img[0..i][0..j] contient les memes valeurs que image->img[0..i][0..j]*/
		int i,j;
		for (i = 0 ; i < image->dim->hauteur ; i++)
			for (j = 0 ; j < image->dim->largeur ; j++)
				image2->img[i][j] = image->img[i][j];
		return image2;
	}
	int moy = (grisMin+grisMax)/2;
	/*Partie traitement de l'image
	Distance entre le blanc et le nouveau gris max*/
	float distMax = (float)(255-grisMax);
	/*Distance entre le noir et le nouveau gris min*/
	float distMin = (float)(0+grisMin);
	/*Invariant : image2->img[0..i][0..j] est contraste*/
	int i,j;
	float coefficient;
	int difference;
	for (i = 0 ; i < image->dim->hauteur ; i++)
	{
		for (j = 0 ; j < image->dim->largeur ; j++)
		{
			/*Si la valeur est superieure a la moyenne alors :
			On calcule un coefficient compris entre 0 et 1, en fonction de la distance de la valeur actuelle, entre la valeur moyenne et le gris maximum. Si valeur=moyenne -> coef=0. Si valeur=grismax -> coef=1
			On multiplie ce coefficient avec la distance max impose par l'utilisateur, qui donne la difference a rajouter a la valeur actuelle du pixel*/
			if (image->img[i][j]>=moy)
			{
				coefficient = ( (float)(image->img[i][j] - moy) / (float)(grisMax - moy) );
				difference =  (int)(coefficient * distMax);
			}
			/*Sinon
			On calcule un coefficient compris entre 0 et 1, en fonction de la distance de la valeur actuelle, entre la valeur moyenne et le gris minimum. Si valeur=moyenne -> coef=0. Si valeur=grismin -> coef=1
			On multiplie ce coefficient avec la distance min impose par l'utilisateur, qui donne la difference a rajouter a la valeur actuelle du pixel*/
			else
			{
				coefficient = ( (float)(moy-image->img[i][j]) / (float)(moy-grisMin) );
				difference = (int)(-coefficient * distMin);
			}
			/*On ajoute la difference*/
			image2->img[i][j] = image->img[i][j] + difference;
			/*On termine par verifier si la valeur depasse des bornes, si oui : on le corrige*/
			if(image2->img[i][j] < 0)
				image2->img[i][j] = 0;
			if(image2->img[i][j] > 255)
				image2->img[i][j] = 255;
		}
	}
	return image2;
}

Image *contrasteAutoPGMraw(Image *image)
{
	/*Donnée : image, l’image originale
	Résultat : une copie de l’image avec un niveau de gris optimisé
	Complexité : O(n*n), plus précisement largeur*hauteur*/
	int maxi = 0;
	int mini = 255;
	int i,j;
	for (i = 0 ; i < image->dim->hauteur ; i++)
	{
		for (j = 0 ; j < image->dim->largeur ; j++)
		{
			if(image->img[i][j]>maxi)
				maxi = image->img[i][j];
			if(image->img[i][j]<mini)
				mini = image->img[i][j];
		}
	}
	return niveauDeGrisPGMraw(image,mini,maxi);
}

Image* negatifPGMraw(Image *image)
{
	int **tab = new2Dtab(image->dim->largeur, image->dim->hauteur);
	Image* image2 = newImage(tab, newDim(image->dim->largeur, image->dim->hauteur));
	int i, j;
	for (i = 0; i < image->dim->hauteur; ++i)
	{
		for (j = 0; j < image->dim->largeur; ++j)
		{
			image2->img[i][j] = 255 - image->img[i][j];
		}
	}
	return image2;
}

Image* pivot90PGMraw(Image *image)
{
	/*Recopie de l'image actuelle. La largeur correspond desormais a la hauteur et vice versa*/
	int **tab = new2Dtab(image->dim->hauteur, image->dim->largeur);
	Image* image2 = newImage(tab, newDim(image->dim->hauteur, image->dim->largeur));
	int i, j;
	/*Rotation de 90 degre dans le sens des aiguilles d'une montre de chaque pixel*/
	/*Invariant : image2->img[0..i][0..j] est tourne de 90 degre dans le sens des aiguilles d'une montre*/
	for (i = 0; i < image->dim->hauteur; ++i)
	{
		for (j = 0; j < image->dim->largeur; ++j)
		{
			image2->img[i][j] = image->img[image->dim->hauteur-j-1][i];
		}
	}
	/*Renvoi de l'image*/
	return image2;
}

Image* pivotMoins90PGMraw(Image *image)
{
	/*Recopie de l'image actuelle. La largeur correspond desormais a la hauteur et vice versa*/
	int **tab = new2Dtab(image->dim->hauteur, image->dim->largeur);
	Image* image2 = newImage(tab, newDim(image->dim->hauteur, image->dim->largeur));
	int i, j;
	/*Rotation de 90 degre dans le sens inverse des aiguilles d'une montre de chaque pixel*/
	/*Invariant : image2->img[0..i][0..j] est tourne de 90 degre dans le sens inverse des aiguilles d'une montre*/
	for (i = 0; i < image->dim->hauteur; ++i)
	{
		for (j = 0; j < image->dim->largeur; ++j)
		{
			image2->img[i][j] = image->img[j][i];
		}
	}
	/*Renvoi de l'image*/
	return image2;
}

Image* miroirHorizontalPGMraw(Image *image)
{
	/*Recopie de l'image actuelle*/
	int **tab = new2Dtab(image->dim->largeur, image->dim->hauteur);
	Image* image2 = newImage(tab, newDim(image->dim->largeur, image->dim->hauteur));
	int i, j;
	/*Inversion horizontale de chaque pixel
	Invariant : image2->img[0..i][0..j] est inversé horizontalement*/
	for (i = 0; i < image->dim->hauteur; ++i)
	{
		for (j = 0; j < image->dim->largeur; ++j)
		{
			image2->img[i][j] = image->img[i][image->dim->largeur-j-1];
		}
	}
	/*Renvoi de l'image*/
	return image2;
}

Image* miroirVerticalPGMraw(Image *image)
{
	/*Recopie de l'image actuelle*/
	Dimension *dim2 = newDim(image->dim->largeur, image->dim->hauteur);
	int **tab = new2Dtab(dim2->largeur, dim2->hauteur);
	Image* image2 = newImage(tab,dim2);
	int i, j;
	/*Inversion verticale de chaque pixel
	Invariant : image2->img[0..i][0..j] est inversé verticalement*/
	for (i = 0; i < image->dim->hauteur; ++i)
	{
		for (j = 0; j < image->dim->largeur; ++j)
		{
			image2->img[i][j] = image->img[image->dim->hauteur-i-1][j];
		}
	}
	/*Renvoi de l'image*/
	return image2;
}


/*def scalePGMraw(image, percentage):
    
    tab = [[0 for x in range((int(image.dim.largeur*percentage)))] for x in range((int(image.dim.hauteur*percentage)))]
    dim = Dimension((int(image.dim.hauteur*percentage)), (int(image.dim.largeur*percentage)))
    # cree la nouvelle image
    image2 = Image(tab,dim)
    # [p1,...,p4] sont les pixels que on regarde pour creer le nouveau pixel, p5, pour la nouvelle image
    p1 = 0; p2 = 0; p3 = 0; p4 = 0; p5 = 0;
    # ii et jj sont variables pour prendre des pixels de la image donnee et les mettre sur la nouvelle image
    ii = 0; jj = 0

   
    for i in range(0, image2.dim.hauteur):
        # Invariant: a la n-eme iteration dans la ligne courent, toutes les positions < j de l'image2  auront les nouveux pixels
        for j in range(0, image2.dim.largeur):
            #prendre des pixels des lignes et des colonnes pour cree la nouvelle image
            ii = int(map(i, image.dim.hauteur, image2.dim.hauteur))
            jj = int(map(j, image.dim.largeur, image2.dim.largeur))

            #prendre 4 pixels de la image donnee pour faire l'interpolation
            p1 = get_pixel(image, ii, jj)
            p2 = get_pixel(image, ii + 1, jj)
            p3 = get_pixel(image, ii, jj + 1)
            p4 = get_pixel(image, ii + 1, jj + 1)

            #l'interpolation. le nouveau pixel aura la moyenne des autres pixels
            p5 = (p1 + p2 + p3 + p4) / 4
            #ajouter le nouveau pixel a la nouvelle image
            image2.img[i][j] = p5


    return image2
*/

Image *scalePGMraw (Image * image, float percentage) {
    //Donnée : image, l’image originale, percentage : le pourcentage de redimensionnement
    //Résultat : une copie de l’image redimensionnée
    //cree une nouvelle matrice et une nouvelle dimension pour la nouvelle image

    int** tab = new2Dtab(image->dim->largeur * percentage,image->dim->hauteur * percentage);
    Dimension *dim = newDim(image->dim->largeur * percentage,image->dim->hauteur * percentage);
    Image *image2 = newImage(tab,dim);

    //ii et jj sont variables pour prendre des pixels de la image donnee et les mettre sur la nouvelle image
    int i, j, ii, jj;

    //[p1,...,p4] sont les pixels que on regarde pour creer le nouveau pixel, p5, pour la nouvelle image
    int p1, p2, p3, p4, p5;

    //Invariant: a la n-eme iteration, toutes les lignes < i de l'image2  auront les nouveux pixels
    for (i = 0; i < image2->dim->hauteur; ++i) {
        for (j = 0; j < image2->dim->largeur; j++) {

        	//prendre des pixels des lignes et des colonnes pour cree la nouvelle image
            ii = map(i, image->dim->hauteur, image2->dim->hauteur);
            jj = map(j, image->dim->largeur, image2->dim->largeur);

            //prendre 4 pixels de la image donnee pour faire l'interpolation
            p1 = get_pixel(image, ii, jj);
            p2 = get_pixel(image, ii + 1, jj);
            p3 = get_pixel(image, ii , jj + 1);
            p4 = get_pixel(image, ii + 1, jj + 1);

            //l'interpolation. le nouveau pixel aura la moyenne des autres pixels
            p5 = (p1 + p2 + p3 + p4) / 4 ;
            
            //ajouter le nouveau pixel a la nouvelle image
            image2->img[i][j] = p5;
        }
    }

    return image2;
}