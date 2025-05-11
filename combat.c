#include "combat.h"

// Fonction à appeler après que le personnage a joué
void definir_prochain_tour(Combattant *perso) {
    // Le personnage devra attendre vitesse avant de rejouer
    perso->prochain_tour = perso->vitesse;
}

int choisir_combattant(Combattant *equipe, int taille) {
    int choix = -1;
    do {
        printf("Choisissez un combattant actif :\n");
        for (int i = 0; i < taille; i++) {
            if (!equipe[i].est_KO && equipe[i].prochain_tour <= 0) {
                printf("%d. %s (PV: %.2f)\n", i, equipe[i].nom, equipe[i].pv);
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
                printf("%d. %s (PV: %.2f)\n", i, equipe[i].nom, equipe[i].pv);
            }
        }
        scanf("%d", &choix);
    } while (choix < 0 || choix >= taille || equipe[choix].est_KO);
    return choix;
}

void maj_tours_combattants(Combattant *equipe, int taille) {
    for (int i = 0; i < taille; i++) {
        if (equipe[i].prochain_tour > 0){
            equipe[i].prochain_tour--;
        }
        for (int j = 0; j < MAX_TECHS; j++) {
            if (equipe[i].temps_recharge[j] > 0){
                equipe[i].temps_recharge[j]--;
            }
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
    float degats = attaquant->attaque;
    // Réduction des dégâts par la défense (mais minimum 1 point de dégât)
    degats = degats - (cible->defense / 2.0);
    if (degats < 1.0) {
        degats = 1.0; // Au moins 1 point de dégât
    }
    // Application du multiplicateur d'élément
    float multi = multiplicateur(attaquant->element, cible->element);
    degats = degats * multi;
    // Appliquer les dégâts
    cible->pv -= degats;
    // Vérifier si la cible est KO
    if (cible->pv <= 0) {
        cible->pv = 0;
        cible->est_KO = 1;
        printf("❌ %s est KO !\n", cible->nom);
    } else {
        printf("💥 %s inflige %.1f dégâts à %s (reste %.1f PV)\n", attaquant->nom, degats, cible->nom, cible->pv);
    }
}

void utiliserTechnique(Combattant *attaquant, Combattant *cible, TechniqueSpeciale *tech) {
    if (tech == NULL) {
        // attaque normale
        attaque_normale(attaquant, cible);
        return;
    }
    // Vérifier si la technique est déjà prête à être utilisée (temps_recharge == 0)
    for (int i = 0; i < MAX_TECHS; i++) {
        if (strcmp(attaquant->techniques[i].nom, tech->nom) == 0) {
            // On ne peut pas utiliser la technique si son temps de recharge n'est pas écoulé
            if (attaquant->temps_recharge[i] > 0) {
                printf("❌ La technique %s est en recharge et ne peut pas être utilisée ce tour !\n", tech->nom);
                return;
            }
            // Sinon, on définit le temps de recharge de la technique à la valeur de 'tours' de la technique
            attaquant->temps_recharge[i] = tech->tours;  // Assigner le nombre de tours de recharge
            break;
        }
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
                }
                printf("⚡ %s utilise %s et inflige %.2f dégâts à %s !\n", attaquant->nom, tech->nom, degats, cible->nom);
            break;
        case DEFENSE:
            attaquant->defense += tech->puissance;
            printf("🛡️ %s renforce sa défense avec %s (+%.2f) !\n", attaquant->nom, tech->nom, tech->puissance);
            break;
        case AGILITE:
            attaquant->agilite += tech->puissance;
            printf("💨 %s augmente son agilité avec %s (+%.2f) !\n", attaquant->nom, tech->nom, tech->puissance);
            break;
        case SOIN:
            attaquant->pv += tech->puissance;
            if (attaquant->pv > attaquant->pv_max){
                attaquant->pv = attaquant->pv_max;
            }
            printf("🧪 %s se soigne avec %s et récupère %.2f PV !\n", attaquant->nom, tech->nom, tech->puissance);
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
    if (joueur->est_KO) {
        printf("⚠️ %s est KO et ne peut pas agir !\n", joueur->nom);
        return;
    } 
    // Vérifier si le joueur est étourdi ou gelé
    if (est_incapacite(joueur)) {
        printf("⛔ %s est incapable d'agir ce tour (gelé ou étourdi) !\n", joueur->nom);
        mettreAJourEffets(joueur, NULL);
        return;
    }    
    // Appliquer les effets de statut
    appliquerDegats(joueur);
    mettreAJourEffets(joueur, NULL);
    printf("\n🔵 Tour de %s (PV: %.1f/%.1f)\n", joueur->nom, joueur->pv, joueur->pv_max);
    // Afficher les cibles disponibles
    printf("Choisissez une cible :\n");
    int nb_cibles_valides = 0;
    for (int i = 0; i < taille_adversaires; i++) {
        if (!adversaires[i].est_KO) {
            printf("%d. %s (PV: %.1f/%.1f)\n", 
                  i, adversaires[i].nom, 
                  adversaires[i].pv, 
                  adversaires[i].pv_max);
            nb_cibles_valides++;
        }
    }
    if (nb_cibles_valides == 0) {
        printf("Aucune cible valide disponible !\n");
        return;
    }
    int choix_cible;
    do {
        printf("Votre choix (0-%d) : ", taille_adversaires-1);
        scanf("%d", &choix_cible);
    } while (choix_cible < 0 || choix_cible >= taille_adversaires || 
            adversaires[choix_cible].est_KO);
    // Choisir l'action
    int action;
    printf("\n1. Attaque normale\n");
    printf("2. Technique spéciale\n");
    printf("Choisissez une action : ");
    scanf("%d", &action);
    while (action != 1 && action != 2) {
        printf("Choix invalide. Veuillez choisir 1 ou 2 : ");
        scanf("%d", &action);
    }
    if (action == 2) {
        // Afficher les techniques disponibles
        printf("\nTechniques disponibles :\n");
        int tech_dispo[MAX_TECHS] = {0};
        int nb_tech = 0;
        for (int i = 0; i < MAX_TECHS; i++) {
            if (joueur->techniques[i].nom[0] != '\0' && (joueur->temps_recharge[i] <= 0)) {
                tech_dispo[nb_tech] = i;
                printf("%d. %s\n", nb_tech + 1, joueur->techniques[i].nom);
                nb_tech++;
            }
        }
        if (nb_tech > 0) {
            int choix_tech;
            do {
                printf("Choisissez une technique (1-%d) : ", nb_tech);
                scanf("%d", &choix_tech);
            } while (choix_tech < 1 || choix_tech > nb_tech);
            utiliserTechnique(joueur, &adversaires[choix_cible], 
                             &joueur->techniques[tech_dispo[choix_tech - 1]]);
        } else {
            printf("Aucune technique disponible. Attaque normale à la place.\n");
            attaque_normale(joueur, &adversaires[choix_cible]);
        }
    } else {
        attaque_normale(joueur, &adversaires[choix_cible]);
    }
    // Mise à jour du prochain tour
    definir_prochain_tour(joueur);
}
void initialiser_comb(Combattant *equipe1, int taille1, Combattant *equipe2, int taille2) {
    srand(time(NULL));  // Initialisation du générateur aléatoire
    printf("Le combat commence entre deux équipes !\n");
    // Initialisation des combattants
    for (int i = 0; i < taille1; i++) {
        equipe1[i].pv = equipe1[i].pv_max;
        equipe1[i].est_KO = 0;
        equipe1[i].prochain_tour = 0;
        for (int j = 0; j < MAX_TECHS; j++) {
            equipe1[i].temps_recharge[j] = 0;
        }
    }
    for (int i = 0; i < taille2; i++) {
        equipe2[i].pv = equipe2[i].pv_max;
        equipe2[i].est_KO = 0;
        equipe2[i].prochain_tour = 0;
        for (int j = 0; j < MAX_TECHS; j++) {
            equipe2[i].temps_recharge[j] = 0;
        }
    }
}


// Gère l'ensemble du combat entre deux équipes
void boucle_de_combat(Combattant *equipe1, int taille1, Combattant *equipe2, int taille2) {
    void boucle_de_combat(Combattant *equipe1, int taille1, Combattant *equipe2, int taille2) {
    printf("\n💥 🎮 Début du combat !\n");
    int tour = 1;
    // Initialisation des combattants
    initialiser_comb(equipe1, taille1, equipe2, taille2);
    // Boucle principale du combat
    while (!equipe_est_KO(equipe1, taille1) && !equipe_est_KO(equipe2, taille2)) {
        printf("\n=== Tour %d ===\n", tour); 
        // Afficher l'état des équipes
        printf("\n--- Équipe 1 ---\n");
        for (int i = 0; i < taille1; i++) {
            printf("%s: %.1f/%.1f PV", equipe1[i].nom, equipe1[i].pv, equipe1[i].pv_max);
            if (equipe1[i].est_KO){
                printf(" (KO)");
            }
            printf("\n");
        }
        printf("\n--- Équipe 2 ---\n");
        for (int i = 0; i < taille2; i++) {
            printf("%s: %.1f/%.1f PV", equipe2[i].nom, equipe2[i].pv, equipe2[i].pv_max);
            if (equipe2[i].est_KO){
                printf(" (KO)");
            }
            printf("\n");
        }
        // Mise à jour des tours de recharge
        maj_tours_combattants(equipe1, taille1);
        maj_tours_combattants(equipe2, taille2);
        // Tour de l'équipe 1
        printf("\n--- Tour de l'équipe 1 ---\n");
        for (int i = 0; i < taille1; i++) {
            if (!equipe1[i].est_KO && equipe1[i].prochain_tour <= 0) {
                effectuer_tour(&equipe1[i], equipe2, taille2);
                if (equipe_est_KO(equipe2, taille2)){
                    break;
                }
            }
        }
        // Vérifier si l'équipe 2 est KO
        if (equipe_est_KO(equipe2, taille2)){
            break;
        }
        // Tour de l'équipe 2
        printf("\n--- Tour de l'équipe 2 ---\n");
        for (int i = 0; i < taille2; i++) {
            if (!equipe2[i].est_KO && equipe2[i].prochain_tour <= 0) {
                effectuer_tour(&equipe2[i], equipe1, taille1);
                if (equipe_est_KO(equipe1, taille1)){
                    break;
                }
            }
        }
        
        tour++;
    }
    // Fin du combat
    printf("\n🎯 Fin du combat !\n");
    if (equipe_est_KO(equipe1, taille1)) {
        printf("🏆 L'équipe 2 a gagné !\n");
    } else {
        printf("🏆 L'équipe 1 a gagné !\n");
    }
}
