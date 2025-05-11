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
    Combattant *cible = NULL;
    float min_pv = 9999;
    for (int i = 0; i < taille; i++) {
        if (!equipe[i].est_KO && equipe[i].pv < min_pv && equipe[i].pv > 0) {
            cible = &equipe[i];
            min_pv = equipe[i].pv;
        }
    }
    return cible;
}


int choisir_cible(Combattant *equipe, int taille) {
    int choix;
    for (int i = 0; i < taille; i++) {
        if (equipe[i].pv > 0){
            printf("%d. %s (PV: %.1f)\n", i, equipe[i].nom, equipe[i].pv);
        }
    }
    printf("Choisissez une cible : ");
    scanf("%d", &choix);
    while (choix < 0 || choix >= taille || equipe[choix].est_KO) {
        printf("⛔ Cible invalide. Réessayez : ");
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
    if (niveau_ia == 0) {
        return choisir_cible_aleatoire(equipe, taille);
    } else if (niveau_ia == 1) {
        return choisir_cible_faible(equipe, taille);
    } else {
        // niveau 2 : choisir celui avec le plus de dégâts potentiels, ou par défaut le plus faible
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
        if (c->temps_recharge[i] == 0)
            return i;
    }
    return -1;
}


int choisir_action_ia(Combattant *ia) {
    if (niveau_ia == 2 && technique_disponible(ia) != -1) {
        return 2;
    }
    return 1;
}


void attaque_elementaire(Combattant *attaquant, Combattant *cible) {
    // Appelle à l’attaque selon éléments
    printf("🔥 %s attaque %s !\n", attaquant->nom, cible->nom);
    // Appelle ta fonction de calcul selon l'élément ici
    attaque_normale(attaquant, cible); 
}


// Simule l’utilisation d’une technique spéciale 
void utiliser_technique(Combattant *source, Combattant *cible, int indice) {
    TechniqueSpeciale tech = source->techniques[indice];
    printf("✨ %s utilise %s !\n", source->nom, tech.nom);
    appliquerEffetElementaire(cible, tech);
    source->temps_recharge[indice] = tech.tours;
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
        cible = &equipe_ennemie[choisir_cible(equipe_ennemie, taille_ennemie)]; // En 1v1, une seule cible
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


int equipe_est_KO(Combattant *equipe, int taille) {
    for (int i = 0; i < taille; i++) {
        if (equipe[i].pv > 0){
            return 0;
        }
    }
    return 1;
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
    if (equipe_est_KO(equipe_joueur, taille_joueur)) {
    printf("💀 Vous êtes K.O. L'IA gagne !\n");
    } else {
    printf("🏆 Vous avez vaincu tous les ennemis !\n");
    }
}

