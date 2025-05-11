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

int choisir_cible(Combattant *equipe, int taille) {
    int choix;
    for (int i = 0; i < taille; i++) {
        if (equipe[i].pv > 0){
            printf("%d. %s (PV: %.1f)\n", i, equipe[i].nom, equipe[i].pv);
    }
    printf("Choisissez une cible : ");
    scanf("%d", &choix);
    while (choix < 0 || choix >= taille || equipe[choix].est_KO) {
        printf("⛔ Cible invalide. Réessayez : ");
        scanf("%d", &choix);
    }
    return choix;
}

int choisir_action_ia(Combattant ia) {
    if (ia.recharge == 0 && ia.pv < 30) {
        return 2; // tente une technique spéciale
    }
    return 1; // attaque simple
}

int choisir_cible_ia(Combattant *equipe, int taille) {
    if (niveau_ia == 0) {
        // IA noob → cible au hasard
        or (int i = 0; i < taille; i++) {
        if (equipe[i].pv > 0)
            return i; // première cible vivante
        }
    }
    return 0;
}

int technique_disponible(Combattant *c) {
    for (int i = 0; i < MAX_TECHS; i++) {
        if (c->temps_recharge[i] == 0)
            return i;
    }
    return -1;
}

// Simule l’utilisation d’une technique spéciale 
void utiliser_technique(Combattant *source, Combattant *cible, int indice_tech) {
    TechniqueSpeciale tech = source->techniques[indice_tech];
    printf("✨ %s utilise %s !\n", source->nom, tech.nom);
    appliquerEffetElementaire(cible, tech);
    source->temps_recharge[indice_tech] = tech.tours;
}

void tour(Combattant *joueur, Combattant *ia, int taille_adv, int est_joueur) {
    int action, cible;
    if(joueur->pv <= 0){
        return;
    }
    printf("\n🔁 Tour de %s\n", joueur->nom);
    if (est_joueur) {
        action = choisir_action_joueur();
        cible = choisir_cible(ia, taille_adv);
        if (action == 1) {
            attaque_elementaire(joueur, &ia[cible]);
    } else {
        action = choisir_action_ia(*ia);
        cible = choisir_cible_ia(joueur, taille_adv);
        printf("🤖 L'IA choisit l'action %d\n", action);
    }
    if (action == 1) {
        attaque_normale(joueur, &ia[cible]);
    } else {
        utiliser_technique(joueur, &ia[cible]);
    }
}

int equipe_est_KO(Combattant *equipe, int taille) {
    for (int i = 0; i < taille; i++) {
        if (equipe[i].pv > 0) return 0;
    }
    return 1;
}

void maj_recharge(Combattant *equipe, int taille) {
    for (int i = 0; i < taille; i++) {
        for (int j = 0; j < MAX_TECHS; j++) {
            if (equipe[i].temps_recharge[j] > 0)
                equipe[i].temps_recharge[j]--;
        }
    }
}

void lancer_combat(Combattant *joueur, Combattant *ia, int taille) {
    printf("\nChoisissez la difficulté IA (0 = noob, 1 = facile, 2 = moyen) : ");
    scanf("%d", &niveau_ia);
    printf("🔥⚔️ Début du combat entre %s et %s !\n", joueur->nom, ia->nom);
    while (!joueur->est_KO && !equipe_est_KO(equipe_ia, taille_ia)) {
        tour_combat(joueur, equipe_ia, taille_ia, 1);
        for (int i = 0; i < taille_ia; i++) {
            if (!equipe_ia[i].est_KO){
                tour_combat(&equipe_ia[i], &joueur, 1, 0);  // Taille = 1 car 1 joueur
            }
        }
        maj_recharge(&joueur, 1);
        maj_recharge(equipe_ia, taille_ia);
    }
    if (joueur->est_KO) {
        printf("💀 Vous êtes K.O. L'IA gagne !\n");
    } else {
        printf("🏆 Vous avez vaincu tous les ennemis !\n");
    }
}

