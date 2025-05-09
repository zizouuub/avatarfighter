#include "combat.h"

// Fonction pour initialiser le combat
void initialiser_combat(Combattant *equipe1, int taille1, Combattant *equipe2, int taille2) {
    // Initialisation des compteurs de tour et des effets
    for (int i = 0; i < taille1; i++) {
        initialiser_combattant(&equipe1[i]);
        equipe1[i].pv = equipe1[i].pv_max;
    }
    for (int i = 0; i < taille2; i++) {
        initialiser_combattant(&equipe2[i]);
        equipe2[i].pv = equipe2[i].pv_max;
    }
}

// fct pour vérifier si une equipe est KO
int equipe_est_KO(Combattant *equipe, int taille) {
    for (int i = 0; i < taille; i++) {
        if (!equipe[i].est_KO) { //choisir le premier qui n’est pas K.O
            return 0;  // L'equipe n'est pas KO
        }
    }
    return 1;  // L'equipe est KO
}

// Calcule les dégâts en fonction de l'attaque, la défense, et l'element
void attaque_normale(Combattant *attaquant, Combattant *cible) {
    float degats = (attaquant->attaque) - (cible->defense);
    if (degats < 0) {
        degats = 0; // minimum 0 dégâts
    }
    // Multiplie les dégâts selon le type d'element 
    float multipli = multiplicateur(attaquant->element, cible->element);
    degats = degats * multipli;
    // Applique les dégâts
    cible->pv -= degats;
    // Si la cible est à 0 PV ou moins, elle est KO
    if (cible->pv <= 0) {
        cible->pv = 0;
        cible->est_KO = 1;
        printf("❌ %s est KO !\n", cible->nom);
    } else {
        // Affiche le résultat du coup
        printf("💥 %s inflige %d dégâts à %s (reste %d PV)\n", attaquant->nom, degats, cible->nom, cible->pv);
    }
}


// Un joueur effectue une action pendant son tour
void effectuer_tour(Combattant *joueur, Combattant *adversaires, int taille_adversaires) {
    TechniqueSpeciale *tech = &joueur->techniques[0]; // par défaut la première techniques;
    if (joueur->est_KO){
        printf("⚠️ %s est KO et ne peut pas agir !\n", joueur->nom);
        return; // ne fait rien s'il est KO
    } 
    // verifier joueur etourdi ou gelé
    if (est_incapacite(joueur)) {
        printf("%s est incapable d'agir ce tour (gelé ou étourdi) !\n", joueur->nom);
        mettreAJourEffets(joueur, NULL); // NULL ou une technique fictive juste pour décrémenter
        return;
    }    
    // applique effets subis
    appliquerDegats(joueur, tech); //degats des effets 
    mettreAJourEffets(joueur, tech); // On peut choisir la technique à utiliser
    // Trouve la première cible ennemie encore vivante
    Combattant *cible = NULL;
    for (int i = 0; i < taille_adversaires; i++) {
        if (adversaires[i].est_KO == 0) { // Si le combattant n'est pas KO
            cible = &adversaires[i]; // On attribue la première cible vivante
        break;
    }
}
    // Si une cible existe, on l’attaque 
    if (cible != NULL) {
        attaque_normale(joueur, cible);
    }
}


// Gère l'ensemble du combat entre deux équipes
void boucle_de_combat(Combattant *equipe1, int taille1, Combattant *equipe2, int taille2) {
    printf("\n💥 🎮 Début du combat !\n");
    int tour = 1; // Compteur de tours
    // Tant qu'une équipe n'est pas KO
    while (equipe_est_KO(equipe1, taille1) == 0 && equipe_est_KO(equipe2, taille2) == 0) {
        printf("\n🔁 Tour %d\n", tour);
        // Tous les combattants de l’équipe 1 jouent
        for (int i = 0; i < taille1; i++) { 
            if (!equipe1[i].est_KO) { // Si le combattant n'est pas KO
                effectuer_tour(&equipe1[i], equipe2, taille2);
                }
            }
        // On vérifie si l'équipe 2 est éliminée avant de continuer
        if (equipe_est_KO(equipe2, taille2)){
            break; // Sort de la boucle si l'équipe 2 est KO
        }
        // Tous les combattants de l’équipe 2 jouent
        for (int i = 0; i < taille2; i++) {
            if (!equipe2[i].est_KO) {
                effectuer_tour(&equipe2[i], equipe1, taille1);
                }
            }
            tour++;
        }
    // Fin du combat : affiche le gagnant
    printf("\n🎯 Fin du combat !\n");
    if (equipe_est_KO(equipe1, taille1)) {
        printf("🏆 L'équipe 2 a gagné !\n");
    } else {
        printf("🏆 L'équipe 1 a gagné !\n");
    }
}
