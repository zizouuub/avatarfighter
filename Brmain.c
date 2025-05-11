
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "interface.h"
#include "type.h"
#include "lecture.h"
#include "effets.h"
#include "joueurVSordi.h"
#include "combat.h"

int main(){
    srand(time(NULL));
    // Lancement du menu d'accueil
    debutJeu();

    // Déclaration du tableau de combattants
    Combattant tab_combattants[MAX_PERSOS];

    // Chargement des combattants depuis le fichier
    int nb_combattants = charger_combattants("fichier.txt", tab_combattants, MAX_PERSOS);

    if (nb_combattants <= 0) {
        printf(ROUGE "Erreur: Aucun combattant chargé.\n" RESET);
        return 1;
    }

    //déclaration des équipes 
    Combattant equipe_joueur[3];  // une équipe de 3 combattants JOUEUR
    Combattant equipe_ordi[3]; //une équipe de 3 combattants ORDI

    Combattant equipe_J1[3]; //une équipe de 3 combattants JOUEUR 1
    Combattant equipe_J2[3]; //une équipe de 3 combattants JOUEUR 2

    
    // Affichage du menu principal
    int choix = choixPrincipal();  // Premier appel
    if (choix == 1) {
        // Sélection du mode de jeu
        int mode = choixModeJeu();

        // Sélection des combattants
        printf("Vous allez maintenant choisir vos équipes de" ROUGE" 3 combattants"RESET" !\n");
        printf("(Attention à bien remonter/descendre pour voir tous les combattants et vos équipes !)\n");
        sleep(4);
        clearScreen();
        if (mode == 1) {
            // Mode joueur contre ordinateur

            //sélection joueur
            selectionnerEquipe(tab_combattants, &nb_combattants, equipe_joueur, 3);

            //sélection ordi 
            choisirEquipeAleatoire(tab_combattants, nb_combattants, equipe_ordi, 3);

            //effacer écran
            clearScreen();
            printf("\n\n\n");
            //affichage équipe joueur
            printf("\n\n      ======== " JAUNE"Votre équipe"RESET": ========\n");
            afficherEquipe(equipe_joueur, 3);

            //affichage équipe ordi
            printf("\n\n      ======= " JAUNE"Equipe de l'ordinateur"RESET": =======\n");
            afficherEquipe(equipe_ordi, 3);
        } else {
            // Mode joueur contre joueur

            //sélection des deux équipes
            selectionnerEquipesJoueurs(tab_combattants, &nb_combattants, equipe_J1, equipe_J2, 3);

            //effacer écran
            clearScreen();
            printf("\n\n\n");
            //affichage du joueur 1
            printf("\n\n      =======" JAUNE"Equipe Joueur 1"RESET": =======\n");
            afficherEquipe(equipe_J1, 3);  

            //affichage du joueur 2
            printf("\n\n      ======= " JAUNE"Equipe Joueur 2"RESET": =======\n"); 
            afficherEquipe(equipe_J2, 3);

            
        }

        //affichage jeu commence
        jeuCommence();

        //lancement du combat
        if (mode == 1){
            lancer_combat(equipe_joueur, 3, equipe_ordi, 3);
        }else if (mode == 2){
            boucle_de_combat(equipe_J1, 3, equipe_J2, 3);
        }

        
    }
    //quitter le jeux 
    finJeu();
    exit(0);
    
    return 0;
}
