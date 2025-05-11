#include "lecture.h"

// Supprime le saut de ligne à la fin d'une chaîne
void supprimer_saut_ligne(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

// Convertit une chaîne en EffetType
EffetType convertirEffetType(const char *effetStr) {
    // Comparaison des chaînes pour déterminer l'effet
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
    return AUCUN; // Valeur par défaut si aucune correspondance
}

// Charge les combattants depuis un fichier
int charger_combattants(const char *nom_fichier, Combattant *persos, int max_persos) {
    // Ouverture du fichier en mode lecture
    FILE *fichier = fopen(nom_fichier, "r");

    // Vérification si le fichier est ouvert correctement
    if (!fichier) {
        printf("Erreur lors de l'ouverture du fichier");
        return 1; // Retourne 1 en cas d'erreur
    }

    int nb_persos = 0; // Compteur de personnages chargés
    char ligne[256]; // Buffer pour la lecture des lignes

    // Lecture du fichier ligne par ligne
    while (fgets(ligne, sizeof(ligne), fichier) && nb_persos < MAX_PERSOS) {
        Combattant p;
        initialiser_combattant(&p);

        // Lecture du nom du personnage
        sscanf(ligne, "Nom: %[^\n]", p.nom);
        supprimer_saut_ligne(p.nom);

        // Lecture de l'élément du personnage
        fgets(ligne, sizeof(ligne), fichier);
        sscanf(ligne, "Element: %[^\n]", ligne);
        supprimer_saut_ligne(ligne);
        // Conversion de l'élément en enum
        if (strcmp(ligne, "FEU") == 0) p.element = FEU;
        else if (strcmp(ligne, "EAU") == 0) p.element = EAU;
        else if (strcmp(ligne, "TERRE") == 0) p.element = TERRE;
        else if (strcmp(ligne, "AIR") == 0) p.element = AIR;

        // Lecture des statistiques du personnage
        fscanf(fichier, "PV_max: %f\n", &p.pv_max);
        p.pv = p.pv_max; // Initialisation des PV courants
        fscanf(fichier, "Attaque: %d\n", &p.attaque);
        fscanf(fichier, "Defense: %d\n", &p.defense);
        fscanf(fichier, "Agilite: %d\n", &p.agilite);
        fscanf(fichier, "Vitesse: %d\n", &p.vitesse);

        // Lecture des techniques du personnage
        for (int i = 0; i < MAX_TECHS; i++) {
            // Lecture du nom de la technique
            fgets(ligne, sizeof(ligne), fichier);
            sscanf(ligne, "Technique: %[^\n]", p.techniques[i].nom);
            supprimer_saut_ligne(p.techniques[i].nom);

            // Lecture du type de technique
            fgets(ligne, sizeof(ligne), fichier);
            sscanf(ligne, "Type: %[^\n]", ligne);
            supprimer_saut_ligne(ligne);
            p.techniques[i].effet = convertirEffetType(ligne);

            // Lecture des caractéristiques de la technique
            fgets(ligne, sizeof(ligne), fichier);
            sscanf(ligne, "Puissance: %d", &p.techniques[i].puissance);

            fgets(ligne, sizeof(ligne), fichier);
            sscanf(ligne, "Portée: %d", &p.techniques[i].portee);

            fgets(ligne, sizeof(ligne), fichier);
            sscanf(ligne, "Tours: %d", &p.techniques[i].tours);

            // Lecture de la description de la technique
            fgets(ligne, sizeof(ligne), fichier);
            sscanf(ligne, "Description: %[^\n]", p.techniques[i].description);
            supprimer_saut_ligne(p.techniques[i].description);
        }

        // Ajout du personnage au tableau
        persos[nb_persos++] = p;
        // Lecture de la ligne vide entre les personnages
        fgets(ligne, sizeof(ligne), fichier);
    }

    // Fermeture du fichier
    fclose(fichier);
    // Retourne le nombre de personnages chargés
    return nb_persos;
}
