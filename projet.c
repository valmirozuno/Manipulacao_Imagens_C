#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct
{
    int largeur,hauteur; 
} Dimension;

Dimension *newDim(int largeur,int hauteur)
{
    Dimension *dim;
    dim = malloc(sizeof(Dimension));
    dim->largeur = largeur;
    dim->hauteur = hauteur;
    return dim;
}

typedef struct
{
    int **img;
    Dimension *dim;
} Image;

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

/*def scalePGMraw(image, percentage):
    #Donnée : image, l’image originale, percentage : le pourcentage de redimensionnement
    #Résultat : une copie de l’image redimensionnée
    # cree une nouvelle matrice et une nouvelle dimension pour la nouvelle image
    tab = [[0 for x in range((int(image.dim.largeur*percentage)))] for x in range((int(image.dim.hauteur*percentage)))]
    dim = Dimension((int(image.dim.hauteur*percentage)), (int(image.dim.largeur*percentage)))
    # cree la nouvelle image
    image2 = Image(tab,dim)
    # [p1,...,p4] sont les pixels que on regarde pour creer le nouveau pixel, p5, pour la nouvelle image
    p1 = 0; p2 = 0; p3 = 0; p4 = 0; p5 = 0;
    # ii et jj sont variables pour prendre des pixels de la image donnee et les mettre sur la nouvelle image
    ii = 0; jj = 0

    # Invariant: a la n-eme iteration, toutes les lignes < i de l'image2  auront les nouveux pixels
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
    int** tab = new2Dtab(image->dim->largeur * percentage,image->dim->hauteur * percentage);
    Dimension *dim = newDim(image->dim->largeur * percentage,image->dim->hauteur * percentage);
    Image *image2 = newImage(tab,dim);

    int i, j, ii, jj;
    int p1, p2, p3, p4, p5;


    for (i = 0; i < image2->dim->hauteur; ++i) {
        for (j = 0; j < image2->dim->largeur; j++) {

            ii = map(i, image->dim->hauteur, image2->dim->hauteur);
            jj = map(j, image->dim->largeur, image2->dim->largeur);


            p1 = get_pixel(image, ii, jj);
            p2 = get_pixel(image, ii + 1, jj);
            p3 = get_pixel(image, ii , jj + 1);
            p4 = get_pixel(image, ii + 1, jj + 1);

            p5 = (p1 + p2 + p3 + p4) / 4 ;
            

            image2->img[i][j] = p5;
        }
    }
    printf("\n");

    return image2;
}

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

bool estCommentaire(char * ligne)
{
    return ligne[0] == '#';
}

/*char * recupererLigne(FILE fichier, int nblignes, bool sauterCommentaires):*/

/*int recupererCouleur(char couleurCaractere)*/

/*void ecrireCouleur(FILE fichier,int couleur) :*/

void afficherAide(char * entete, char * utilisation, char * description)
{
    printf("%s",entete);
    printf("%s",utilisation);
    printf("%s",description);
}

int main(int argc, char *argv[])
{
    /*Image *image = lirePGMraw("crane.pgm");
    Image *image2 = negatifPGMraw(image);
    ecrirePGMraw(image2, "result.pgm");
    return 0;*/
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
                    printf("Pourcentage : %f \n", pourcentage*100);
                    i = i + 1;
                    imSource = scalePGMraw(imSource,pourcentage);
                }
                else if(strcmp ( argv[i] , "-autoc\n") == 0)
                {
                    printf("Activation des contrastes automatiques\n");
                    imSource = contrasteAutoPGMraw(imSource);
                }
                else if(strcmp ( argv[i] , "-ndg") == 0)
                {
                    printf("Le niveau de gris sera changé avec les paramètres suivants : \n");
                    /*try :
                    grisMin = (int)(argv[i+1])
                    i = i + 1
                    grisMax = (int)(argv[i+1])
                    i = i + 1
                    except (ValueError) :
                    print "Les valeurs entrées sont incorrectes. Syntaxe de la fonction : -ndg [min] [max] ..."
                    return(1)
                    print "Gris minimal : "+str(grisMin)+", gris maximal : "+ str(grisMax)
                    imSource = niveauDeGrisPGMraw(imSource,grisMin,grisMax)*/
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