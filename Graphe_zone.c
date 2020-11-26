#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Graphe_zone.h"
#include "Entete_Fonctions.h"
#include "Liste_case.h"

#define GRAND_NOMBRE 9999999

void ajoute_liste_sommet(Cellule_som **L, Sommet *s) {
  Cellule_som *temp = (Cellule_som *)malloc(sizeof(Cellule_som)); /* pointeur pour parcourir L */

  if(temp==NULL) { /* test le malloc */
    fprintf(stderr, "Erreur malloc");
    exit(EXIT_FAILURE);
  }
  temp -> sommet = s; /* Ajoute le sommet a la liste */
  temp -> suiv = *L; /* On attache le nouveau maillon temp a la chaine L */
  *L=temp;
}

void detruit_liste_sommet(Cellule_som **L) {
    Cellule_som *cour,*temp; /* Creer 2 pointeurs pour parcourir la liste chainee */
    cour=*L; /* On fait pointer cour sur le premier element de la chaine */
    while (cour!=NULL){ /* On avance dans la chaine et on supprimme chaque element de cette liste  */
      temp=cour;
      temp -> sommet = NULL;
      cour=cour->suiv;
      free(temp);
   }
   *L=NULL;
}


void ajoute_voisin(Sommet *s1, Sommet *s2) {
  ajoute_liste_sommet(&(s1->sommet_adj), s2);
  ajoute_liste_sommet(&(s2->sommet_adj), s1);
}


int adjacent(Sommet *s1, Sommet *s2) {
  Cellule_som *cour; /* Creer 2 pointeurs pour parcourir la liste chainee */
  cour=s1->sommet_adj; /* On fait pointer cour sur le premier element de la chaine */

  while (cour!=NULL){ /* On avance dans la chaine et on verifie si s2 se trouve dans la liste de sommet adjacent a s1 */
     if(cour->sommet==s2) { /* Si s2 s'y trouve on retourne vrai */
       return 1;
     }
     cour=cour->suiv;
  }
  return 0; /* Sinon on retourne faux */
}


Graphe_zone cree_graphe_zone(int **M, int dim) {
  int taille;
  int nbsommet=1;
  int i,j;
  Graphe_zone graphe;

  graphe.nbsom = 0;
  graphe.som = NULL;

  graphe.mat=(Sommet ***)malloc(sizeof(Sommet **) *dim);
  for(i=0; i<dim; i++) {
    (graphe.mat)[i]=(Sommet **)malloc(sizeof(Sommet *) * dim);
    for(j=0; j<dim; j++) {
      graphe.mat[i][j]=NULL;
    }
  }

  for(i=0; i<dim; i++) {
    for(j=0; j<dim; j++) {
      if(graphe.mat[i][j]==NULL) {

        taille = 0;

        Sommet *sommet=(Sommet *) malloc(sizeof(Sommet));
        sommet -> marque = 2;
        sommet -> distance = GRAND_NOMBRE;
        sommet -> pere = NULL;
        sommet -> num = nbsommet;
        sommet -> cl = M[i][j];
        sommet -> cases = NULL;
        sommet -> nbcase_som = 0;
        sommet -> sommet_adj = NULL;

        nbsommet++;

        ajoute_liste_sommet(&graphe.som, sommet);

        trouve_zone_rec(M, dim, i, j, &taille, &(sommet->cases));

        sommet -> nbcase_som = taille;

        while(sommet->cases) {
          graphe.mat[sommet->cases->i][sommet->cases->j] = sommet;
          sommet->cases = sommet->cases -> suiv;
        }
        graphe.nbsom++;
      }
    }
  }

  for(i=0; i<dim; i++) {
    for(j=0; j<dim; j++) {
      if(i<dim-1) {
        if(graphe.mat[i][j]!=graphe.mat[i+1][j] && !adjacent(graphe.mat[i][j], graphe.mat[i+1][j])) {
          ajoute_voisin(graphe.mat[i][j], graphe.mat[i+1][j]);
        }
      }

      if(j<dim-1) {
        if (graphe.mat[i][j]!=graphe.mat[i][j+1] && !adjacent(graphe.mat[i][j], graphe.mat[i][j+1])){
          ajoute_voisin(graphe.mat[i][j], graphe.mat[i][j+1]);
        }
      }
    }

  }
return graphe;
}


void affiche_graphe(Graphe_zone graphe) {

  Sommet *sommet;
  Cellule_som *elem = graphe.som;

  printf("graphe : %d sommets et zones\n", graphe.nbsom);

  while(elem!=NULL) {
    sommet = elem -> sommet;
    printf("Le sommet S%d est de couleur %d et a %d cases.\n", sommet->num, sommet->cl, sommet->sommet_adj);
    Cellule_som *L_som_adj = sommet->sommet_adj;
    while(L_som_adj!=NULL) {
      printf("Sommet S%d\n", L_som_adj->sommet->num);
      L_som_adj = L_som_adj->suiv;
    }
    printf("\n");
    elem = elem -> suiv;
  }

}
