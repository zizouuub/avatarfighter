#include "effets.h"

const char* nomEffets[] = {
    "Aucun", "Attaque", "Défense", "Agilité", "Stun", "Gel", "Soin", "Brûlure", "Contre", "Poison"
};
void initialiser_combattant(Combattant *c) {
    c->est_KO = 0;
    c->prochain_tour = 0;
    for (int i = 0; i < MAX_TECHS; i++) {
        c->temps_recharge[i] = 0;
    }
    for (int i = 0; i < NB_EFFETS; i++) {
        c->effets[i] = AUCUN;
        c->duree_effet[i] = 0;
    }
}



//fct pour appliquer un effet elementaire direct à partir d une technique
void appliquerEffetElementaire(Combattant *cible, TechniqueSpeciale effet) {
    // Appliquer l'effet élémentaire au combattant donc la cible
    int duree = effet.tours; // Durée de l'effet (1 tour par défaut)
    if (effet.effet >= 0 && effet.effet < sizeof(nomEffets) / sizeof(char *)) {
        printf("%s subit l'effet de type %s pendant %d tours !\n", cible->nom, nomEffets[effet.effet], duree);
    } else {
        printf("%s subit un effet inconnu pendant %d tours !\n", cible->nom, duree);
    }
    switch (effet.effet) {
        // effet imediat
        case SOIN:
            cible->pv += effet.puissance;
            printf("%s récupère %d PV !\n", cible->nom, effet.puissance);
            if (cible->pv > cible->pv_max) cible->pv = cible->pv_max;
            break;
        default:
            break;
    }
// Ajout dans appliquerEffetElementaire :
    for (int i = 0; i < NB_EFFETS; i++) {
        if (cible->effets[i] == AUCUN) {
            cible->effets[i] = effet.effet;
            cible->duree_effet[i] = effet.tours;
            break;
        }
    }
}


//fct pour appliquer les degats à chaque tour 
void appliquerDegats(Combattant *c) {
    for (int i = 0; i < NB_EFFETS; i++) { // On parcourt chaque effet actif du combattant
        switch (c->effets[i]) { // On applique l'effet de la première case (peut être amélioré pour gérer plusieurs effets)
            case POISON: // Le poison inflige 10 PV de dégâts par tour
                c->pv -= 10;
                printf("%s subit 10 points de dégâts à cause du poison !\n", c->nom);
                break;
            case BRULURE:  // La brûlure inflige 15 PV de dégâts par tour
                c->pv -= 15;
                printf("%s est brûlé et perd 15 points de vie !\n", c->nom);
                break;
            case GEL:
                // Le combattant est gelé et ne pourra pas agir pendant un tour
                printf("%s est gelé et ne pourra pas agir ce tour-ci !\n", c->nom);
                break;
            case STUN:
                // Le combattant est étourdi et ne pourra pas agir pendant un tour
                printf("%s est étourdi et ne pourra pas agir ce tour-ci !\n", c->nom);
                break;
            case ATTAQUE:
            case DEFENSE:
            case AGILITE:
            case SOIN:
            case CONTRE:
            case AUCUN: // Aucun effet à appliquer, donc rien à faire
            default:
                // Aucun effet de dégâts à appliquer
                break;
        }
        if (c->pv <= 0) {
            c->pv = 0;
            c->est_KO = 1;
            printf("%s est K.O. !\n", c->nom);
            break;
        }
    }
}


//fct pour mettre à jour les effets de tous les combattants
void mettreAJourEffets(Combattant *c, TechniqueSpeciale *tech) {
    // On vérifie si le combattant est KO
    if (c->est_KO) {
        printf("%s est KO et ne peut pas agir !\n", c->nom);
        return; // Le combattant est KO, on ne fait rien
    }

    // Appliquer les effets de la technique spéciale
    if (tech != NULL && tech->effet != AUCUN) {
        // Définir la durée de l'effet basée sur la portée
        for (int i = 0; i < NB_EFFETS; i++) {
            if (c->effets[i] == AUCUN) {  // Si la case d'effet est vide
                c->effets[i] = tech->effet;       // Appliquer l'effet
                c->duree_effet[i] = tech->portee; // La durée de l'effet est égale à la portée
                break;  // Sortir de la boucle après avoir appliqué l'effet
            }
        }
        appliquerEffetElementaire(c, *tech); // Appliquer l'effet spécifique à l'élément
    }

    // Réduire la durée de tous les effets actifs et appliquer si nécessaire
    for (int i = 0; i < NB_EFFETS; i++) {
        if (c->effets[i] != AUCUN) {  // Si l'effet est actif
            c->duree_effet[i]--;  // On diminue la durée de l'effet d'un tour
            if (c->duree_effet[i] <= 0) {  // Si l'effet est terminé
                printf("L'effet %s sur %s prend fin.\n", nomEffets[c->effets[i]], c->nom);
                c->effets[i] = AUCUN;  // Supprimer l'effet
                c->duree_effet[i] = 0;  // Remettre la durée à 0
            }
        }
    }
}



// Retourne un multiplicateur de dégâts selon l'élément
float multiplicateur(Element attaquant, Element defenseur) {
    if (attaquant == FEU && defenseur == TERRE){ 
        return 1.5;
    }
    if (attaquant == EAU && defenseur == FEU){ 
        return 1.5;
    }
    if (attaquant == TERRE && defenseur == AIR){ 
        return 1.5;
    }
    if (attaquant == AIR && defenseur == TERRE){ 
        return 1.5 ;
    }
    if (attaquant == defenseur){ 
        return 1;
    }
    return 1; // Par défaut
}


int est_incapacite(Combattant *c) {
    for (int i = 0; i < NB_EFFETS; i++) {
        if ((c->effets[i] == GEL || c->effets[i] == STUN) && c->duree_effet[i] > 0) {
            return 1; // Incapacité active
        }
    }
    return 0;
}



// Fonction qui utilise l'agilité pour déterminer si l'esquive réussit
int tentativeEsquive(Combattant *defenseur) {
    int chance = defenseur->agilite * 5;
    int tirage = rand() % 100;
    return tirage < chance;
}

