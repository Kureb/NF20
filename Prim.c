#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE* fichier = NULL;
    int score[3] = {0};
    int nbNode,coutMin,nbEdge,i,j,g,z,nbNodeDisc,flag,nDep,nPrem,nSec,pondMin, Fin, nbNodeParc = 0;
    int** matNode;
    int** matCouv;
    int* nodCouv;
    int* tab1;
    int* tab2;


    fichier = fopen("inst_v900.dat", "r");

    if (fichier != NULL)
    {
        // On positionne le curseur o� se trouve le nombre de Noeuds et le r�cup�re.
        fseek(fichier, 26, SEEK_SET);
        fscanf(fichier,"%d" , &nbNode);
        // On positionne le curseur o� se trouve le nombre d'arr�tes et le r�cup�re.
        fseek(fichier, 10, SEEK_CUR);
        fscanf(fichier,"%d" , &nbEdge);
        printf("Le graphe est compose de : %d noeud et %d arretes.\n\n", nbNode, nbEdge);


        // Maintenant que l'on conna�t le nombre de noeuds, on peut cr�er la matrice de taille nbNode*nbNode
        matNode=(int **) malloc(nbNode*sizeof(int*));
        // On cr�e aussi la matrice o� l'on va stocker l'arbre couvrant.
        matCouv=(int **) malloc(nbNode*sizeof(int*));

        for (i=0; i<nbNode; i++){
            matNode[i]=(int *) malloc(nbNode*sizeof(int));
            matCouv[i]=(int *) malloc(nbNode*sizeof(int));
        }



        // Une fois la matrice cr��e, on l'initialise � '-1' pour identifier les noeuds qui ne sont pas li�s entre eux par des arr�tes.
        for(i=0; i<nbNode; i++){
            for(j=0; j<=nbNode; j++){
                matNode[i][j] = -1;
                matCouv[i][j] = -1;
            }
        }
        printf ("Contenu de la matrice : %d, %d, %d \n", matNode[0][3], matNode[3][1], matNode[4][4]);

        // Maintenant on va au fur et � mesure lire une ligne du fichier et rentrer les donn�es dans la matrice pour chaque Arr�te.
        // On renseigne les infos de la premi�re arr�te.
        fseek(fichier, 22, SEEK_CUR); // Position dans le fichier des infos sur la premi�re arr�te
        fscanf(fichier,"%d  %d  %d" , &score[0],&score[1], &score[2]);
        matNode[score[0]][score[1]] = score[2];
        matNode[score[1]][score[0]] = score[2];
        printf ("Contenu de la matrice : %d, %d, %d \n\n", matNode[score[0]][score[1]], matNode[score[1]][score[0]], matNode[4][4]);

        fseek(fichier, 1, SEEK_CUR);
            fscanf(fichier,"%d  %d  %d" , &score[0],&score[1], &score[2]);
            matNode[score[0]][score[1]] = score[2];
            matNode[score[1]][score[0]] = score[2];
        // On boucle tant qu'on a des arr�te
        for(i=1; i<nbEdge-1; i++){
            fseek(fichier, 1, SEEK_CUR);
            fscanf(fichier,"%d  %d  %d" , &score[0],&score[1], &score[2]);
            matNode[score[0]][score[1]] = score[2];
            matNode[score[1]][score[0]] = score[2];
        }
        /*for(i=0; i<nbNode;i++){
            printf("[");
            for(j=0; j<nbNode;j++)
                printf(" %d ", matNode[i][j]);
            printf("]\n");
        }*/

        fclose(fichier);

        //VERIFICATION DE CONNEXITE DU GRAPHE.

        tab1=(int) malloc(nbNode*sizeof(int));
        tab2=(int) malloc(nbNode*sizeof(int));

        for(i=0; i<nbNode; i++)
            tab1[i] = 0;

        tab1[0] = -1;
        tab2[0] = 0;
        i=0;
        // On boucle tant qu'on arrive � lier des noeuds entre eux.
        while( Fin != 1){

            nDep = tab2[i];
            for(j=0; j<nbNode; j++){

                if(matNode[nDep][j] != -1){ // On a trouv� un noeud avec lequel nDep est li�
                    if(tab1[j] != -1){ // Le noeud avec lequel nDep est li� n'a pas encore �t� reli� avec un noeud d�j� parcouru.
                        nbNodeParc = nbNodeParc + 1;
                        tab2[nbNodeParc] = j;
                        tab1[j] = -1;
                    }
                }

            }
            // Si c'est plus petit, on a encore des noeuds pour lesquels on doit v�rifier la connexit�.
            if(i < nbNodeParc)
                i = i++;

            // Si c'est �gale, on a parcourut tout les noeuds reli�s entre eux, on arr�te de boucler
            if (i == nbNodeParc)
                Fin = 1;

        }

        // Si le nombre de noeuds reli�s n'est pas �gale au nombre de noeuds du graphe, le graphe n'est pas connexe.
        if (nbNodeParc != (nbNode -1)){
            printf("\n\nLe Graphe n'est pas connexe, l'algorithme n'est pas applicable\n\n");
            return 0;
        }
        else{

        printf("Veuillez renseigner le Noeud de depart de l'algorithme de Prim : ");
        scanf("%d",&nDep);
        // on initialise le tableau qui servira � conna�tre les noeuds d�j� pris en compte dans l'arbre couvrant.
        nodCouv = malloc(nbNode*sizeof(int));
        // Parcourt de la matrice pour trouver l'arr�te reli� au noeud de d�part ayant la plus petite pond�ration.
        pondMin = 9999999999999;
        // On v�rifie � quels sommets notre sommet est reli� et avec quelle pond�ration.
        for(i=0; i<nbNode; i++)
        {
            if((matNode[nDep][i] < pondMin) && (matNode[nDep][i] > 0) )
            {
                pondMin = matNode[nDep][i];
                nSec = i; //On prend le noeud avec lequel est reli� nodCouv avec la pond�ration minimale.
                //printf("Valeur de pondMin : %d \n", pondMin);
            }
        }

        // Ici, on conna�t l'arr�te et les noeuds associ�s avec la pond�ration minimale, on peut les rentrer dans la matriceCouv et indiquer dans la matriceNod que cette arr�te n'est plus � prendre en compte (-2)
        matNode[nDep][nSec] = -2;
        matNode [nSec][nDep] = -2;
        matCouv [nDep] [nSec] = pondMin;
        matCouv [nSec] [nDep] = pondMin;
        coutMin = pondMin;

        nodCouv [0] = nDep;
        nodCouv [1] = nSec;

        //Nombre de sommet acutellement d�couverts.
        nbNodeDisc = 2;
        //On parcourt tout les noeuds d�j� visit� en regardant leurs pond�ration et garde la plus petite.
        for(g=2; g<nbNode; g++){
            do{
                flag = 1;
                pondMin = 9999999999999;
                for(i=0;i<nbNodeDisc;i++){
                    nDep = nodCouv[i];
                    for(j=0; j<nbNode; j++){
                        if((matNode[nDep][j] < pondMin) && (matNode[nDep][j] > 0) ){
                            pondMin = matNode[nDep][j];
                            nSec = j; //On prend le noeud avec lequel est reli� nodCouv avec la pond�ration minimale.
                            nPrem = nDep;
                        }
                    }
                }
                // On v�rifie si on cr�e une boucle ou non. Si Oui, on ne prend pas en compte le noeud, on l'indique et continue � chercher.
                for(z=0; z<nbNodeDisc; z++){ // Pire des cas nbNideDisc = (nbNode - 1).
                    if(nSec == nodCouv[z])
                        flag = 0;
            }
                matNode[nPrem][nSec] = -2;
                matNode [nSec][nPrem] = -2;
            }while(flag == 0); // Pire des cas, on boucle (nbNode-1) fois.

            //nSec n'est pas d�j� pr�sent dans l'arbre couvrant, on renseigne donc la valeur trouv�e et tag le noeud.
            matCouv [nPrem] [nSec] = pondMin;
            matCouv [nSec] [nPrem] = pondMin;
            coutMin = coutMin + pondMin;

            nbNodeDisc= nbNodeDisc + 1;

            nodCouv[i] = nSec;
        }

        /*for(i=0; i<nbNode;i++){
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
        for(g=0; g<nbNodeDisc; g++){
                   printf("[ %d ,", nodCouv[g]);
                   printf("]");
        }*/
        }
        printf("\n\n\n\nLe cout minimal de l'arbre associe est : %d", coutMin);
    }
    else
        printf("Ouverture du fichier failed");


    printf("\n\nHello world!\n");
    return 0;
}
