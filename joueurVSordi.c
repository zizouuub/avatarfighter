#include "joueurVSordi.h"

int niveau_ia = 0; // 0=noob, 1=facile, 2=moyen

int choisir_action_joueur() {
    int choix;
    printf("1. Attaque normale\n");
    printf("2. Technique spéciale\n");
    printf("Choisissez votre action : ");
    scanf("%d", &choix);
    if (choix < 1 || choix > 2) {
        printf("⛔ Choix invalide, vous devez choisir entre 1 et 2. Réessayez !!\n");
        return choisir_action_joueur(); // redemande le choix
    }
    return choix;
}

Combattant* choisir_cible_soin(Combattant *equipe, int taille) {
    Combattant *plus_blesse = NULL;
    float pourcentage_min = 1.0;  // 100%
    for (int i = 0; i < taille; i++) {
        if (!equipe[i].est_KO && equipe[i].pv < equipe[i].pv_max) {
            float pourcentage = equipe[i].pv / equipe[i].pv_max;
            if (pourcentage < pourcentage_min) {
                pourcentage_min = pourcentage;
                plus_blesse = &equipe[i];
            }
        }
    }
    return plus_blesse;
}

int choisir_cible1(Combattant *equipe, int taille) {
    int choix = -1;
    printf("Choisissez une cible :\n");
    for (int i = 0; i < taille; i++) {
        if (!equipe[i].est_KO) {
            printf("%d. %s (PV: %.2f)\n", i, equipe[i].nom, equipe[i].pv);
        }
    }
    scanf("%d", &choix);
    while (choix < 0 || choix >= taille || equipe[choix].est_KO) {
        printf("⛔ Choix invalide. Réessayez : ");
        scanf("%d", &choix);
    }
    return choix;
}

Combattant* choisir_cible_faible(Combattant *ennemis, int taille) {
    Combattant *cible = NULL;
    float min_pv = 9999;
    for (int i = 0; i < taille; i++) {
        if (!ennemis[i].est_KO && ennemis[i].pv < min_pv) {
            cible = &ennemis[i];
            min_pv = ennemis[i].pv;
        }
    }
    return cible;
}

Combattant* choisir_cible_attaque(Combattant *equipe, int taille) {
    if (niveau_ia == 1) {
        return choisir_cible_aleatoire(equipe, taille);
    } else {
        return choisir_cible_faible(equipe, taille);
    }
}

Combattant* choisir_cible_aleatoire(Combattant *equipe, int taille) {
    int index;
    do {
        index = rand() % taille;
    } while (equipe[index].est_KO);
    return &equipe[index];
}

int technique_disponible(Combattant *c) {
    for (int i = 0; i < MAX_TECHS; i++) {
        if (c->temps_recharge[i] <= 0) {
            return 1;
        }
    }
    return 0;
}

int choisir_action_ia(Combattant *ia) {
    if (niveau_ia == 0) {
        return rand() % 2 + 1; // action aléatoire (1 ou 2)
    }
    if (technique_disponible(ia)) {
        return 2; // utilise sa technique si elle est prête
    }
    return 1; // sinon attaque normale
}

void attaque_elementaire1(Combattant *attaquant, Combattant *cible) {
    printf("🔥 %s attaque %s !\n", attaquant->nom, cible->nom);
    attaque_normale(attaquant, cible);
}

void tour(Combattant *acteur, Combattant *equipe_ennemie, int taille_ennemie, 
          Combattant *equipe_alliee, int taille_alliee, int est_joueur) {
    if (acteur->est_KO) {
        return;
    }
    printf("\n🔁 Tour de %s\n", acteur->nom);
    int action;
    int tech = -1;
    Combattant *cible = NULL;
    if (est_joueur) {
        action = choisir_action_joueur();
        if (action == 2) {
            printf("Choisissez une technique :\n");
            int compteur = 1;
            int tech_indices[MAX_TECHS] = {-1, -1, -1};
            int nb_tech_dispo = 0;
            // D'abord, compter et lister les techniques disponibles
            for (int i = 0; i < MAX_TECHS; i++) {
                if (acteur->temps_recharge[i] <= 0) {
                    tech_indices[nb_tech_dispo] = i;
                    printf("%d. %s", compteur, acteur->techniques[i].nom);
                    if (acteur->techniques[i].effet == SOIN) {
                        printf(" (Soin: %.1f PV)", acteur->techniques[i].puissance);
                    } else {
                        printf(" (Dégâts: %.1f)", acteur->techniques[i].puissance);
                    }
                    printf("\n");
                    nb_tech_dispo++;
                    compteur++;
                }
            }
            if (nb_tech_dispo == 0) {
                printf("Aucune technique disponible ! Attaque normale à la place.\n");
                action = 1;
            } else {
                int choix_tech;
                scanf("%d", &choix_tech);
                
                if (choix_tech < 1 || choix_tech > nb_tech_dispo) {
                    printf("Choix invalide, attaque normale à la place.\n");
                    action = 1;
                } else {
                    tech = tech_indices[choix_tech - 1];
                }
            }
        }
        cible = &equipe_ennemie[choisir_cible1(equipe_ennemie, taille_ennemie)];
    } else {
        action = choisir_action_ia(acteur);
        if (action == 2) {
            // Chercher une technique disponible
            for (int i = 0; i < MAX_TECHS; i++) {
                if (acteur->temps_recharge[i] <= 0) {
                    tech = i;
                    break;
                }
            }
            if (tech != -1) {
                TechniqueSpeciale t = acteur->techniques[tech];
                if (t.effet == SOIN) {
                    cible = choisir_cible_soin(equipe_alliee, taille_alliee);
                    if (cible == NULL) {
                        cible = choisir_cible_attaque(equipe_ennemie, taille_ennemie);
                        action = 1; // Si personne à soigner, on fait une attaque normale
                    }
                } else {
                    cible = choisir_cible_attaque(equipe_ennemie, taille_ennemie);
                }
            } else {
                action = 1;
                cible = choisir_cible_attaque(equipe_ennemie, taille_ennemie);
            }
        } else {
            cible = choisir_cible_attaque(equipe_ennemie, taille_ennemie);
        }
        printf("🤖 L'IA choisit l'action %d\n", action);
    }
    if (action == 1) {
        attaque_normale(acteur, cible);
    } else if (tech != -1) {
        // Utiliser la fonction utiliserTechnique de combat.c
        utiliserTechnique(acteur, cible, &acteur->techniques[tech]);
    }
}

