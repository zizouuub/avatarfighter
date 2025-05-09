#include "effets.h"
#include <stdio.h>

void initialiser_combattant(Combattant *c) {
    c->est_KO = 0;
    for (int i = 0; i < NB_EFFETS; i++) {
        c->effets[i] = AUCUN;
        c->duree_effet[i] = 0;
    }
}


//fct pour appliquer un effet elementaire direct à partir d une technique
void appliquerEffetElementaire(Combattant *cible, TechniqueSpeciale effet) {
    // Appliquer l'effet élémentaire au combattant donc la cible
    int duree = 1; // Durée de l'effet (1 tour par défaut)
    printf("%s subit l'effet de type %s pendant %d tours !\n", cible->nom, effet.type, duree);
    switch (effet.effet) {
        case BRULURE:
            break;
        case GEL: 
            break;
        case STUN:
            break;
        case POISON:
            break;
        //effet imediat 
        case SOIN:
            cible->pv_max += effet.effet;
            printf("%s récupère %d PV !\n", cible->nom, effet.effet);
            break;
        default:
            printf("Effet inconnu !\n");
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
void appliquerDegats(Combattant *c, TechniqueSpeciale *tech) {
    for (int i = 0; i < NB_EFFETS; i++) { // On parcourt chaque effet actif du combattant
        switch ( c->effets[i]){ // On applique l'effet de la première case (peut être amélioré pour gérer plusieurs effets)
            case POISON:
                c->pv -= 10; // Le poison inflige 10 PV de dégâts par tour
                printf("%s subit 10 points de dégâts à cause du poison !\n", c->nom);
                break;
            case BRULURE:
                c->pv -= 15; // La brûlure inflige 15 PV de dégâts par tour
                printf("%s est brûlé et perd 15 points de vie !\n", c->nom);
                break;
            case ATTAQUE:
                if (tech != NULL) {
                    int degats = tech->puissance;
                    // Bonus si la puissance est élevée
                    if (tech->puissance > 50){
                        degats += 10;
                    }else if (tech->puissance > 100){
                        degats += 20;
                    }
                    c->pv -= degats;
                    printf("%s subit %d points de dégâts de l'attaque spéciale '%s' !\n", c->nom, degats, tech->nom);
                }
                break;
            case GEL:
                // Le combattant est gelé et ne pourra pas agir pendant un tour
                printf("%s est gelé et ne pourra pas agir ce tour-ci !\n", c->nom);
                break;
            case STUN:
                // Le combattant est étourdi et ne pourra pas agir pendant un tour
                printf("%s est étourdi et ne pourra pas agir ce tour-ci !\n", c->nom);
                break;
            case AUCUN : // Aucun effet à appliquer, donc rien à faire
                break;
            }
        if (c->pv <= 0) {
            c->pv = 0;
            c->est_KO = 1;
            printf("%s est K.O. !\n", c->nom);
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
    // On applique les effets de la technique spéciale
    if (tech!=NULL && tech->effet != AUCUN) { // Si la technique a un effet
        appliquerEffetElementaire(c, *tech); // Appliquer l'effet de la technique
    }
    // reduire la durée de tous les effets actifs et l'appliquer si necessaire
    for (int i = 0; i < NB_EFFETS; i++) { // on parcourt chaque effet actif du combattant
        if (c->effets[i] != AUCUN) { // si l'effet est actif alors :
            c->duree_effet[i]--; // on diminue la durée de l'effet d'un tour
            if (c->duree_effet[i] <= 0) { // si la durée de l'effet est fini alors :
                c->effets[i] = AUCUN; // on le supprime
                c->duree_effet[i] = 0; // on s'assure que la durée est à 0
            }
        }
    }
}

/*// Retourne un multiplicateur de dégâts selon l’effet
float multiplicateur(EffetType e) {
    switch (e) {
        case BRULURE:
            return 1.2; // +20% de degats
        case GEL:
            return 0.8; // -20% de degats
        case STUN:
            return 0.8; // -20% de degats
        case POISON:
            return 1.1;
        case AUCUN:
            return 1.0;        
        default:
            return 1.0; // pas de changement
    }
} */

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
