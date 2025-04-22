#include "effets.h"
#include <stdio.h>

//suppsons brulure, poisson, gel, stun, paralysie, aucuneffet etc etre une valeur de type Effet dans le fichier types.h
//supposons duree_effet ect dans Combattant dans types.h

//fct pour appliquer un effet elementaire
/*void appliquerEffetElementaire(Combattant *cible, Effetspecial effet) {
    // Appliquer l'effet élémentaire au combattant donc la cible
    switch (effet.type) {
        case BRULURE:
            break;
        case GEL: //renommer paralysie ?
            break;
        case STUN:
            break;
        default:
            printf("Effet inconnu !\n");
            break;
    }
}
VERSION NUMERO 1 
*/ 
// VERSION NUMERO 2
void appliquerEffetElementaire(Combattant *cible, Effetspecial effet) {
    cible->effets[effet.type] = effet.type;         // applique l'effet
    cible->duree_effet[effet.type] = effet.duree;   // applique la durée
    printf("%s subit l'effet %d pour %d tours !\n", cible->nom, effet.type, effet.duree);
}


//fct pour appliquer les degats à chaque tour 
void appliquerDegats(Combattant *c){
    switch (c->effet) {
        case POISON:
            c->pv -= 10; // Le poison inflige 10 PV de dégâts par tour
            printf("%s subit 10 points de dégâts à cause du poison !\n", c->nom);
            break;
        case BRULURE:
            c->pv -= 15; // La brûlure inflige 15 PV de dégâts par tour
            printf("%s est brûlé et perd 15 points de vie !\n", c->nom);
            break;
        case GEL:
            c->est_gelee = 1; // Le combattant est gelé et ne pourra pas agir pendant un tour
            printf("%s est gelé et ne pourra pas agir ce tour-ci !\n", c->nom);
            break;

        case AUCUN_EFFET: // Aucun effet à appliquer, donc rien à faire
            break;
    }
}

//fct pour mettre à jour les effets de tous les combattants
void mettreAJourEffets(Combattant *c) {
    // reduire la durée de tous les effets actifs et l'appliquer si necessaire
    for (int i = 0;i < NBEFFET; i++) { // on parcourt chaque effet actif du combattant
        if (c->effets[i] != AUCUNEFFET) { // si l'effet est actif alors :
            c->duree_effet[i]--; // on diminue la durée de l'effet d'un tour
            if (c->duree_effet[i] <= 0) { // si la durée de l'effet est fini alors :
                c->effets[i] = AUCUNEFFET; // on le supprime
                c->duree_effet[i] = 0; // on s'assure que la durée est à 0
            }
        }
    }
}


float multiplicateur(Effet e) {
    switch (e) {
        case BRULURE:
            return 1.2; // +20% de degats
        case GEL:
            return 0.8; // -20% de degats
        case POISON:
            return 1.1;
        case AUCUN_EFFET:
            return 1.0;        
        default:
            return 1.0; // pas de changement
    }
}
