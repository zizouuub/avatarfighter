#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "personnages.h"  // Inclusion du fichier .h contenant les définitions des structures

// Supprimer le saut de ligne à la fin d'une chaîne
void supprimer_saut_ligne(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

int main() {
    // Ouverture du fichier contenant les personnages
    FILE *fichier = fopen("personnages.txt", "r");
    if (!fichier) {
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }

    Combattant persos[MAX_PERSOS];
    int nb_persos = 0;
    char ligne[100];

    // Lecture du fichier ligne par ligne pour extraire les données
    while (fgets(ligne, sizeof(ligne), fichier) && nb_persos < MAX_PERSOS) {
        Combattant p;

        // Lecture du nom du personnage
        sscanf(ligne, "Nom: %[^\n]", p.nom);
        supprimer_saut_ligne(p.nom);  // Supprimer le saut de ligne

        // Lecture de l'élément du personnage
        fgets(ligne, sizeof(ligne), fichier);
        sscanf(ligne, "Element: %[^\n]", ligne);
        supprimer_saut_ligne(ligne);

        // Conversion de l'élément string en enum Element
        if (strcmp(ligne, "FEU") == 0) p.element = FEU;
        else if (strcmp(ligne, "EAU") == 0) p.element = EAU;
        else if (strcmp(ligne, "TERRE") == 0) p.element = TERRE;
        else if (strcmp(ligne, "AIR") == 0) p.element = AIR;

        // Lecture des statistiques du personnage
        fscanf(fichier, "PV_max: %d\n", &p.pv_max);
        fscanf(fichier, "Attaque: %d\n", &p.attaque);
        fscanf(fichier, "Defense: %d\n", &p.defense);
        fscanf(fichier, "Agilite: %d\n", &p.agilite);
        fscanf(fichier, "Vitesse: %d\n", &p.vitesse);

        // Lecture des techniques spéciales
        for (int i = 0; i < MAX_TECHS; i++) {
            fgets(ligne, sizeof(ligne), fichier);
            sscanf(ligne, "Technique: %[^\n]", p.techniques[i].nom);
            supprimer_saut_ligne(p.techniques[i].nom);

            fgets(ligne, sizeof(ligne), fichier);
            sscanf(ligne, "Type: %[^\n]", p.techniques[i].type);
            supprimer_saut_ligne(p.techniques[i].type);

            fscanf(fichier, "Puissance: %d\n", &p.techniques[i].puissance);
            fscanf(fichier, "Portée: %d\n", &p.techniques[i].portee);
            fscanf(fichier, "Tours: %d\n", &p.techniques[i].tours);
        }

        // Ajouter le personnage au tableau
        persos[nb_persos++] = p;
    }

    fclose(fichier);
}
return 0;
