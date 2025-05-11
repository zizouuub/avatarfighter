#include "effets.h"

// tableau global des noms d'effets pour l'affichage
const char* nomEffets[] = {
    "Aucun", "Attaque", "Défense", "Agilité", "Stun", "Gel", "Soin", "Brûlure", "Contre", "Poison"
};


//initialiser un combattant avec des valeurs 
void initialiser_combattant(Combattant *c) {
    // securite contre les poiteurs invalides
    if(c==NULL){ 
        return;
    } // initialiser des valeurs de base 
    c->est_KO = 0; // le combattant n'est pas KO
    c->prochain_tour = 0; // pret à jouer
    //Initialisation des temps de recharge des techniques
    for (int i = 0; i < MAX_TECHS; i++) { 
        c->temps_recharge[i] = 0; // Aucune technique en recharge
    } 
    // Initialisation des effets actifs
    for (int i = 0; i < NB_EFFETS; i++) {
        c->effets[i] = AUCUN;
        c->duree_effet[i] = 0;
    }
}


//fct pour appliquer un effet elementaire direct à partir d une technique
void appliquerEffetElementaire(Combattant *cible, TechniqueSpeciale effet) {
    // Vérification de la validité de la cible
    if (cible == NULL || cible->est_KO){
        return ;
    }
    // Appliquer l'effet élémentaire au combattant donc la cible
    int duree = effet.tours; // Durée de l'effet (1 tour par défaut)
    // Affichage informatif de l'effet
    if (effet.effet >= 0 && effet.effet < sizeof(nomEffets) / sizeof(char *)) {
        printf("%s subit l'effet de type %s pendant %d tours !\n", cible->nom, nomEffets[effet.effet], duree);
    } else {
        printf("%s subit un effet inconnu pendant %d tours !\n", cible->nom, duree);
    }
    // Traitement des effets immédiats
    switch (effet.effet) {
        // effet imediat
        case SOIN:
            cible->pv += effet.puissance;
            printf("%s récupère %.2f PV !\n", cible->nom, effet.puissance);
            // On ne dépasse pas les PV max
            if (cible->pv > cible->pv_max){
                cible->pv = cible->pv_max;
            }
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
    if (c == NULL){ // securité 
        return;
    }
    for (int i = 0; i < NB_EFFETS; i++) { // On parcourt chaque effet actif du combattant
        if (c->effets[i] == BRULURE && c->duree_effet[i] > 0) {
            float degats = c->pv_max * 0.05; // 5% des PV max
            c->pv -= degats;
            printf("🔥 %s subit %.1f points de dégâts de brûlure !\n", c->nom, degats);
            c->duree_effet[i]--;
        }else if (c->effets[i] == POISON && c->duree_effet[i] > 0) {
            float degats = c->pv_max * 0.03; // 3% des PV max
            c->pv -= degats;
            printf("☠️ %s subit %.1f points de dégâts de poison !\n", c->nom, degats);
            c->duree_effet[i]--;
        switch (c->effets[i]) { // On applique l'effet de la première case (peut être amélioré pour gérer plusieurs effets)
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
            case SOIN:
            case AUCUN: // Aucun effet à appliquer, donc rien à faire
            default:
                // Aucun effet de dégâts à appliquer
                break;
        }
        if (c->pv <= 0) {
            c->pv = 0;
            c->est_KO = 1;
            printf("%s est K.O. !\n", c->nom);
            break; // on sort de la boucle combattant KO
        }
    }
}


//fct pour mettre à jour les effets de tous les combattants
void mettreAJourEffets(Combattant *c, TechniqueSpeciale *tech) {
     if (c == NULL){
         return;
     }
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
    }
    // Réduire la durée de tous les effets actifs et appliquer si nécessaire
    for (int i = 0; i < NB_EFFETS; i++) {
        if (c->effets[i] != AUCUN && c->duree_effet[i] > 0) {
            if (c->effets[i] == GEL) {
                printf("❄️ %s est gelé et ne peut pas agir ! (%d tours restants)\n", 
                      c->nom, c->duree_effet[i]);
                c->duree_effet[i]--;
            }
            else if (c->effets[i] == STUN) {
                printf("💫 %s est étourdi et ne peut pas agir ! (%d tours restants)\n", 
                      c->nom, c->duree_effet[i]);
                c->duree_effet[i]--;
            }
            // On ne décrémente pas la durée ici pour BRULURE et POISON
            // car on veut qu'ils fassent des dégâts avant de disparaître
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


// fct pour exécute une attaque élémentaire de base
void attaque_elementaire(Combattant *attaquant, Combattant *cible, TechniqueSpeciale *tech) {
    // securité 
    if (attaquant == NULL || cible == NULL || tech == NULL){
        return;
    }
    if (attaquant->est_KO || cible->est_KO){
        return;
    }
    // verification temps de recharge
    if (attaquant->temps_recharge > 0) {
        printf("%s est en recharge !\n", attaquant->nom);
        return;
    }
    // Calcul des dégâts avec multiplicateur élémentaire
    float mult = multiplicateur(attaquant->element, cible->element); // Utilise la fonction existante
    float degats = tech->puissance * mult;
    // Application des dégâts
    cible->pv -= degats;
    if (cible->pv < 0){ // PV ne peuvent pas être négatifs
        cible->pv = 0;
    }
    printf("%s utilise %s sur %s infligeant %.2f dégâts (pv restants : %f)\n",
           attaquant->nom, tech->nom, cible->nom, degats, cible->pv);
    // Mise en recharge de la technique
    attaquant->temps_recharge[2] = tech->tours;
}


//vérifie si un combattant est incapable d'agir (gelé ou étourdi)
int est_incapacite(Combattant *c) {
    if ( c== NULL){
        return 0;
    } 
    // Recherche d'effets incapacitants actifs
    for (int i = 0; i < NB_EFFETS; i++) {
        if ((c->effets[i] == GEL || c->effets[i] == STUN) && c->duree_effet[i] > 0) {
            return 1; // Incapacité active
        }
    }
    return 0;
}



// Fonction qui utilise l'agilité pour déterminer si l'esquive réussit
int tentativeEsquive(Combattant *defenseur) {
    // Vérifications de sécurité
    if (defenseur == NULL || defenseur->est_KO){
        return 0;
    }
    // Calcul de la chance d'esquive (5% par point d'agilité)
    int chance = defenseur->agilite * 5;
    int tirage = rand() % 100; //tirage aleatoire
    return tirage < chance; // comparaison avec seuil 
}
