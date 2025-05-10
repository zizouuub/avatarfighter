#include "combat.h"

int choisir_combattant(Combattant *equipe, int taille) {
    int choix = -1;
    do {
        printf("Choisissez un combattant actif :\n");
        for (int i = 0; i < taille; i++) {
            if (!equipe[i].est_KO && equipe[i].prochain_tour <= 0) {
                printf("%d. %s (PV: %d)\n", i, equipe[i].nom, equipe[i].pv);
            }
        }
        scanf("%d", &choix);
    } while (choix < 0 || choix >= taille || equipe[choix].est_KO || equipe[choix].prochain_tour > 0);
    return choix;
}

int choisir_cible(Combattant *equipe, int taille) {
    int choix = -1;
    do {
        printf("Choisissez une cible :\n");
        for (int i = 0; i < taille; i++) {
            if (!equipe[i].est_KO) {
                printf("%d. %s (PV: %d)\n", i, equipe[i].nom, equipe[i].pv);
            }
        }
        scanf("%d", &choix);
    } while (choix < 0 || choix >= taille || equipe[choix].est_KO);
    return choix;
}

void maj_tours_combattants(Combattant *equipe, int taille) {
    for (int i = 0; i < taille; i++) {
        if (equipe[i].prochain_tour > 0) equipe[i].prochain_tour--;
        for (int j = 0; j < MAX_TECHS; j++) {
            if (equipe[i].temps_recharge[j] > 0)
                equipe[i].temps_recharge[j]--;
        }
    }
}

// Fonction pour initialiser le combat
void initialiser_combat(Combattant *equipe1, int taille1, Combattant *equipe2, int taille2) {
    // Initialisation des compteurs de tour et des effets
    for (int i = 0; i < taille1; i++) {
        initialiser_combattant(&equipe1[i]);
        equipe1[i].pv = equipe1[i].pv_max;
    }
    for (int i = 0; i < taille2; i++) {
        initialiser_combattant(&equipe2[i]);
        equipe2[i].pv = equipe2[i].pv_max;
    }
}

// fct pour vérifier si une equipe est KO
int equipe_est_KO(Combattant *equipe, int taille) {
    for (int i = 0; i < taille; i++) {
        if (!equipe[i].est_KO) { //choisir le premier qui n’est pas K.O
            return 0;  // L'equipe n'est pas KO
        }
    }
    return 1;  // L'equipe est KO
}

// Calcule les dégâts en fonction de l'attaque, la défense, et l'element
void attaque_normale(Combattant *attaquant, Combattant *cible) {
    float degats = (attaquant->attaque) - (cible->defense);
    if (degats < 0) {
        degats = 0; // minimum 0 dégâts
    }else{
        // Multiplie les dégâts selon le type d'element
        degats = multiplicateur(attaquant->element, cible->element) * degats; // Applique le multiplicateur d'élément
    } 
    // Applique les dégâts
    cible->pv -= degats;
    // Si la cible est à 0 PV ou moins, elle est KO
    if (cible->pv <= 0) {
        cible->pv = 0;
        cible->est_KO = 1;
        printf("❌ %s est KO !\n", cible->nom);
    } else {
        // Affiche le résultat du coup
        printf("💥 %s inflige %.2f dégâts à %s (reste %d PV)\n", attaquant->nom, degats, cible->nom, cible->pv);
    }
}


void utiliserTechnique(Combattant *attaquant, Combattant *cible, TechniqueSpeciale *tech) {
    if (tech == NULL) {
        // attaque normale
        attaque_normale(attaquant, cible);
        return;
    }
    float degats = 0; // Initialisation des dégâts
    switch (tech->effet) {
        case ATTAQUE:
                degats = tech->puissance;
                if (tech->puissance > 20) {
                    degats += 20;
                } else if (tech->puissance > 10) {
                    degats += 10;
                }
                // Application du multiplicateur selon l'élément
                degats *= multiplicateur(attaquant->element, cible->element);
                cible->pv -= degats;
                if (cible->pv < 0){ 
                cible->pv = 0;
                cible->est_KO = 1;
                printf("❌ %s est KO !\n", cible->nom);
                }
                printf("⚡ %s utilise %s et inflige %.2f dégâts à %s !\n", attaquant->nom, tech->nom, degats, cible->nom);
            break;
        case DEFENSE:
            attaquant->defense += tech->puissance;
            printf("🛡️ %s renforce sa défense avec %s (+%d) !\n", attaquant->nom, tech->nom, tech->puissance);
            break;
        case AGILITE:
            attaquant->agilite += tech->puissance;
            printf("💨 %s augmente son agilité avec %s (+%d) !\n", attaquant->nom, tech->nom, tech->puissance);
            break;
        case SOIN:
            attaquant->pv += tech->puissance;
            if (attaquant->pv > attaquant->pv_max){
                attaquant->pv = attaquant->pv_max;
            }
            printf("🧪 %s se soigne avec %s et récupère %d PV !\n", attaquant->nom, tech->nom, tech->puissance);
            break;
        case STUN:
            appliquerEffetElementaire(cible, *tech);
            break;
        case GEL:
            appliquerEffetElementaire(cible, *tech);
            break;
        case BRULURE:
            appliquerEffetElementaire(cible, *tech);
            break;
        case POISON:
            appliquerEffetElementaire(cible, *tech);
            printf("🎯 %s applique l'effet %s à %s !\n", attaquant->nom, tech->nom, cible->nom);
            break;
        default:
            printf("Effet inconnu ou non implémenté !\n");
            break;
        }
    if (cible->pv <= 0) {
        cible->pv = 0;
        cible->est_KO = 1;
        printf("❌ %s est KO !\n", cible->nom);
    }
}

