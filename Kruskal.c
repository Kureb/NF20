#include <stdio.h>
#include <stdlib.h>

int main()
{

    FILE* fichier = NULL;
    int score[3] = {0};
    int nbNode,nbEdge,i,j,pondMax,pondActuel,pondMin,indPondMin, nodDes,trouve,nod1DejTrouv,nod2DejTrouv,nbNodeParc, nod1, nod2, nbEdgeTrouv, nDep, cycle, n, m, couvMin, Fin = 0;
    int* tabPonde;
    int* tabPondeTri;
    int* tabNode1;
    int* tabNode2;
    int* tab1;
    int* tab2;
    int** matNode;
    int** matCouv;
    fichier = fopen("Test.txt", "r");

    if (fichier != NULL)
    {
        // On positionne le curseur o� se trouve le nombre de Noeuds et le r�cup�re.
        fseek(fichier, 26, SEEK_SET);
        fscanf(fichier,"%d" , &nbNode);
        // On positionne le curseur o� se trouve le nombre d'arr�tes et le r�cup�re.
        fseek(fichier, 10, SEEK_CUR);
        fscanf(fichier,"%d" , &nbEdge);
        printf("Le graphe est compos� de : %d noeud et %d arretes.\n\n", nbNode, nbEdge);


        // Maintenant que l'on conna�t le nombre d'arr�tes, on peut cr�er les tableaux de taille nbEdge qui vont contenir respectivement les noeuds et les pond�rations.
        tabNode1=(int *) malloc(nbEdge*sizeof(int));
        tabNode2=(int *) malloc(nbEdge*sizeof(int));
        tabPonde=(int *) malloc(nbEdge*sizeof(int));

        // On peut aussi cr�er nos tableaux de longueur nbNode qui vont nous servir � rep�rer les Boucles.
        tab1 = (int *) malloc(nbNode*sizeof(int));
        tab2 = (int *) malloc(nbNode*sizeof(int));

        // On cr�e aussi la matrice o� l'on va stocker l'arbre couvrant.
        matCouv=(int **) malloc(nbNode*sizeof(int*));
        matNode=(int **) malloc(nbNode*sizeof(int*));


        for (i=0; i<nbNode; i++){
            matCouv[i]=(int *) malloc(nbNode*sizeof(int));
            matNode[i]=(int *) malloc(nbNode*sizeof(int));
            }

         for(i=0; i<nbNode; i++){
            for(j=0; j<=nbNode; j++){
                matCouv[i][j] = -1;
                matNode[i][j] = -1;
            }
        }

        // Maintenant on va au fur et � mesure lire une ligne du fichier et rentrer les donn�es dans les tableaux pour chaque Arr�te, une ligne �tant une arr�te.
        // On renseigne les infos de la premi�re arr�te.
        fseek(fichier, 22, SEEK_CUR); // Position dans le fichier des infos sur la premi�re arr�te
        fscanf(fichier,"%d  %d  %d" , &score[0],&score[1], &score[2]);
        tabNode1[0] = score[0];
        tabNode2[0] = score[1];
        tabPonde[0] = score[2];
        matNode[score[0]][score[1]] = score[2];
        matNode[score[1]][score[0]] = score[2];

        // On boucle tant qu'on a des arr�te
        for(i=1; i<nbEdge; i++){
            fseek(fichier, 1, SEEK_CUR);
            fscanf(fichier,"%d  %d  %d" , &score[0],&score[1], &score[2]);
            tabNode1[i] = score[0];
            tabNode2[i] = score[1];
            tabPonde[i] = score[2];
            matNode[score[0]][score[1]] = score[2];
            matNode[score[1]][score[0]] = score[2];
        }


        // Affichage des tableau cr��s.
        printf("[");
        for(i=0; i<nbEdge;i++){
                printf(" %d ", tabNode1[i]);
        }
        printf("]\n\n\n");


        printf("[");
        for(i=0; i<nbEdge;i++){
                printf(" %d ", tabNode2[i]);
        }
        printf("]\n\n\n");


        printf("[");
        for(i=0; i<nbEdge;i++){
                printf(" %d ", tabPonde[i]);
        }
        printf("]\n\n\n");

        fclose(fichier);

        for(i=0; i<nbNode;i++){
            printf("[");
            for(j=0; j<nbNode;j++){
                printf(" %d ", matNode[i][j]);
            }
            printf("]\n");
        }

        // TEST DE CONNEXITEE.

        for(i=0; i<nbNode; i++)
            tab1[i] = 0;

        tab1[0] = -1;
        tab2[0] = 0;
        i=0;
        nbNodeParc = 0;


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

            //On initialise le tableau qui va nous servir � v�rifier la formation de boucles
            for (i=0; i<nbNode; i++){
                tab1[i] = -1;
                tab2[i] = -2;
            }

            //Recherche de la plus grande pond�ration avant la cr�ation du tableau de pond�ration tries.
            pondMax = tabPonde [0];
            for(i=1; i<nbEdge;i++){
                    if(tabPonde[i] > pondMax)
                        pondMax = tabPonde[i]+1;
            }
            tabPondeTri = (int *) malloc(pondMax*sizeof(int));

            // On initialise le tableau nouvellement cr�� de 0
            for(i=0; i<pondMax; i++)
                tabPondeTri[i] = 0;

            //On parcourt le tableau de pond�ration et incr�mente la case � l'indice corespondant si pond�ration existe.
            for(i=0; i<nbEdge;i++){
                pondActuel = tabPonde[i];
                tabPondeTri[pondActuel] = tabPondeTri[pondActuel] + 1;
            }

/*
            printf("[");
            for(i=0; i<pondMax;i++){
                    printf(" %d ", tabPondeTri[i]);
            }
            printf("]\n\n\n");
*/

            // LANCEMENT ALGORITHME DE KRUSKAL

            // On � notre arbre couvrant quand on a (nbNode - 1) arr�tes -> Arr�te de bucler

            nodDes = 0;
            nbEdgeTrouv = 0;
            couvMin = 0;
            while (nbEdgeTrouv != (nbNode-1)){

            // Recherche de la plus petite pond�ration
            pondMin = 0;
            i = 0;
            trouve = 0;
            while(pondMin == 0)
            {
                if (tabPondeTri[i] != 0)
                {
                    pondMin = i;
                    tabPondeTri[i] = tabPondeTri[i]-1;
                }
                else
                    i = i+1;
            }

           // printf("\n\nPonderation minimal : %d \n\n\n", pondMin);

            // Recherche de l'indice o� est situ� la plus petite pond�ration.
            indPondMin = 0;
            while (trouve != 1){
                if(tabPonde[indPondMin] == pondMin)
                    trouve = 1;
                else
                   indPondMin = indPondMin +1;
            }

            nod1 = tabNode1[indPondMin];
            nod2 = tabNode2[indPondMin];

            // Recherche de l'ensemble des noeuds avec lesquels sont d�j� li�s nod1 et nod2
            for(n=0; n<nbNode; n++){
                if( matCouv[nod1][n] != -1)
                    tab1[n] = matCouv[nod1][n];
                if( matCouv[nod2][n] != -1)
                    tab2[n] = matCouv[nod2][n];
            }

            // Si nod1 et nod2 ont un noeud en commun, c'est qu'il y aura formation de cycle
            n=0;
            m=0;
            while(tab1[n] != -1){
                while(tab2[m] != -2){
                    if(tab1[n] == tab2[m])
                        cycle = 1;
                    m = m+1;
                }
                n = n+1;
            }

            // Si il y a un cycle on ne rentre pas la valeur mais on fait en sorte de ne plus prendre en compte cette arr�te.
            if (cycle == 1){
                tabNode1[indPondMin] = -1;
                tabNode2[indPondMin] = -1;
                tabPonde[indPondMin] = -1;
            }
            else{

                //On stock la valeur retenue dans la matrice r�sultat aux indices des noeuds corespondants.
                matCouv[nod1][nod2] = pondMin;
                matCouv[nod2][nod1] = pondMin;
                couvMin = couvMin + pondMin;

                //Pour ne pas reconcid�rer cet arc, on met +l'infini aux indices d�j� parcouru.
                tabNode1[indPondMin] = -1;
                tabNode2[indPondMin] = -1;
                tabPonde[indPondMin] = -1;

                nbEdgeTrouv = nbEdgeTrouv +1;

            }

             //Avant de v�rifier une autre arr�te, on r�initialise les tableaux de comparaison.
            for (i=0; i<nbNode; i++){
                tab1[i] = -1;
                tab2[i] = -2;
            }
        }
            printf("Hello world!\n\n");
/*
             for(i=0; i<nbNode;i++){
                printf("[");
                for(j=0; j<nbNode;j++)
                    printf(" %d ", matCouv[i][j]);
                printf("]\n");
             }
*/
            printf("\n\nLa couverture minimal du graphe donn� en argument est : %d \n\n", couvMin);
            return 0;
        }}
    else
        printf("\nLecture du fichier Impossible ! \n\n ");

    return 0;
}