void maj_recharge(Combattant *c) {
    if (c == NULL || c->est_KO) {
        return;
    }
    for (int i = 0; i < MAX_TECHS; i++) {
        if (c->temps_recharge[i] > 0) {
            c->temps_recharge[i]--;
            if (c->temps_recharge[i] < 0) {
                c->temps_recharge[i] = 0;
            }
        }
    }
    for (int i = 0; i < NB_EFFETS; i++) {
        if (c->duree_effet[i] > 0) {
            c->duree_effet[i]--;
            if (c->duree_effet[i] <= 0) {
                c->effets[i] = AUCUN;
            }
        }
    }
}

void afficher_etat_combattant(Combattant *c) {
    if (c->est_KO) {
        printf("%s: K.O.\n", c->nom);
        return;
    }
    printf("%s: PV %.1f/%.1f [", c->nom, c->pv, c->pv_max);
    int j;
    int premier = 1;
    for (j = 0; j < NB_EFFETS; j++) {
        if (c->effets[j] != AUCUN && c->duree_effet[j] > 0) {
            if (premier == 0) {
                printf(", ");
            }
            if (c->effets[j] == POISON) {
                printf("POISON");
            } else if (c->effets[j] == BRULURE) {
                printf("BRULURE");
            } else if (c->effets[j] == GEL) {
                printf("GEL");
            } else if (c->effets[j] == STUN) {
                printf("STUN");
            } else if (c->effets[j] == SOIN) {
                printf("SOIN");
            } else if (c->effets[j] == ATTAQUE) {
                printf("ATQ+");
            } else if (c->effets[j] == DEFENSE) {
                printf("DEF+");
            } else if (c->effets[j] == AGILITE) {
                printf("AGI+");
            } else {
                printf("EFFET");
            }
            printf("(%d)", c->duree_effet[j]);
            premier = 0;
        }
    }
    printf("] | Techniques: ");
    int i;
    for (i = 0; i < MAX_TECHS; i++) {
        if (c->techniques[i].nom[0] != '\0') {
            printf("%s", c->techniques[i].nom);
            if (c->temps_recharge[i] > 0) {
                printf("(temps de recharge :%d) ", c->temps_recharge[i]);
            } else {
                printf("(Prêt) ");
            }
        }
    }
    printf("\n");
}

void lancer_combat(Combattant *equipe_joueur, int taille_joueur, Combattant *equipe_ia, int taille_ia) {
    printf("\nChoisissez la difficulté IA (0 = noob, 1 = facile, 2 = moyen) : ");
    scanf("%d", &niveau_ia);
    printf("🔥⚔️ Début du combat !\n");
    // Initialisation des temps de recharge
    for (int i = 0; i < taille_joueur; i++) {
        for (int j = 0; j < MAX_TECHS; j++) {
            equipe_joueur[i].temps_recharge[j] = 0;
        }
    }
    for (int i = 0; i < taille_ia; i++) {
        for (int j = 0; j < MAX_TECHS; j++) {
            equipe_ia[i].temps_recharge[j] = 0;
        }
    }
    int tour_actuel = 1;
    while (!equipe_est_KO(equipe_joueur, taille_joueur) && !equipe_est_KO(equipe_ia, taille_ia)) {
        printf("\n=== Tour %d ===\n", tour_actuel++);
        printf("\n--- Votre équipe ---\n");
        for (int i = 0; i < taille_joueur; i++) {
            afficher_etat_combattant(&equipe_joueur[i]);
        }
        printf("\n--- Équipe ennemie ---\n");
        for (int i = 0; i < taille_ia; i++) {
            afficher_etat_combattant(&equipe_ia[i]);
        }
        // Tour du joueur
        for (int i = 0; i < taille_joueur; i++) {
            tour(&equipe_joueur[i], equipe_ia, taille_ia, equipe_joueur, taille_joueur, 1);
            if (equipe_est_KO(equipe_ia, taille_ia)) {
                break;
            }
        }
        if (equipe_est_KO(equipe_ia, taille_ia)) {
            break;
        }
        // Tour de l'IA
        for (int i = 0; i < taille_ia; i++) {
            tour(&equipe_ia[i], equipe_joueur, taille_joueur, equipe_ia, taille_ia, 0);
            if (equipe_est_KO(equipe_joueur, taille_joueur)) {
                break;
            }
        }
        // Mise à jour des recharges
        for (int i = 0; i < taille_joueur; i++) {
            maj_recharge(&equipe_joueur[i]);
        }
        for (int i = 0; i < taille_ia; i++) {
            maj_recharge(&equipe_ia[i]);
        }
    }
    printf("\n🎮 Le combat est terminé !\n");
    if (equipe_est_KO(equipe_joueur, taille_joueur)) {
        printf("💀 Vous êtes K.O. L'IA gagne !\n");
    } else {
        printf("🏆 Vous avez vaincu tous les ennemis !\n");
    }
}
