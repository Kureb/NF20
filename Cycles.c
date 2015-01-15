#include <stdio.h>
#include <stdlib.h>

int estDedans(int nb, int* colonne, int compteur);
void afficherMatrice(int** matrice, int taille);
void supprimer(int* colonne, int* ligne, int* pond, int** matrice, int compteur, int nbNode, int** matCouv);
int compterCoutTotal(int nbNode, int** matrice);

int estDedans(int nb, int* colonne, int compteur){
    for (int z = 0; z < compteur; ++z)
    {
        if (colonne[z] == nb)// Si le noeud est déjà présent
             return 1;
    }
    return 0;
}


void afficherMatrice(int** matrice, int taille) {
    for (int i = 0; i < taille ; ++i)
    {
        printf("[ ");
        for (int j = 0; j < taille; ++j)
        {
            printf(" %d ", matrice[i][j]);
        }
        printf(" ]\n");
    }
}



void supprimer (int* colonne, int* ligne, int*pond, int** matrice, int compteur, int nbNode, int** matCouv) {
    //printf("nbNodeDiscovered %d \n" , nbNodeDiscovered);
    int max = 0;
    int poids = 0;
    int indice = 0;
    for (int i = 0; i < compteur; ++i)
    {
        poids = pond[i];
        //printf(" %d ", poids);
        if (poids > max) {
            indice = i;
            max = poids;
        }

            
    }
   
    int lig = ligne[indice];
    int col = colonne[indice];
    pond[indice] = -1;

   // printf("valeur du plus gros %d en [%d %d] \n", max, col, lig);


    matCouv[lig][col] = 0;
    matCouv[col][lig] = 0;


}


int compterCoutTotal(int nbNode, int** matrice){
     int cout = 0;
            for (int i = 0; i < nbNode; ++i)
            {
                for (int j = i+1; j < nbNode; ++j)
                {
                    if (matrice[i][j] != -1 && matrice[i][j] != 0)
                        cout += matrice[i][j];
                }
            }
        return cout;
}




int main(int argc, char *argv[])
{
    FILE* fichier = NULL;
    int score[3] = {0};
    int nbNode,nbEdge,i,j,g,z,nbNodeDisc, nbNodeDiscovered = 0,flag,nDep,nPrem,nSec,pondMin = 0;
    int** matNode;
    int** matCouv;
    int* colonne;
    int* ligne;
    int* pond;
    //char graph[20] = argv[1];
    //printf("%s\n", &graph);
    fichier = fopen(argv[1], "r");

    if (argc==1) {
      printf("mauvais usage"); 
      return 0;  
    } 

    if (fichier != NULL)
    {
        // On positionne le curseur où se trouve le nombre de Noeuds et le récupère.
        fseek(fichier, 26, SEEK_SET);
        fscanf(fichier,"%d" , &nbNode);
        // On positionne le curseur où se trouve le nombre d'arrêtes et le récupère.
        fseek(fichier, 10, SEEK_CUR);
        fscanf(fichier,"%d" , &nbEdge);
        printf("Le graphe est compose de : %d noeud et %d arretes.\n\n", nbNode, nbEdge);


        // Maintenant que l'on connaît le nombre de noeuds, on peut créer la matrice de taille nbNode*nbNode
        matNode=(int **) malloc(nbNode*sizeof(int*));
        // On crée aussi la matrice où l'on va stocker l'arbre couvrant.
        matCouv=(int **) malloc(nbNode*sizeof(int*));

        for (i=0; i<nbNode; i++){
            matNode[i]=(int *) malloc(nbNode*sizeof(int));
            matCouv[i]=(int *) malloc(nbNode*sizeof(int));
        }



        // Une fois la matrice créée, on l'initialise à '-1' pour identifier les noeuds qui ne sont pas liés entre eux par des arrêtes.
        for(i=0; i<nbNode; i++){
            for(j=0; j<=nbNode; j++){
                matNode[i][j] = -1;
                matCouv[i][j] = -1;
            }
        }
        //printf ("Contenu de la matrice : %d, %d, %d \n", matNode[0][3], matNode[3][1], matNode[4][4]);

        // Maintenant on va au fur et à mesure lire une ligne du fichier et rentrer les données dans la matrice pour chaque Arrête.
        // On renseigne les infos de la première arrête.
        fseek(fichier, 22, SEEK_CUR); // Position dans le fichier des infos sur la première arrête
        fscanf(fichier,"%d  %d  %d" , &score[0],&score[1], &score[2]);
        matNode[score[0]][score[1]] = score[2];
        matNode[score[1]][score[0]] = score[2];
        //printf ("Contenu de la matrice : %d, %d, %d \n\n", matNode[score[0]][score[1]], matNode[score[1]][score[0]], matNode[4][4]);

        fseek(fichier, 1, SEEK_CUR);
            fscanf(fichier,"%d  %d  %d" , &score[0],&score[1], &score[2]);
            matNode[score[0]][score[1]] = score[2];
            matNode[score[1]][score[0]] = score[2];
        // On boucle tant qu'on a des arrête
        for(i=1; i<nbEdge-1; i++){
            fseek(fichier, 1, SEEK_CUR);
            fscanf(fichier,"%d  %d  %d" , &score[0],&score[1], &score[2]);
            matNode[score[0]][score[1]] = score[2];
            matNode[score[1]][score[0]] = score[2];
        }
        
        
        //afficherMatrice(matNode, nbNode);

        
        fclose(fichier);


        
        colonne = malloc(nbEdge*sizeof(int));
        ligne = malloc(nbEdge*sizeof(int));
        pond = malloc(nbEdge*sizeof(int));
        int ponderation = 0;
        int compteur = 0;
        for (int i = 0; i < nbNode; ++i)
        {
            for (int j = 0; j < nbNode; ++j)
            {
                ponderation = matNode[i][j];
                if (ponderation != -1) {
                    pond[compteur] = ponderation;
                    ligne[compteur] = j; 
                    colonne[compteur] = i;
                    
                    compteur++;
                    
                    matCouv[i][j] = ponderation;
                    matCouv[j][i] = ponderation;
                    matNode[j][i] = -1;
                    matNode[i][j] = -1;
                    
                    

                    int dejaDecouvert = estDedans(j, colonne, compteur);
                     
                     if (dejaDecouvert == 1)
                        supprimer(colonne, ligne, pond, matCouv, compteur, nbNode, matCouv);
                     else 
                        break;   //ligne suivante
                    
                    

                
                   
                }
                
            }
            
        }

        int cout = compterCoutTotal(nbNode, matCouv);
        printf("cout : %d\n", cout);
        //afficherMatrice(matCouv, nbNode);



    }
    else
        printf("Ouverture du fichier failed");

    return 0;
}
