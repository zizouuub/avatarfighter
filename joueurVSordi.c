#include <stdio.h>
#include <string.h>
#include "combat.h"  // Pour attaque_normale
#include "effets.h"  // Pour appliquerEffetElementaire

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

int choisir_action_ia(Combattant ia) {
    if (ia.pv < 30) {
        return 2; // tente une technique spéciale
    }
    return 1; // attaque simple
}

// Simule l’utilisation d’une technique spéciale (exemple simplifié)
void utiliser_technique(Combattant *source, Combattant *cible) {
    TechniqueSpeciale tech = source->techniques[0];  // prend la 1ère technique
    printf("%s utilise sa technique spéciale : %s\n", source->nom, tech.nom);
    appliquerEffetElementaire(cible, tech);
    attaque_normale(source, cible);  // ajoute des dégâts en plus si tu veux
}

void tour(Combattant *joueur, Combattant *ia, int est_joueur) {
    int action;
    if (est_joueur) {
        action = choisir_action_joueur();
    } else {
        action = choisir_action_ia(*ia);
        printf("🤖 L'IA choisit l'action %d\n", action);
    }
    switch (action) {
        case 1:
            attaque_normale(joueur, ia);
            break;
        case 2:
            utiliser_technique(joueur, ia);
            break;
        default:
            printf("⛔ Action inconnue.\n");
            break;
    }
}

void lancer_combat(Combattant *joueur, Combattant *ia) {
    printf("🔥 Combat entre %s et %s !\n", joueur->nom, ia->nom);
    while (joueur->pv > 0 && ia->pv > 0) {
        // Tour du joueur
        tour(joueur, ia, 1);
        if (ia->pv > 0) { // Si l'IA est toujours en vie, son tour
            tour(ia, joueur, 0);
        }
    }
    if (joueur->pv <= 0) {
        printf("💀 Vous êtes K.O. !!🤖 L'IA a gagné !\n");
    } else {
        printf("🏆 Vous avez gagné !\n");
    }
}

