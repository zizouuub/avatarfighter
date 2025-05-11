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

int choisir_action_ia(Combattant *ia, Combattant *joueur) {
    if (niveau_ia == 0 || niveau_ia == 1) {
        return 1;  // Noob ou Facile = attaque normale
    } else if (niveau_ia == 2) {
        int tech = technique_disponible(ia);
        if (tech != -1){
            return 2;  // Technique dispo
        }
    }
    return 1;
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

void attaque_elementaire(Combattant *attaquant, Combattant *cible) {
    // Appelle à l’attaque selon éléments
    printf("🔥 %s attaque %s !\n", attaquant->nom, cible->nom);
    // Appelle ta fonction de calcul selon l'élément ici
    attaque_normale(attaquant, cible); 
}

// Simule l’utilisation d’une technique spéciale 
void utiliser_technique(Combattant *source, Combattant *cible, int indice_tech) {
    TechniqueSpeciale tech = source->techniques[indice_tech];
    printf("✨ %s utilise %s !\n", source->nom, tech.nom);
    appliquerEffetElementaire(cible, tech);
    source->temps_recharge[indice_tech] = tech.tours;
}

void tour(Combattant *joueur, Combattant *ia, int taille_adv, int est_joueur) {
    int action;
    if(joueur->est_KO){
        return;
    }
    printf("\n🔁 Tour de %s\n", joueur->nom);
    action = 1;
    if (est_joueur) {
        action = choisir_action_joueur();
    }else{
        action = choisir_action_ia(joueur, ia);
        printf("🤖 L’IA choisit l’action %d\n", action);
    }
    if (action == 1) {
        attaque_normale(joueur, &ia[cible]);
    } else {
        int tech = technique_disponible(acteur);
        if (tech != -1) {
            utiliser_technique(acteur, adversaire, tech_idx);
        } else {
            printf("❌ Aucune technique disponible ! Attaque normale...\n");
            attaque_elementaire(acteur, adversaire);
        }
    }
}

int equipe_est_KO(Combattant *equipe, int taille) {
    for (int i = 0; i < taille; i++) {
        if (equipe[i].pv > 0) return 0;
    }
    return 1;
}

void maj_recharge(Combattant *c) {
    for (int i = 0; i < MAX_TECHS; i++) {
        if (c->temps_recharge[i] > 0)
            c->temps_recharge[i]--;
    }
}

void lancer_combat(Combattant *joueur, Combattant *ia, int taille) {
    printf("\nChoisissez la difficulté IA (0 = noob, 1 = facile, 2 = moyen) : ");
    scanf("%d", &niveau_ia);
    printf("🔥⚔️ Début du combat entre %s et %s !\n", joueur->nom, ia->nom);
    while (!joueur->est_KO && !equipe_est_KO(equipe_ia, taille_ia)) {
        tour(joueur, ia, 1);
        if (ia->pv <= 0) {
            ia->est_KO = 1;
            return;
        }
        tour(ia, joueur, 0);
        if (joueur->pv <= 0) {
            joueur->est_KO = 1;
            return;
        }
         maj_recharge(joueur);
         maj_recharge(ia);
    }
    if (joueur->est_KO) {
        printf("💀 Vous êtes K.O. L'IA gagne !\n");
    } else {
        printf("🏆 Vous avez vaincu tous les ennemis !\n");
    }
}