// Un joueur effectue une action pendant son tour
void effectuer_tour(Combattant *joueur, Combattant *adversaires, int taille_adversaires) { 
    if (joueur->est_KO){
        printf("⚠️ %s est KO et ne peut pas agir !\n", joueur->nom);
        return; // ne fait rien s'il est KO
    } 
    // verifier joueur etourdi ou gelé
    if (est_incapacite(joueur)) {
        printf(" ⛔ %s est incapable d'agir ce tour (gelé ou étourdi) !\n", joueur->nom);
        mettreAJourEffets(joueur, NULL); // NULL ou une technique fictive juste pour décrémenter
        return;
    }    
    // applique effets subis
    appliquerDegats(joueur); //degats des effets 
    mettreAJourEffets(joueur, NULL); // On peut choisir la technique à utiliser
    // Trouve la première cible ennemie encore vivante
    Combattant *cible = NULL;
    for (int i = 0; i < taille_adversaires; i++) {
        if (adversaires[i].est_KO == 0) { // Si le combattant n'est pas KO
            cible = &adversaires[i]; // On attribue la première cible vivante
        break;
    }
}
    // Si une cible existe, on l’attaque 
    if (cible != NULL) {
        utiliserTechnique(joueur, cible, &joueur->techniques[0]); // Utilise la première technique
    }
}

void initialiser_comb(Combattant *equipe1, int taille1, Combattant *equipe2, int taille2) {
    srand(time(NULL));  // Initialisation du générateur aléatoire
    printf("Le combat commence entre deux équipes !\n");
}


// Gère l'ensemble du combat entre deux équipes
void boucle_de_combat(Combattant *equipe1, int taille1, Combattant *equipe2, int taille2) {
    printf("\n💥 🎮 Début du combat !\n");
    int tour = 1; // Compteur de tours
    int equipe_actuelle = rand()%2; //0 = equipe1 commence, 1 = equipe2 
    // Tant qu'une équipe n'est pas KO
    while (equipe_est_KO(equipe1, taille1) == 0 && equipe_est_KO(equipe2, taille2) == 0) {
        printf("\n🔁 Tour %d\n", tour);
        // Tous les combattants de l’équipe 1 jouent
        if (equipe_actuelle == 0) {
            for (int i = 0; i < taille1; i++) { 
                if (!equipe1[i].est_KO) { // Si le combattant n'est pas KO
                    effectuer_tour(&equipe1[i], equipe2, taille2);
                    }
                }
            equipe_actuelle = 1; // prochain tour : équipe 2
        }
        // On vérifie si l'équipe 2 est éliminée avant de continuer
        if (equipe_est_KO(equipe2, taille2)){
            break; // Sort de la boucle si l'équipe 2 est KO
        }else {
            for (int i = 0; i < taille2; i++) {
                if (!equipe2[i].est_KO) {  // Tous les combattants de l’équipe 2 jouent
                    effectuer_tour(&equipe2[i], equipe1, taille1);
                }
            }
            equipe_actuelle = 0; // prochain tour : équipe 1
        }
        tour++;
        }
    // Fin du combat : affiche le gagnant
    printf("\n🎯 Fin du combat !\n");
    if (equipe_est_KO(equipe1, taille1)) {
        printf("🏆 L'équipe 2 a gagné !\n");
    } else {
        printf("🏆 L'équipe 1 a gagné !\n");
    }
}


