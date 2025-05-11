#include <stdio.h>
#include "interface.h"
#include "type.h"

int main(){
    srand(time(NULL));
    // Lancement du menu d'accueil
    debutJeu();

    // Déclaration du tableau de combattants
    Combattant tab_combattants[MAX_PERSOS];

    // Chargement des combattants depuis le fichier
    int nb_combattants = chargerCombattants("fichier.txt", tab_combattants);
    if (nb_combattants <= 0) {
        printf(ROUGE "Erreur: Aucun combattant chargé.\n" RESET);
        return 1;
    }

    // Affichage du menu principal
    int choix = choixPrincipal();  // Premier appel
    if (choix == 1) {
        // Sélection du mode de jeu
        int mode = choixModeJeu();

        // Sélection des combattants
        printf("Vous allez mainentant choisir vos équipes de" ROUGE" 3 combattants"RESET" !\n");
        printf("(Attention à descendre pour voir tous les combattants !)\n");
        sleep(4);
        printf("\033[H\033[2J");
        
        if (mode == 1) {
            

            // Afficher la liste des combattants une seule fois ici
            afficheCombattants(tab_combattants, nb_combattants);

            Combattant equipe_joueur[3];  // par exemple, une équipe de 3 combattants
            selectionnerEquipe(tab_combattants, &nb_combattants, equipe_joueur, 3);

            //sélection ordi 
            Combattant equipe_ordi[3];
            choisirEquipeAleatoire(tab_combattants, nb_combattants, equipe_ordi, 3);

            //affichage équipe joueur
            printf("\033[H\033[2J");
            printf("   === " JAUNE"Votre équipe"RESET": ===\n");
            afficherEquipe(equipe_joueur, 3);

            //affichage équipe ordi
            printf("\n\n");
            printf("   === " JAUNE"Equipe de l'ordinateur"RESET": ===\n");
            afficherEquipe(equipe_ordi, 3);
        } else{

            // Mode joueur contre joueur
            Combattant equipe_J1[3];
            Combattant equipe_J2[3];
            selectionnerEquipesJoueurs(tab_combattants, &nb_combattants, equipe_J1, equipe_J2, 3);

            //affichage équipe joueur 1
            printf("\033[H\033[2J");
            printf("   === " JAUNE"Equipe Joueur 1"RESET": ===\n");
            afficherEquipe(equipe_J1, 3);

            //affichage équipe joueur 2
            printf("\n\n");
            printf("   === " JAUNE"Equipe Joueur 2"RESET": ===\n");
            afficherEquipe(equipe_J2, 3);
        }
    }
    return 0;
}
