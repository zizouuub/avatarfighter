#include "interface.h"
#include "type.h"
#include <stdlib.h> // Pour system()

#define NOM_MAX 500 

// Fonction portable pour effacer l'écran
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

//Pour sécuriser le choix entre les options 
int lireChoix(int min, int max) {
    int valeur;
    char entree[100]; // Buffer assez grand

    while (1) {
        if (fgets(entree, sizeof(entree), stdin) == NULL) {
            printf("Erreur de lecture. Réessayez.\n");
            continue;
        }

        if (sscanf(entree, "%d", &valeur) == 1) {
            if (valeur >= min && valeur <= max) {
                return valeur;
            }
        }

        printf("Veuillez entrer un nombre entre %d et %d : ", min, max);
    }
}


//convertir élement en emoji
const char* getEmoji(Element element) {
    switch (element) {
        case FEU:   return "🔥";
        case EAU:   return "🌊";
        case TERRE: return "🌍";
        case AIR:   return "🌪";
        default:    return "";
    }
}

//afficher fin de jeu
void finJeu(){
    clearScreen();
    printf("\n\n\n");
    printf("===================================\n");
    printf("  ===  ");
    printf(ROUGE "A la prochaine 👋 !" RESET);
    printf("  ===\n");
    printf("===================================\n");
    sleep(2);
    clearScreen();
}


//afficher titre 
void debutJeu() {
    clearScreen();
    printf("\n\n\n");
    printf("===================================\n");
    printf("  ===  ");
    printf(ROUGE "CY-AVATAR FIGHTERS 🤺" RESET);
    printf("  ===\n");
    printf("===================================\n");
    sleep(2);
    clearScreen();
}


// jouer ou quitter 
int choixPrincipal() {
    int choix;
    clearScreen();
    printf("\n\n\n");
    printf(ROUGE"Je souhaite:\n"RESET);
    printf("1. Jouer une partie\n");
    printf("2. Quitter\n");

    printf("Choisissez une option (1 ou 2):\n");
    printf(">> ");
    choix = lireEntierSimple(1, 2);

    clearScreen();
    return choix;
}



//contre ordi ou joueur 
int choixModeJeu() {
    int mode;
    clearScreen();
    printf("\n\n\n");
    printf(ROUGE"Je souhaite:\n"RESET);
    printf("1. Jouer contre Ordinateur\n");
    printf("2. Jouer contre Joueur 2\n");

    printf(">> ");
    mode = lireEntierSimple(1, 2);

    clearScreen();
    return mode;
}


//convertir élément en nom
const char* getEffetNom(EffetType effet) {
    switch (effet) {
        case ATTAQUE: return "ATTAQUE";
        case DEFENSE: return "DEFENSE";
        case AGILITE: return "AGILITE";
        case STUN: return "STUN";
        case GEL: return "GEL";
        case SOIN: return "SOIN";
        case BRULURE: return "BRULURE";
        case CONTRE: return "CONTRE";
        case POISON: return "POISON";
        default: return "AUCUN";
    }
}


//affiche les combattants
void afficheCombattants(Combattant* tab, int taille) {
    printf("   ==========================================\n");
    printf("   === " JAUNE "      Liste des Combattants      " RESET "  ===\n");
    printf("   ===========================================\n\n");

    for (int i = 0; i < taille; i++) {
        Combattant c = tab[i];

        if (c.pv == -1) {
            printf(" (%d)" NOIR" %s" RESET" %s ❌Déjà choisi\n", i + 1, c.nom, getEmoji(c.element));
        } else {
            printf(" (%d)" BEIGE" %s" RESET" %s\n", i + 1, c.nom, getEmoji(c.element));
        

        printf("\n     VIE: " JAUNE "%.1f" RESET " | ATTAQUE: " JAUNE "%d" RESET " | DEFENSE: " JAUNE "%d" RESET 
               " | AGILITE: " JAUNE "%d" RESET " | VITESSE: " JAUNE "%d" RESET "\n", 
               c.pv, c.attaque, c.defense, c.agilite, c.vitesse);

        printf(BEIGE"  \n  ⚔ Techniques spéciales:\n"RESET);
        for (int j = 0; j < MAX_TECHS; j++) {
            TechniqueSpeciale t = c.techniques[j];
            printf("       %d. " JAUNE "%s" RESET " | Portée: "JAUNE"%d"RESET" | Puissance:"JAUNE"%.1f"RESET" | Tours: "JAUNE"%d\n"RESET,
                   j + 1, t.nom, t.portee, t.puissance, t.tours);
        }
    }

        if(i < taille - 1) {
            printf("     ----------------------------------------\n\n");
        }
    }
    printf("   =========================================\n\n");
}


