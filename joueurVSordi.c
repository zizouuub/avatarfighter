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
        if (equipe[i].pv > 0)
            printf("%d. %s (PV: %.2f)\n", i, equipe[i].nom, equipe[i].pv);
    }
    printf("Choisissez une cible : ");
    scanf("%d", &choix);
    while (choix < 0 || choix >= taille || equipe[choix].pv <= 0) {
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
    for (int i = 0; i < taille; i++) {
        if (equipe[i].pv > 0)
            return i; // première cible vivante
    }
    return 0;
}


// Simule l’utilisation d’une technique spéciale (exemple simplifié)
void utiliser_technique(Combattant *source, Combattant *cible) {
    if (source->recharge > 0) {
        printf("⏳ %s est en recharge et ne peut pas utiliser de technique spéciale !\n", source->nom);
        attaque_normale(source, cible);
        return;
    }
    TechniqueSpeciale tech = source->techniques[0];  // prend la 1ère technique
    printf("%s utilise sa technique spéciale : %s\n", source->nom, tech.nom);
    appliquerEffetElementaire(cible, tech);
    source->recharge = tech.tour;
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
        if (equipe[i].recharge > 0)
            equipe[i].recharge--;
    }
}

void lancer_combat(Combattant *joueur, Combattant *ia, int taille) {
    printf("🔥⚔️ Début du combat entre %s et %s !\n", joueur->nom, ia->nom);
    while (joueur->pv > 0 && ia->pv > 0) {
       for (int i = 0; i < taille; i++) {
            if (joueur[i].pv > 0)
                tour_combat(&joueur[i], ia, taille, 1);
        }
    printf("\n=== 🤖 Tour de l'IA ===\n");
        for (int i = 0; i < taille; i++) {
            if (ia[i].pv > 0)
                tour_combat(&ia[i], joueur, taille, 0);
        }
    if (equipe_est_KO(ia, taille)) {
        printf("🏆 Vous avez vaincu tous les ennemis ! Victoire !\n");
        return;
        }
    if (equipe_est_KO(joueur, taille)) {
            printf("💀 Toute votre équipe est K.O... L'IA l'emporte !\n");
            return;
        }
        maj_recharge(joueur, taille);
        maj_recharge(ia, taille);
    }
}

