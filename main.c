#include "interface.h"

int main(){
  srand(time(NULL)); 

if(afficheMenuPrincipal()==1){
  int mode;
  printf(ROUGE"Je souhaite:\n"RESET);
  printf( "1. Jouer contre l'ordi\n" );
  printf( "2. Jouer contre Joueur 2\n" );
  do {
    printf("Choisissez une option (1 ou 2):\n");
      printf(">> ");
      scanf("%d", &mode);
  } while (mode != 1 && mode != 2);
  sleep(1);

    Combattant combattants[8] = {
      {"Zuko 🔥",     "Maître du Feu"},
      {"Azula 🔥",    "Danseuse de Feu"},
      {"Katara 🌊",   "Maîtresse de l'Eau"},
      {"Sokka 🌊",    "Guerrier des Glaces"},
      {"Toph 🌍",     "Maître de la Terre"},
      {"Kyoshi 🌍",   "La Lame de la Terre"},
      {"Aang 🌪",     "Maître de l'Air"},
      {"Yangchen 🌪", "Le Souffle des Mondes"}
    };

    for (int i = 0; i < 8; i++) {
      combattants[i].pv = rand() % 21 + 80;     // 80 à 100
      combattants[i].attaque = rand() % 51 + 50;    // 50 à 100
      combattants[i].defense = rand() % 31;         // 0 à 30

      }

        Combattant equipeJ1[3];
        Combattant equipeJ2[3];
        int taille_dispo = 8;

  void afficherListeCombattants(Combattant* tab, int taille);
  void selectionnerEquipe(Combattant* disponibles, int* taille_dispo, Combattant* equipe, int taille_equipe);
  void afficherEquipe(Combattant* equipe, int taille_equipe);
  void choisirEquipeAleatoire(Combattant* disponibles, int taille_dispo, Combattant* equipe, int taille_equipe);
  void selectionnerEquipesJoueurs(Combattant* disponibles, int* taille_dispo, Combattant* equipeJ1, Combattant* equipeJ2, int taille_equipe);
  

      if(mode ==1){
        printf("Vous avez choisi de jouer contre" VERT" l'ordinateur!\n"RESET );
        sleep(2);
        printf("\033[H\033[2J");

          selectionnerEquipe(combattants, &taille_dispo, equipeJ1, 3);
        printf("\033[H\033[2J");
          choisirEquipeAleatoire(combattants, taille_dispo, equipeJ2, 3);
          printf("\033[H\033[2J");
        printf(JAUNE"Vous:\n"RESET);
          afficherEquipe(equipeJ1, 3);
          printf("\n");
        printf(JAUNE"L'ordi:\n"RESET);
          afficherEquipe(equipeJ2, 3);
          printf(ROUGE"\nVos équipes sont prêtes à combattre!\n"RESET);
      }else if(mode == 2){
        printf("Vous avez choisi de jouer contre" VERT" un autre joueur!\n"RESET );
        sleep(2);
        printf("\033[H\033[2J");
        selectionnerEquipesJoueurs(combattants, &taille_dispo, equipeJ1, equipeJ2, 3);
        printf("\033[H\033[2J");
        printf(JAUNE"Joueur 1:\n"RESET);
          afficherEquipe(equipeJ1, 3);
          printf("\n");
        printf(JAUNE"Joueur 2:\n"RESET);
          afficherEquipe(equipeJ2, 3);
        sleep(2);
        printf("\033[H\033[2J");
          printf(ROUGE"\nLes équipes sont prêtes à combattre!\n"RESET);
      }
  

      sleep(3);
      printf("\033[H\033[2J");
      printf("   ===  ");
      printf(ROUGE "Le jeu va commencer🎮! " RESET);
      printf("  ===\n");

    }

  return 0;
}
