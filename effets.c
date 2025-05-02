#include "effets.h"
#include <stdio.h>


//fct pour appliquer un effet elementaire direct à partir d une technique
void appliquerEffetElementaire(Combattant *cible, Effetspecial effet) {
    // Appliquer l'effet élémentaire au combattant donc la cible
    printf("%s subit l'effet %s (type %d) pendant %d tours !\n", cible->nom, effet.nom, effet.effet, effet.duree);
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
            cible->pv_max += effet.valeur;
            printf("%s récupère %d PV !\n", cible->nom, effet.valeur);
            break;
        default:
            printf("Effet inconnu !\n");
            break;
    }
}



//fct pour appliquer les degats à chaque tour 
void appliquerDegats(Combattant *c){
    switch (c->EffetType) {
        case POISON:
            c->pv_max -= 10; // Le poison inflige 10 PV de dégâts par tour
            printf("%s subit 10 points de dégâts à cause du poison !\n", c->nom);
            break;
        case BRULURE:
            c->pv_max -= 15; // La brûlure inflige 15 PV de dégâts par tour
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
        case AUCUN_EFFET: // Aucun effet à appliquer, donc rien à faire
            break;
        if (c->pv_max <= 0) {
            c->pv_max = 0;
            c->est_KO = 1;
            printf("%s est K.O. !\n", c->nom);
        }
    }
}

//fct pour mettre à jour les effets de tous les combattants
void mettreAJourEffets(Combattant *c, TechniqueSpeciale *tech) {
    int nbeffet = 13; // nombre d'effets possibles
    // On vérifie si le combattant est KO
    if (c->est_KO) {
        printf("%s est KO et ne peut pas agir !\n", c->nom);
        return; // Le combattant est KO, on ne fait rien
    }
    // On applique les effets de la technique spéciale
    if (tech->effet != AUCUN) {
        appliquerEffetElementaire(c, *tech); // Appliquer l'effet de la technique spéciale
    }
    // On applique les effets de l'élément 
    if (c->element == FEU) {
        appliquerEffetElementaire(c, *tech); // Appliquer l'effet de l'élément feu
    }
    if (tech->element == EAU) {
        appliquerEffetElementaire(c, *tech); // Appliquer l'effet de l'élément eau
    }
    if (tech->element == TERRE) {
        appliquerEffetElementaire(c, *tech); // Appliquer l'effet de l'élément terre
    }
    if (tech->element == AIR) {
        appliquerEffetElementaire(c, *tech); // Appliquer l'effet de l'élément air 
    }
   
    // reduire la durée de tous les effets actifs et l'appliquer si necessaire
    for (int i = 0;i < nbeffet; i++) { // on parcourt chaque effet actif du combattant
        if (c->effets[i] != AUCUNEFFET) { // si l'effet est actif alors :
            c->duree_effet[i]--; // on diminue la durée de l'effet d'un tour
            if (c->duree_effet[i] <= 0) { // si la durée de l'effet est fini alors :
                c->effets[i] = AUCUNEFFET; // on le supprime
                c->duree_effet[i] = 0; // on s'assure que la durée est à 0
            }
        }
    }
}


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
        case AUCUN_EFFET:
            return 1.0;        
        default:
            return 1.0; // pas de changement
    }
}
