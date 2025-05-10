#include "lecture.h"


// Supprimer le saut de ligne à la fin d'une chaîne
void supprimer_saut_ligne(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

// Conversion texte → EffetType
EffetType convertirEffetType(const char *effetStr) {
    if (strcmp(effetStr, "AUCUN") == 0) return AUCUN;
    if (strcmp(effetStr, "ATTAQUE") == 0) return ATTAQUE;
    if (strcmp(effetStr, "DEFENSE") == 0) return DEFENSE;
    if (strcmp(effetStr, "AGILITE") == 0) return AGILITE;
    if (strcmp(effetStr, "STUN") == 0) return STUN;
    if (strcmp(effetStr, "GEL") == 0) return GEL;
    if (strcmp(effetStr, "SOIN") == 0) return SOIN;
    if (strcmp(effetStr, "BRULURE") == 0) return BRULURE;
    if (strcmp(effetStr, "CONTRE") == 0) return CONTRE;
    if (strcmp(effetStr, "POISON") == 0) return POISON;
    return AUCUN;
}


    
int main() {
    FILE *fichier = fopen("fichier.txt", "r");  // <- NOM DE FICHIER MODIFIÉ ICI
    if (!fichier) {
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }
    Combattant persos[MAX_PERSOS];
    int nb_persos = 0;
    char ligne[100];

    while (fgets(ligne, sizeof(ligne), fichier) && nb_persos < MAX_PERSOS) {
        Combattant p;
        initialiser_combattant(&p); // Important pour zéro-initialiser

        // Nom
        sscanf(ligne, "Nom: %[^\n]", p.nom);
        supprimer_saut_ligne(p.nom);

        // Élément
        fgets(ligne, sizeof(ligne), fichier);
        sscanf(ligne, "Element: %[^\n]", ligne);
        supprimer_saut_ligne(ligne);

        if (strcmp(ligne, "FEU") == 0) p.element = FEU;
        else if (strcmp(ligne, "EAU") == 0) p.element = EAU;
        else if (strcmp(ligne, "TERRE") == 0) p.element = TERRE;
        else if (strcmp(ligne, "AIR") == 0) p.element = AIR;

        // Stats
        fscanf(fichier, "PV_max: %d\n", &p.pv_max);
        p.pv = p.pv_max; // pv actuels = max au début
        fscanf(fichier, "Attaque: %d\n", &p.attaque);
        fscanf(fichier, "Defense: %d\n", &p.defense);
        fscanf(fichier, "Agilite: %d\n", &p.agilite);
        fscanf(fichier, "Vitesse: %d\n", &p.vitesse);

        // Techniques
        for (int i = 0; i < MAX_TECHS; i++) {
            fgets(ligne, sizeof(ligne), fichier);
            sscanf(ligne, "Technique: %[^\n]", p.techniques[i].nom);
            supprimer_saut_ligne(p.techniques[i].nom);

            fgets(ligne, sizeof(ligne), fichier);
            sscanf(ligne, "Type: %[^\n]", p.techniques[i].type);
            supprimer_saut_ligne(p.techniques[i].type);

            fgets(ligne, sizeof(ligne), fichier);
            sscanf(ligne, "Puissance: %d", &p.techniques[i].puissance);

            fgets(ligne, sizeof(ligne), fichier);
            sscanf(ligne, "Portée: %d", &p.techniques[i].portee);

            fgets(ligne, sizeof(ligne), fichier);
            sscanf(ligne, "Tours: %d", &p.techniques[i].tours);

            fgets(ligne, sizeof(ligne), fichier);
            char effetStr[MAX_STR];
            sscanf(ligne, "Effet: %[^\n]", effetStr);
            supprimer_saut_ligne(effetStr);
            p.techniques[i].effet = convertirEffetType(effetStr);
        }

        persos[nb_persos++] = p;
    }

    fclose(fichier);

    // ✅ Vérification de chargement
    for (int i = 0; i < nb_persos; i++) {
        printf("\n>> %s (%d PV, Élément: %d)\n", persos[i].nom, persos[i].pv_max, persos[i].element);
        for (int j = 0; j < MAX_TECHS; j++) {
            TechniqueSpeciale *tech = &persos[i].techniques[j];
            printf("  - %s (%s) Puissance: %d | Portée: %d | Tours: %d | Effet: %d\n",
                   tech->nom, tech->type, tech->puissance, tech->portee, tech->tours, tech->effet);
        }
    }

    return 0;
}