//choix des combattants pour le joueur
void selectionnerEquipe(Combattant* disponibles, int* taille_dispo, Combattant* equipe, int taille_equipe) {
    int choix;

    for (int i = 0; i < taille_equipe; i++) {
        clearScreen();
        afficheCombattants(disponibles, *taille_dispo);

        do {
            printf(BLANC"\nChoisissez votre combattant n°%d (1 à %d):\n"RESET, i + 1, *taille_dispo);
            printf(">> ");

            if (scanf("%d", &choix) != 1) {
                printf(ROUGE"⚠️ Saisie invalide. Veuillez entrer un nombre.\n"RESET);
                while(getchar() != '\n');
                choix = -1;
                continue;
            }

            if (choix < 1 || choix > *taille_dispo) {
                printf(ROUGE"⚠️ Vous devez choisir un nombre entre 1 et %d !\n"RESET, *taille_dispo);
                choix = -1;
            } else if (disponibles[choix - 1].pv == -1) {
                printf(ROUGE "⚠️ Ce combattant a déjà été choisi. Sélectionnez-en un autre.\n" RESET);
                choix = -1;
            }
        } while (choix < 1 || choix > *taille_dispo || disponibles[choix - 1].pv == -1);

        equipe[i] = disponibles[choix - 1];
        disponibles[choix - 1].pv = -1;

        clearScreen();
        printf(VERT"\n\n\n✅ %s a été ajouté à votre équipe🥊!\n"RESET, equipe[i].nom);
        sleep(2);
    }
}


//choix combattants joueur contre joueur
void selectionnerEquipesJoueurs(Combattant* disponibles, int* taille_dispo, Combattant* equipeJ1, Combattant* equipeJ2, int taille_equipe) {
    int choix;

    for (int i = 0; i < taille_equipe; i++) {
        // Tour du Joueur 1
        clearScreen();
        afficheCombattants(disponibles, *taille_dispo);
        do {
            printf(BLANC"Joueur 1, choisissez votre combattant n°%d (1 à %d):\n"RESET, i+1, *taille_dispo);
            printf(">> ");
            scanf("%d", &choix);
        } while(choix < 1 || choix > *taille_dispo || disponibles[choix-1].pv == -1);

        equipeJ1[i] = disponibles[choix - 1];
        disponibles[choix - 1].pv = -1;

        clearScreen();
        printf("\n\n\n");
        printf(VERT" ✅%s %s a été ajouté à l'équipe du Joueur 1!🥊\n"RESET, equipeJ1[i].nom, getEmoji(equipeJ1[i].element));
        sleep(2);

        // Tour du Joueur 2
        clearScreen();
        afficheCombattants(disponibles, *taille_dispo);
        do {
            printf(BLANC"Joueur 2, choisissez votre combattant n°%d (1 à %d):\n"RESET, i+1, *taille_dispo);
            printf(">> ");
            scanf("%d", &choix);
        } while(choix < 1 || choix > *taille_dispo || disponibles[choix-1].pv == -1);

        equipeJ2[i] = disponibles[choix - 1];
        disponibles[choix - 1].pv = -1;

        clearScreen();
        printf("\n\n\n");
        printf(VERT" ✅%s %s a été ajouté à l'équipe du Joueur 2!🥊\n"RESET, equipeJ2[i].nom, getEmoji(equipeJ2[i].element));
        sleep(2);
    }
}

// affciche l'équipe 
void afficherEquipe(Combattant* equipe, int taille_equipe) {
    printf("\n");
    for (int i = 0; i < taille_equipe; i++) {
        const char* emoji = getEmoji(equipe[i].element);
        printf("%d. "BEIGE"%s"RESET" %s\n", i + 1, equipe[i].nom, emoji);
        printf("\n");
        printf("    VIE:" JAUNE" %.1f" RESET " |  ATTAQUE:" JAUNE" %d" RESET " |  DEFENSE:" JAUNE" %d" RESET " |  AGILITE:" JAUNE" %d" RESET " |  VITESSE:" JAUNE" %d" RESET "\n\n",
               equipe[i].pv, equipe[i].attaque, equipe[i].defense, equipe[i].agilite, equipe[i].vitesse);
    }
    printf("   ============================================\n");
}


//choix des combattants pour l'ordi 
void choisirEquipeAleatoire(Combattant* disponibles, int taille_dispo, Combattant* equipe, int taille_equipe) {
    int indice;
    clearScreen();
    printf("\n\n\n");
    printf(VERT"L'ordinateur a choisi :\n"RESET);
    for(int i = 0; i < taille_equipe; i++) {
        do {
            indice = rand() % taille_dispo;
        } while(disponibles[indice].pv == -1);

        equipe[i] = disponibles[indice];
        disponibles[indice].pv = -1;

        const char* emoji = getEmoji(equipe[i].element);
        printf("    - %s %s\n", equipe[i].nom, emoji);
        sleep(3);
    }
}

void jeuCommence() {
    sleep(5);
    clearScreen();
    printf("\n\n\n");
    printf("   ===  " ROUGE "Le jeu va commencer🎮! " RESET "  ===\n");
}





