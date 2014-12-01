#include <stdio.h>
#include <stdlib.h>

int main()
{

    FILE* fichier = NULL;
    int score[3] = {0};
    int nbNode,nbEdge,i,j,nDep,nSec,pondMin = 0;
    int** matNode;
    int** matCouv;
    int* nodCouv;
    fichier = fopen("test.txt", "r");

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
        printf ("Contenu de la matrice : %d, %d, %d \n", matNode[0][3], matNode[3][1], matNode[4][4]);

        // Maintenant on va au fur et à mesure lire une ligne du fichier et rentrer les données dans la matrice pour chaque Arrête.
        // On renseigne les infos de la première arrête.
        fseek(fichier, 22, SEEK_CUR); // Position dans le fichier des infos sur la première arrête
        fscanf(fichier,"%d  %d  %d" , &score[0],&score[1], &score[2]);
        matNode[score[0]][score[1]] = score[2];
        matNode[score[1]][score[0]] = score[2];
        printf ("Contenu de la matrice : %d, %d, %d \n\n", matNode[score[0]][score[1]], matNode[score[1]][score[0]], matNode[4][4]);

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
        for(i=0; i<nbNode;i++){
            printf("[");
            for(j=0; j<nbNode;j++)
                printf(" %d ", matNode[i][j]);
            printf("]\n");
        }

        fclose(fichier);

        printf("Veuillez renseigner le Noeud de depart de l'algorithme de Prim : ");
        scanf("%d",&nDep);
        // on initialise le tableau qui servira à connaître les noeuds déjà pris en compte dans l'arbre couvrant.
        nodCouv = malloc(nbNode*sizeof(int));
        // Parcourt de la matrice pour trouver l'arrête relié au noeud de départ ayant la plus petite pondération.
        pondMin = 9999999999999;

        for(i=0; i<nbNode; i++){
            if((matNode[nDep][i] < pondMin) && (matNode[nDep][i] > 0) ){
                pondMin = matNode[nDep][i];
                nSec = i; //On prend le noeud avec lequel est relié nodCouv avec la pondération minimale.
                printf("Valeur de pondMin : %d \n", pondMin);
            }
        }

        // Ici, on connaît l'arrête et les noeuds associés avec la pondération minimale, on peut les rentrer dans la matriceCouv et indiquer dans la matriceNod que cette arrête n'est plus à prendre en compte (-2)
        matNode[nDep][nSec] = -2;
        matNode [nSec][nDep] = -2;
        matCouv [nDep] [nSec] = pondMin;
        matCouv [nSec] [nDep] = pondMin;
        nodCouv [0] = nDep;
        nodCouv [1] = nSec;

        for(i=0; i<nbNode;i++){
            printf("[");
            for(j=0; j<nbNode;j++)
                printf(" %d ", matNode[i][j]);
            printf("]\n");
        }
          printf("\n\n");
        for(i=0; i<nbNode;i++){
            printf("[");
            for(j=0; j<nbNode;j++)
                printf(" %d ", matCouv[i][j]);
            printf("]\n");
        }


    }
    else
        printf("Ouverture du fichier failed");


    printf("Hello world!\n");
    return 0;
}
