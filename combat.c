#include "combat.h"

// Fonction pour initialiser le combat
void initialiser_combat(Combattant *equipe1, int taille1, Combattant *equipe2, int taille2) {
    // Initialisation des compteurs de tour et des effets
    for (int i = 0; i < taille1; i++) {
        equipe1[i].est_KO = 0;
    }
    for (int i = 0; i < taille2; i++) {
        equipe2[i].est_KO = 0;
    }
}

// fct pour vérifier si une equipe est KO
int equipe_est_KO(Combattant *equipe, int taille) {
    for (int i = 0; i < taille; i++) {
        if (!equipe[i].est_KO) {
            return 0;  // L'equipe n'est pas KO
        }
    }
    return 1;  // L'equipe est KO
}

// Calcule les dégâts en fonction de l'attaque, la défense, et l'element
void attaque_normale(Combattant *attaquant, Combattant *cible) {
    int degats = (attaquant->attaque) - (cible->defense);
    if (degats < 0) {
        degats = 0; // minimum 0 dégâts
    }
    // Multiplie les dégâts selon le type d'element 
    int multipli = multiplicateur(attaquant->element, cible->element);
    degats *= multipli;
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
    if (joueur->est_KO){
        printf("⚠️ %s est KO et ne peut pas agir !\n", joueur->nom);
        return; // ne fait rien s'il est KO
    } 
    // Met à jour les effets temporaires comme la brûlure
    mettreAJourEffets(joueur);
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
    // Tant qu'une équipe n'est pas KO
    while (equipe_est_KO(equipe1, taille1) == 0 && equipe_est_KO(equipe2, taille2) == 0) {
        // S'il appartient à l'équipe 1 → il attaque l'équipe 2
        if (joueur != NULL) {
            // Vérification si le joueur appartient à l'équipe 1 ou 2
            if (joueur >= equipe1 && joueur < equipe1 + taille1) {
                // Le joueur est dans l'équipe 1, il attaque l'équipe 2
                effectuer_tour(joueur, equipe2, taille2);
            } else {
                // Le joueur est dans l'équipe 2, il attaque l'équipe 1
                effectuer_tour(joueur, equipe1, taille1);
            }
        }
    }
    // Fin du combat : affiche le gagnant
    if (equipe_est_KO(equipe1, taille1)) {
        printf("🏆 L'équipe 2 a gagné !\n");
    } else {
        printf("🏆 L'équipe 1 a gagné !\n");
    }
}
