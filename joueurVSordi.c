#include"joueurVSordi.h"

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
    // Appelle à l’attaque selon éléments
    printf("🔥 %s attaque %s !\n", attaquant->nom, cible->nom);
    // Appelle ta fonction de calcul selon l'élément ici
    attaque_normale(attaquant, cible); 
}


// Simule l’utilisation d’une technique spéciale 
void utiliser_technique(Combattant *source, Combattant *cible, int indice) {
    if (indice >= 0 && indice < MAX_TECHS) {
        utiliserTechnique(source, cible, &source->techniques[indice]);
    }
}


void tour(Combattant *acteur, Combattant *equipe_ennemie, int taille_ennemie, Combattant *equipe_alliee, int taille_alliee, int est_joueur) {
    int action =1;
    int tech = -1;
    if (acteur->est_KO) {
        return;
    }
    printf("\n🔁 Tour de %s\n", acteur->nom);
    Combattant *cible = NULL;
    if (est_joueur) {
        action = choisir_action_joueur();
        cible = &equipe_ennemie[choisir_cible1(equipe_ennemie, taille_ennemie)]; // En 1v1, une seule cible
    }else{
        action = choisir_action_ia(acteur);
        if (action == 2) {
            tech = technique_disponible(acteur);
            if (tech != -1) {
                TechniqueSpeciale t = acteur->techniques[tech];
                if (t.effet == SOIN){
                    cible = choisir_cible_soin(equipe_alliee, taille_alliee);
                }else{
                    cible = choisir_cible_attaque(equipe_ennemie, taille_ennemie);
            } 
        }else {
            action = 1; 
            cible = choisir_cible_attaque(equipe_ennemie, taille_ennemie);
        }
    }else{
        cible = choisir_cible_attaque(equipe_ennemie, taille_ennemie);
    }
    printf("🤖 L’IA choisit l’action %d\n", action);
    }
    if (action == 1) {
        attaque_normale(acteur, cible);
    } else {
        utiliser_technique(acteur, cible, tech);
    }
}



void maj_recharge(Combattant *c) {
    for (int i = 0; i < MAX_TECHS; i++) {
        if (c->temps_recharge[i] > 0)
            c->temps_recharge[i]--;
    }
}


void lancer_combat(Combattant *equipe_joueur, int taille_joueur, Combattant *equipe_ia, int taille_ia) {
    printf("\nChoisissez la difficulté IA (0 = noob, 1 = facile, 2 = moyen) : ");
    scanf("%d", &niveau_ia);
    printf("🔥⚔️ Début du combat !\n");
    while (!equipe_est_KO(equipe_joueur, taille_joueur) && !equipe_est_KO(equipe_ia, taille_ia)) {
        for (int i = 0; i < taille_joueur; i++) {
            tour(&equipe_joueur[i], equipe_ia, taille_ia, equipe_joueur, taille_joueur, 1);
        }
        for (int i = 0; i < taille_ia; i++) {
            tour(&equipe_ia[i], equipe_joueur, taille_joueur, equipe_ia, taille_ia, 0);
        }
        for (int i = 0; i < taille_joueur; i++){
            maj_recharge(&equipe_joueur[i]);
        }
        for (int i = 0; i < taille_ia; i++){
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

