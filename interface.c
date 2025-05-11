#include "interface.h"
#include "type.h"

#define NOM_MAX 500 

//fonction pour les emojis 
const char* getEmoji(Element element) {
    switch (element) {
        case FEU:   return "🔥";
        case EAU:   return "🌊";
        case TERRE: return "🌍";
        case AIR:   return "🌪";
        default:    return "";
    }
}

//affichage du menu principal 
void debutJeu(){
    //affichage nom du jeu 
    printf("===================================\n");
    printf("  ===  ");
    printf(ROUGE "CY-AVATAR FIGHTERS 🤺" RESET);
    printf("  ===\n");
    printf("===================================\n");
    sleep(4); // Pause de 2 secondes avant disparition de l'écran
    printf("\033[H\033[2J"); // Pour effacer l'écran
}

//jouer ou quitter
int choixPrincipal(){
    int choix;

    printf(ROUGE"Je souhaite:\n"RESET);
    printf("1. Jouer une partie\n");
    printf("2. Quitter\n");
    do {
        printf("Choisissez une option (1 ou 2):\n");
        printf(">> ");
        scanf("%d", &choix);
    } while (choix != 1 && choix != 2);

    if (choix == 1) {
        printf("Vous avez choisi de" JAUNE" jouer une partie"RESET"🥳!\n");         
        sleep(1);
        printf("\033[H\033[2J");
    } else {
        printf("Vous avez choisi de" ROUGE" nous quitter" RESET"😤!\n");
        sleep(1);
        printf("\033[H\033[2J");
        printf("==================================\n");
        printf("   ===  ");
        printf(ROUGE "A la Prochaine ! " RESET);
        printf("  ===\n");
        printf("==================================\n");
    }
    return choix;
}

int choixModeJeu(){
    int mode;

    printf(ROUGE"Je souhaite:\n"RESET);
    printf("1. Jouer contre Ordi\n");
    printf("2. Jouer contre Joueur 2\n");
    do {
        printf(">> ");
        scanf("%d", &mode);
    } while (mode != 1 && mode != 2);

    if (mode == 1) {
        printf("Vous avez choisi de" JAUNE" jouer contre l'ordi"RESET"👨‍💻!\n");      
        sleep(1);
        printf("\033[H\033[2J");
    } else {
        printf("Vous avez choisi de" JAUNE" jouer contre Joueur 2" RESET"🤼!\n");
        sleep(1);
        printf("\033[H\033[2J");
    }
    return mode;
}

//nom effet
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
        case AUCUN:
        default: return "AUCUN";
    }
}

//affichage des combattants
void afficheCombattants(Combattant* tab, int taille) {
    printf("   ==========================================\n");
    printf("   === " JAUNE "      Liste des Combattants      " RESET "  ===\n");
    printf("   ===========================================\n\n");

    for (int i = 0; i < taille; i++) {
        Combattant c = tab[i];
        printf(" (%d)"BEIGE" %s"RESET" %s\n", i + 1, c.nom, getEmoji(c.element));
        printf("\n");
        printf("     VIE: " JAUNE "%.1f" RESET " | ATTAQUE: " JAUNE "%d" RESET " | DEFENSE: " JAUNE "%d" RESET
               " | AGILITE: " JAUNE "%d" RESET " | VITESSE: " JAUNE "%d" RESET "\n", 
               c.pv, c.attaque, c.defense, c.agilite, c.vitesse);

        printf(BEIGE"  \n  ⚔ Techniques spéciales:\n"RESET);

        for (int j = 0; j < MAX_TECHS; j++) {
            TechniqueSpeciale t = c.techniques[j];
            printf("       %d. " JAUNE "%s" RESET " | Portée: "JAUNE"%d"RESET" | Puissance:"JAUNE"%d"RESET" | Tours: "JAUNE"%d\n"RESET,
                   j + 1, t.nom, t.portee, t.puissance, t.tours);
        }
        if(i < taille - 1){
            printf("     ----------------------------------------\n\n");
        }
    }

    printf("   =========================================\n\n");
}



