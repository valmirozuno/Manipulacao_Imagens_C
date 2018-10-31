/*Traitement de l'image*/

#ifndef TRAITEMENT_H_
#define TRAITEMENT_H_


Image *niveauDeGrisPGMraw(Image *image, int grisMin, int grisMax);

Image *contrasteAutoPGMraw(Image *image);

Image* negatifPGMraw(Image *image);

Image* pivot90PGMraw(Image *image);

Image* pivotMoins90PGMraw(Image *image);

Image* miroirHorizontalPGMraw(Image *image);

Image* miroirVerticalPGMraw(Image *image);

Image *scalePGMraw (Image * image, float percentage);

#endif