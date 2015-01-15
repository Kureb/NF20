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
        // On positionne le curseur où se trouve le nombre de Noeuds et le récupère.
        fseek(fichier, 26, SEEK_SET);
        fscanf(fichier,"%d" , &nbNode);
        // On positionne le curseur où se trouve le nombre d'arrêtes et le récupère.
        fseek(fichier, 10, SEEK_CUR);
        fscanf(fichier,"%d" , &nbEdge);
        printf("Le graphe est composé de : %d noeud et %d arretes.\n\n", nbNode, nbEdge);


        // Maintenant que l'on connaît le nombre d'arrêtes, on peut créer les tableaux de taille nbEdge qui vont contenir respectivement les noeuds et les pondérations.
        tabNode1=(int *) malloc(nbEdge*sizeof(int));
        tabNode2=(int *) malloc(nbEdge*sizeof(int));
        tabPonde=(int *) malloc(nbEdge*sizeof(int));

        // On peut aussi créer nos tableaux de longueur nbNode qui vont nous servir à repérer les Boucles.
        tab1 = (int *) malloc(nbNode*sizeof(int));
        tab2 = (int *) malloc(nbNode*sizeof(int));

        // On crée aussi la matrice où l'on va stocker l'arbre couvrant.
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

        // Maintenant on va au fur et à mesure lire une ligne du fichier et rentrer les données dans les tableaux pour chaque Arrête, une ligne étant une arrête.
        // On renseigne les infos de la première arrête.
        fseek(fichier, 22, SEEK_CUR); // Position dans le fichier des infos sur la première arrête
        fscanf(fichier,"%d  %d  %d" , &score[0],&score[1], &score[2]);
        tabNode1[0] = score[0];
        tabNode2[0] = score[1];
        tabPonde[0] = score[2];
        matNode[score[0]][score[1]] = score[2];
        matNode[score[1]][score[0]] = score[2];

        // On boucle tant qu'on a des arrête
        for(i=1; i<nbEdge; i++){
            fseek(fichier, 1, SEEK_CUR);
            fscanf(fichier,"%d  %d  %d" , &score[0],&score[1], &score[2]);
            tabNode1[i] = score[0];
            tabNode2[i] = score[1];
            tabPonde[i] = score[2];
            matNode[score[0]][score[1]] = score[2];
            matNode[score[1]][score[0]] = score[2];
        }


        // Affichage des tableau créés.
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


        // On boucle tant qu'on arrive à lier des noeuds entre eux.
        while( Fin != 1){


            nDep = tab2[i];
            for(j=0; j<nbNode; j++){

                if(matNode[nDep][j] != -1){ // On a trouvé un noeud avec lequel nDep est lié
                    if(tab1[j] != -1){ // Le noeud avec lequel nDep est lié n'a pas encore été relié avec un noeud déjà parcouru.
                        nbNodeParc = nbNodeParc + 1;
                        tab2[nbNodeParc] = j;
                        tab1[j] = -1;
                    }
                }

            }
            // Si c'est plus petit, on a encore des noeuds pour lesquels on doit vérifier la connexité.
            if(i < nbNodeParc)
                i = i++;

            // Si c'est égale, on a parcourut tout les noeuds reliés entre eux, on arrête de boucler
            if (i == nbNodeParc)
                Fin = 1;

        }

        // Si le nombre de noeuds reliés n'est pas égale au nombre de noeuds du graphe, le graphe n'est pas connexe.
        if (nbNodeParc != (nbNode -1)){
            printf("\n\nLe Graphe n'est pas connexe, l'algorithme n'est pas applicable\n\n");
            return 0;
        }
        else{

            //On initialise le tableau qui va nous servir à vérifier la formation de boucles
            for (i=0; i<nbNode; i++){
                tab1[i] = -1;
                tab2[i] = -2;
            }

            //Recherche de la plus grande pondération avant la création du tableau de pondération tries.
            pondMax = tabPonde [0];
            for(i=1; i<nbEdge;i++){
                    if(tabPonde[i] > pondMax)
                        pondMax = tabPonde[i]+1;
            }
            tabPondeTri = (int *) malloc(pondMax*sizeof(int));

            // On initialise le tableau nouvellement créé de 0
            for(i=0; i<pondMax; i++)
                tabPondeTri[i] = 0;

            //On parcourt le tableau de pondération et incrémente la case à l'indice corespondant si pondération existe.
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

            // On à notre arbre couvrant quand on a (nbNode - 1) arrêtes -> Arrête de bucler

            nodDes = 0;
            nbEdgeTrouv = 0;
            couvMin = 0;
            while (nbEdgeTrouv != (nbNode-1)){

            // Recherche de la plus petite pondération
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

            // Recherche de l'indice où est situé la plus petite pondération.
            indPondMin = 0;
            while (trouve != 1){
                if(tabPonde[indPondMin] == pondMin)
                    trouve = 1;
                else
                   indPondMin = indPondMin +1;
            }

            nod1 = tabNode1[indPondMin];
            nod2 = tabNode2[indPondMin];

            // Recherche de l'ensemble des noeuds avec lesquels sont déjà liés nod1 et nod2
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

            // Si il y a un cycle on ne rentre pas la valeur mais on fait en sorte de ne plus prendre en compte cette arrête.
            if (cycle == 1){
                tabNode1[indPondMin] = -1;
                tabNode2[indPondMin] = -1;
                tabPonde[indPondMin] = -1;
            }
            else{

                //On stock la valeur retenue dans la matrice résultat aux indices des noeuds corespondants.
                matCouv[nod1][nod2] = pondMin;
                matCouv[nod2][nod1] = pondMin;
                couvMin = couvMin + pondMin;

                //Pour ne pas reconcidérer cet arc, on met +l'infini aux indices déjà parcouru.
                tabNode1[indPondMin] = -1;
                tabNode2[indPondMin] = -1;
                tabPonde[indPondMin] = -1;

                nbEdgeTrouv = nbEdgeTrouv +1;

            }

             //Avant de vérifier une autre arrête, on réinitialise les tableaux de comparaison.
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
            printf("\n\nLa couverture minimal du graphe donné en argument est : %d \n\n", couvMin);
            return 0;
        }}
    else
        printf("\nLecture du fichier Impossible ! \n\n ");

    return 0;
}