void selectionnerEquipe(Combattant* disponibles, int* taille_dispo, Combattant* equipe, int taille_equipe) {
    int choix;
    for (int i = 0; i < taille_equipe; i++) {
        do {
            printf(BLANC"\nChoisissez votre combattant n°%d (1 à %d):\n"RESET, i + 1, *taille_dispo);
            printf(">> ");
            if (scanf("%d", &choix) != 1) {
                printf("Erreur de lecture du choix\n");
                while(getchar() != '\n');
            }

            if (choix < 1 || choix > *taille_dispo) {
                printf(ROUGE"⚠️ Vous devez choisir un nombre entre 1 et %d !\n"RESET, *taille_dispo);
            } else if (disponibles[choix - 1].pv == -1) {
                printf(ROUGE "⚠️ Vous avez déjà choisi ce combattant ! Choisissez-en un autre.\n" RESET);
                choix = -1;
            }
        } while (choix < 1 || choix > *taille_dispo || disponibles[choix - 1].pv == -1);

        equipe[i] = disponibles[choix - 1];
        disponibles[choix - 1].pv = -1;

        printf(VERT" %s a été ajouté à votre équipe !🥊\n"RESET, equipe[i].nom);
        sleep(1);

        if (i < taille_equipe - 1) {
            printf("\033[H\033[2J");
            afficheCombattants(disponibles, *taille_dispo);
        }
    }
}

void choisirEquipeAleatoire(Combattant* disponibles, int taille_dispo, Combattant* equipe, int taille_equipe) {
    int indice;
    printf("\033[H\033[2J");
    printf("L'ordinateur a choisi :\n");
    for(int i = 0; i < taille_equipe; i++) {
        do {
            indice = rand() % taille_dispo;
        } while(disponibles[indice].pv == -1);

        equipe[i] = disponibles[indice];
        disponibles[indice].pv = -1;

        const char* emoji = getEmoji(equipe[i].element);
        printf("%s %s\n", equipe[i].nom, emoji);
        sleep(1);
    }
}

void afficherEquipe(Combattant* equipe, int taille_equipe) {
    printf("\033[H\033[2J");
    printf("\n");
    for (int i = 0; i < taille_equipe; i++) {
        const char* emoji = getEmoji(equipe[i].element);
        printf("%d. "BEIGE"%s"RESET" %s\n", i + 1, equipe[i].nom, emoji);
        printf("\n");
        printf("    VIE:" JAUNE" %.1f" RESET " |  ATTAQUE:" JAUNE" %d" RESET " |  DEFENSE:" JAUNE" %d" RESET " |  AGILITÉ:" JAUNE" %d" RESET " |  VITESSE:" JAUNE" %d" RESET "\n\n",
               equipe[i].pv, equipe[i].attaque, equipe[i].defense, equipe[i].agilite, equipe[i].vitesse);
    }
    printf("   =================================\n");
}


void selectionnerEquipesJoueurs(Combattant* disponibles, int* taille_dispo, Combattant* equipeJ1, Combattant* equipeJ2, int taille_equipe) {
    int choix;
    for (int i = 0; i < taille_equipe; i++) {
        const char* emoji1 = getEmoji(equipeJ1[i].element);

        afficheCombattants(disponibles, *taille_dispo);
        do {
            printf(BLANC"Joueur 1, choisissez votre combattant n°%d (1 à %d):\n"RESET, i+1, *taille_dispo);
            scanf("%d", &choix);
            if (choix < 1 || choix > *taille_dispo || disponibles[choix-1].pv == -1)
                printf(ROUGE"⚠️ Choix invalide. Réessayez.\n"RESET);
        } while(choix < 1 || choix > *taille_dispo || disponibles[choix-1].pv == -1);

        equipeJ1[i] = disponibles[choix - 1];
        disponibles[choix - 1].pv = -1;
        printf(VERT"%s %s a été ajouté à l'équipe du Joueur 1!🥊\n"RESET, equipeJ1[i].nom, emoji1);
        sleep(1);
        printf("\033[H\033[2J");

        const char* emoji2 = getEmoji(equipeJ2[i].element);

        afficheCombattants(disponibles, *taille_dispo);
        do {
            printf(BLANC"Joueur 2, choisissez votre combattant n°%d (1 à %d):\n"RESET, i+1, *taille_dispo);
            scanf("%d", &choix);
            if (choix < 1 || choix > *taille_dispo || disponibles[choix-1].pv == -1)
                printf(ROUGE"⚠️ Choix invalide. Réessayez.\n"RESET);
        } while(choix < 1 || choix > *taille_dispo || disponibles[choix-1].pv == -1);

        equipeJ2[i] = disponibles[choix - 1];
        disponibles[choix - 1].pv = -1;
        printf(VERT"%s %s a été ajouté à l'équipe du Joueur 2!🥊\n"RESET, equipeJ2[i].nom, emoji2);
        sleep(1);
        printf("\033[H\033[2J");
    }
}
