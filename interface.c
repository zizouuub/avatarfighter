#include <stdio.h>
#include "interface.h"


int afficheMenuPrincipal(){
    int a;
    
    printf("==================================\n");
    printf("   ===  ");
    printf(ROUGE "CY-FIGHTERS 2025" RESET);
    printf("  ===\n");
    printf("==================================\n");
    sleep(2); // Pause de 2 secondes avant disparition de l'écran
    printf("\033[H\033[2J"); // Pour effacer l'écran
   
    printf(ROUGE"Je souhaite:\n"RESET);
    printf( "1. Jouer une partie\n" );
    printf( "2. Quitter\n" );
    do {
        printf("Choisissez une option (1 ou 2):\n");
        printf(">> ");
        scanf("%d", &a);
    } while (a != 1 && a != 2);

    if (a == 1) {
            printf("Vous avez choisi de" BLEU" jouer une partie" RESET"🥳!\n" );         
        sleep(1);
        printf("\033[H\033[2J");
        
    } else {
        printf("Vous avez choisi de" ROUGE" nous quitter" RESET"😤!\n" );
        sleep(1);
        printf("\033[H\033[2J");
        printf("==================================\n");
        printf("   ===  ");
        printf(ROUGE "A la Prochaine ! " RESET);
        printf("  ===\n");
        printf("==================================\n");
    }
    return a;
}

void choisirEquipeAleatoire(Combattant* disponibles, int taille_dispo, Combattant* equipe, int taille_equipe){
    int indice;
    for(int i=0; i<taille_equipe; i++){
        do{
            indice = rand() % taille_dispo;
        }while(disponibles[indice].pv == -1);
        equipe[i] = disponibles[indice];
        disponibles[indice].pv = -1;
        printf("L'ordi a choisi %s - %s\n", equipe[i].nom, equipe[i].titre);
        sleep(2);
    }
        printf("\033[H\033[2J");
}


void afficherListeCombattants(Combattant* tab, int taille) {
    printf("   ===  ");
    printf(JAUNE "Liste des Combattants" RESET);
    printf("  ===\n");
    printf("\n");
    
    for (int i = 0; i < taille; i++) {
        if (tab[i].pv == -1) {
            printf(NOIR "%d. %s - %s (déjà choisi)\n" RESET, i + 1, tab[i].nom, tab[i].titre);
            printf(NOIR "    VIE: -- | ATTAQUE: -- | DEFENSE: --\n\n" RESET);
        } else {
            printf("%d. %s - %s\n", i + 1, tab[i].nom, tab[i].titre);
            printf("    VIE:" JAUNE " %d" RESET " |  ATTAQUE:" JAUNE " %d" RESET " |  DEFENSE:" JAUNE " %d\n\n" RESET,
                   tab[i].pv, tab[i].attaque, tab[i].defense);
        }
    }
    printf("   =================================\n");
}



void selectionnerEquipe(Combattant* disponibles, int* taille_dispo, Combattant* equipe, int taille_equipe) {
    int choix;

    for (int i = 0; i <taille_equipe; i++) {

        afficherListeCombattants(disponibles, *taille_dispo);  

      do { 
          printf(CYAN"\nChoisissez votre combattant n°%d (1 à %d):\n"RESET, i+1, *taille_dispo);
          scanf("%d", &choix);
          if (disponibles[choix - 1].pv == -1) {

              printf(ROUGE "⚠️ Vous avez déjà choisi ce combattant ! Choisissez-en un autre.\n" RESET);
              choix = -1; 
          }

          else if(choix<1||choix>*taille_dispo){
              printf(ROUGE"⚠️ Vous devez choisir un nombre entre 1 et %d !\n"RESET, *taille_dispo);
          }

      } while (choix < 1 || choix > *taille_dispo);

      equipe[i] = disponibles[choix - 1];
    disponibles[choix - 1].pv = -1;  

          printf(">>"VERT" %s a été ajouté à votre équipe !🥊\n"RESET, equipe[i].nom);
    sleep(1);
    printf("\033[H\033[2J");
    }
}

void selectionnerEquipesJoueurs(Combattant* disponibles, int* taille_dispo, Combattant* equipeJ1, Combattant* equipeJ2, int taille_equipe){
    int choix;
    for (int i = 0; i < taille_equipe; i++) {
        // Joueur 1
        afficherListeCombattants(disponibles, *taille_dispo);
        do {
            printf(CYAN"Joueur 1, choisissez votre combattant n°%d (1 à %d):\n"RESET, i+1, *taille_dispo);
            scanf("%d", &choix);
            if (choix < 1 || choix > *taille_dispo || disponibles[choix-1].pv == -1)
                printf(ROUGE"⚠️ Choix invalide. Réessayez.\n"RESET);
        } while(choix < 1 || choix > *taille_dispo || disponibles[choix-1].pv == -1);
        equipeJ1[i] = disponibles[choix - 1];
        disponibles[choix - 1].pv = -1;
        printf(VERT">> %s a été ajouté à l'équipe du Joueur 1!🥊\n"RESET, equipeJ1[i].nom);
        sleep(1);
        printf("\033[H\033[2J");

        // Joueur 2
        afficherListeCombattants(disponibles, *taille_dispo);
        do {
            printf(CYAN"Joueur 2, choisissez votre combattant n°%d (1 à %d):\n"RESET, i+1, *taille_dispo);
            scanf("%d", &choix);
            if (choix < 1 || choix > *taille_dispo || disponibles[choix-1].pv == -1)
                printf(ROUGE"⚠️ Choix invalide. Réessayez.\n"RESET);
        } while(choix < 1 || choix > *taille_dispo || disponibles[choix-1].pv == -1);
        equipeJ2[i] = disponibles[choix - 1];
        disponibles[choix - 1].pv = -1;
        printf(VERT">> %s a été ajouté à l'équipe du Joueur 2!🥊\n"RESET, equipeJ2[i].nom);
        sleep(1);
        printf("\033[H\033[2J");
    }
}


void afficherEquipe(Combattant* equipe, int taille_equipe){

    printf("   ===  ");
    printf(JAUNE "Equipe Selectionné" RESET);
    printf("  ===\n");
    printf("\n");
    for (int i = 0; i < taille_equipe; i++) {
        printf("%d. %s - %s\n", i + 1, equipe[i].nom, equipe[i].titre);
        printf("    VIE:" JAUNE" %d" RESET " |  ATTAQUE:" JAUNE" %d" RESET " |  DEFENSE:" JAUNE" %d\n\n" RESET,
                   equipe[i].pv, equipe[i].attaque, equipe[i].defense);
        }
    printf("   =================================\n");

  }

