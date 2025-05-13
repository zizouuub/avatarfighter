#include "joueurVSordi.h"
#include <string.h>

int niveau_ia = 1; // Niveau par défaut : facile

void afficher_etat_combattant(Combattant *c) {
    if (c->est_KO) {
        printf("%s: K.O.\n", c->nom);
        return;
    }
    printf("%s: PV %.1f/%.1f [", c->nom, c->pv, c->pv_max);
    int premier = 1;
    for (int j = 0; j < NB_EFFETS; j++) {
        if (c->effets[j] != AUCUN && c->duree_effet[j] > 0) {
            if (!premier) {
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
    printf("]");
    printf(" | Techniques: ");
    for (int i = 0; i < MAX_TECHS; i++) {
        if (c->techniques[i].nom[0] != '\0') {
            printf("%s", c->techniques[i].nom);
            if (c->temps_recharge[i] > 0) {
                printf("(RCD:%d) ", c->temps_recharge[i]);
            } else {
                printf("(Prêt) ");
            }
        }
    }
    printf("\n");
}

Combattant* choisir_cible_aleatoire(Combattant *equipe, int taille) {
    int index;
    int tentatives = 0;
    int cible_valide = 0;
    while (!cible_valide && tentatives < 100) {
        index = rand() % taille;
        if (!equipe[index].est_KO) {
            cible_valide = 1;
        }
        tentatives++;
    }
    if (cible_valide) {
        return &equipe[index];
    } else {
        return NULL;
    }
}

Combattant* choisir_cible_faible(Combattant *equipe, int taille) {
    float min_pv = 9999;
    int index = -1;
    for (int i = 0; i < taille; i++) {
        if (!equipe[i].est_KO && equipe[i].pv < min_pv) {
            min_pv = equipe[i].pv;
            index = i;
        }
    }
    if (index >= 0) {
        return &equipe[index];
    } else {
        return NULL;
    }
}

Combattant* choisir_cible_attaque(Combattant *equipe, int taille) {
    if (niveau_ia == 0) {
        return choisir_cible_aleatoire(equipe, taille);
    } else {
        return choisir_cible_faible(equipe, taille);
    }
}

Combattant* choisir_cible_soin(Combattant *equipe, int taille) {
    float min_ratio = 2.0f;
    int index = -1;
    for (int i = 0; i < taille; i++) {
        if (!equipe[i].est_KO && equipe[i].pv < equipe[i].pv_max) {
            float ratio = equipe[i].pv / equipe[i].pv_max;
            if (ratio < min_ratio) {
                min_ratio = ratio;
                index = i;
            }
        }
    }
    if (index >= 0) {
        return &equipe[index];
    } else {
        return NULL;
    }
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
        if (technique_disponible(ia) && (rand() % 2 == 0)) {
            return 2;
        }
        return 1;
    } else if (niveau_ia == 1) {
        if (technique_disponible(ia)) {
            return 2;
        } else {
            return 1;
        }
    } else {
        if ((ia->pv / ia->pv_max) < 0.3f) {
            for (int i = 0; i < MAX_TECHS; i++) {
                if (ia->techniques[i].effet == SOIN && ia->temps_recharge[i] <= 0) {
                    return 2;
                }
            }
        }
        if (technique_disponible(ia)) {
            return 2;
        } else {
            return 1;
        }
    }
}

void tour(Combattant *acteur, Combattant *equipe_ennemie, int taille_ennemie,
          Combattant *equipe_alliee, int taille_alliee, int est_joueur) {
    if (acteur->est_KO) {
        return;
    }
  /*      // Vérifier si le joueur est étourdi ou gelé
    if (est_incapacite(joueur)) {
        printf("⛔ %s est incapable d'agir ce tour (gelé ou étourdi) !\n", joueur->nom);
        mettreAJourEffets(joueur, NULL);
        appliquerDegats(joueur);  // Appliquer les dégâts même si le personnage ne peut pas agir
        return;
    }    */
    printf("\n🔁 Tour de %s\n", acteur->nom);
    int action;
    int tech_index = -1;
    Combattant *cible = NULL;
    if (est_joueur) {
        printf("1. Attaque normale\n");
        printf("2. Technique spéciale\n");
        printf("Choisissez votre action : ");
        scanf("%d", &action);
        while (action != 1 && action != 2) {
            printf("Choix invalide. Veuillez choisir 1 ou 2 : ");
            scanf("%d", &action);
        }
        if (action == 2) {
            printf("Choisissez une technique :\n");
            int tech_dispo[MAX_TECHS] = {-1, -1, -1};
            int nb_tech_dispo = 0;
            for (int i = 0; i < MAX_TECHS; i++) {
                if (acteur->temps_recharge[i] <= 0) {
                    tech_dispo[nb_tech_dispo] = i;
                    printf("%d. %s (", nb_tech_dispo + 1, acteur->techniques[i].nom);
                    if (acteur->techniques[i].effet == ATTAQUE) {
                    printf("ATTAQUE:%.1f", acteur->techniques[i].puissance);
                    } else if (acteur->techniques[i].effet == SOIN) {
                     printf("SOIN:%.1f", acteur->techniques[i].puissance);
                    } else if (acteur->techniques[i].effet == DEFENSE) {
                     printf("DEFENSE:%.1f", acteur->techniques[i].puissance);
                    } else if (acteur->techniques[i].effet == AGILITE) {
                    printf("AGILITE:%.1f", acteur->techniques[i].puissance);
                    } else if (acteur->techniques[i].effet == BRULURE) {
                        printf("BRULURE");
                    } else if (acteur->techniques[i].effet == GEL) {
                        printf("GEL");
                    } else if (acteur->techniques[i].effet == STUN) {
                        printf("STUN");
                    } else if (acteur->techniques[i].effet == POISON) {
                        printf("POISON");
                    } else {
                        printf("EFFET");
                    }
                    printf(")\n");
                    nb_tech_dispo++;
                }
            }
            if (nb_tech_dispo == 0) {
                printf("Aucune technique disponible ! Attaque normale à la place.\n");
                action = 1;
            } else {
                int choix;
                printf(">> ");
                scanf("%d", &choix);
                while (choix < 1 || choix > nb_tech_dispo) {
                    printf("Choix invalide. Veuillez choisir entre 1 et %d : ", nb_tech_dispo);
                    scanf("%d", &choix);
                }
                tech_index = tech_dispo[choix - 1];
            }
        }
        if (action == 2 && tech_index != -1) {
            if (acteur->techniques[tech_index].effet == SOIN) {
                printf("Choisissez un allié à soigner :\n");
                for (int i = 0; i < taille_alliee; i++) {
                    if (!equipe_alliee[i].est_KO) {
                        printf("%d. %s (PV: %.1f/%.1f)\n", 
                               i, equipe_alliee[i].nom, 
                               equipe_alliee[i].pv, 
                               equipe_alliee[i].pv_max);
                    }
                }
                int choix_cible;
                printf(">> ");
                scanf("%d", &choix_cible);
                while (choix_cible < 0 || choix_cible >= taille_alliee || 
                       equipe_alliee[choix_cible].est_KO) {
                    printf("Cible invalide. Réessayez : ");
                    scanf("%d", &choix_cible);
                }
                cible = &equipe_alliee[choix_cible];
            } else {
                cible = choisir_cible1(equipe_ennemie, taille_ennemie);

            }
        } else {
            cible = choisir_cible1(equipe_ennemie, taille_ennemie);
        }
    } else {
        action = choisir_action_ia(acteur);
        if (action == 2) {
            for (int i = 0; i < MAX_TECHS; i++) {
                if (acteur->temps_recharge[i] <= 0) {
                    tech_index = i;
                    break;
                }
            }
            if (tech_index == -1) {
                action = 1;
            }
        }
        if (action == 2 && tech_index != -1) {
            if (acteur->techniques[tech_index].effet == SOIN) {
                cible = choisir_cible_soin(equipe_alliee, taille_alliee);
                if (cible == NULL) {
                    action = 1;
                    cible = choisir_cible_attaque(equipe_ennemie, taille_ennemie);
                }
            } else {
                cible = choisir_cible_attaque(equipe_ennemie, taille_ennemie);
            }
        } else {
            cible = choisir_cible_attaque(equipe_ennemie, taille_ennemie);
        }
        printf("🤖 %s ", acteur->nom);
        if (action == 1) {
            printf("choisit d'attaquer %s\n", cible->nom);
        } else {
            printf("utilise %s sur %s\n", 
                   acteur->techniques[tech_index].nom, cible->nom);
        }
    }
    if (action == 1) {
        attaque_normale(acteur, cible);
    } else if (tech_index != -1) {
        utiliserTechnique(acteur, cible, &acteur->techniques[tech_index]);
    }
}

void maj_recharge(Combattant *c) {
    if (c == NULL) {
        return;
    }
    for (int i = 0; i < MAX_TECHS; i++) {
        if (c->temps_recharge[i] > 0) {
            c->temps_recharge[i]--;
        }
    }
}

Combattant* choisir_cible1(Combattant *equipe, int taille){
    int choix;
    int tentative = 0;
    int cible_valide = 0;
    do {
        if (tentative > 0) {
            printf("Cible invalide. Veuillez réessayer.\n");
        }
        printf("Choisissez une cible :\n");
        for (int i = 0; i < taille; i++) {
            if (!equipe[i].est_KO) {
                printf("%d. %s (PV: %.1f/%.1f)\n", 
                       i, equipe[i].nom, 
                       equipe[i].pv, 
                       equipe[i].pv_max);
            }
        }
        printf(">> ");
        scanf("%d", &choix);
        tentative++;
        if (choix >= 0 && choix < taille && !equipe[choix].est_KO) {
            cible_valide = 1;
        }
    } while (!cible_valide);
    
    return &equipe[choix];  // Retourner un pointeur vers le combattant choisi
}


void lancer_combat(Combattant *equipe_joueur, int taille_joueur, 
                   Combattant *equipe_ia, int taille_ia) {
    srand(time(NULL));
    printf("\nChoisissez la difficulté de l'IA :\n");
    printf("0. Noob\n");
    printf("1. Facile\n");
    printf("2. Moyen\n");
    printf(">> ");
    scanf("%d", &niveau_ia);
    while (niveau_ia < 0 || niveau_ia > 2) {
        printf("Choix invalide. Veuillez choisir entre 0 et 2 : ");
        scanf("%d", &niveau_ia);
    }
    printf("\n🔥⚔️ Début du combat !\n");
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
    int equipe_actuelle = rand() % 2;
    
    while (!equipe_est_KO(equipe_joueur, taille_joueur) && 
           !equipe_est_KO(equipe_ia, taille_ia)) {
        printf("\n=== Tour %d ===\n", tour_actuel++);
        
        printf("\n--- Votre équipe ---\n");
        for (int i = 0; i < taille_joueur; i++) {
            afficher_etat_combattant(&equipe_joueur[i]);
        }
        printf("\n--- Équipe ennemie ---\n");
        for (int i = 0; i < taille_ia; i++) {
            afficher_etat_combattant(&equipe_ia[i]);
        }
        
        if (equipe_actuelle == 0) {
            for (int i = 0; i < taille_joueur; i++) {
                if (!equipe_joueur[i].est_KO) {
                    tour(&equipe_joueur[i], equipe_ia, taille_ia, 
                         equipe_joueur, taille_joueur, 1);
                    if (equipe_est_KO(equipe_ia, taille_ia)) {
                        break;
                    }
                }
            }
            equipe_actuelle = 1;
        } else {
            for (int i = 0; i < taille_ia; i++) {
                if (!equipe_ia[i].est_KO) {
                    tour(&equipe_ia[i], equipe_joueur, taille_joueur,
                         equipe_ia, taille_ia, 0);
                    if (equipe_est_KO(equipe_joueur, taille_joueur)) {
                        break;
                    }
                }
            }
            equipe_actuelle = 0;
        }
        for (int i = 0; i < taille_joueur; i++) {
            maj_recharge(&equipe_joueur[i]);
        }
        for (int i = 0; i < taille_ia; i++) {
            maj_recharge(&equipe_ia[i]);
        }
    }
    printf("\n🎮 Le combat est terminé !\n");
    if (equipe_est_KO(equipe_joueur, taille_joueur)) {
        printf("💀 Votre équipe a été vaincue ! L'IA gagne.\n");
    } else {
        printf("🏆 Félicitations ! Vous avez vaincu l'équipe ennemie !\n");
    }
}
