# balneo

Un projet Particle nommé balneo

## Description du projet

Il s'agit d'un système de gestion de la qualité d'air installée dans une salle d'eau. Le système relève la température, l'humidité et le taux de co2 dans l'air. Une ventilation de la pièce est assurée et régulée en fonction de l'humidité et du taux de co2 captés.
Le système possède également un afficheur OLED permettant d'afficher les valeurs issues des capteurs et un LED RGB en façade pour indiquer l'indice de Qualité d'Air Intérieure calculé.

## Notes de versions

`v2.0` : Step up in progress version

    - Création de classes et d'objets pour organiser la structure du code.
    - Intégration d'un véritable workflow géré via un `switch` dans la boucle principale.
    - Données remontées sur le cloud particle : Température, Humidité, CO2, Présence, Compteur de présence.
    - Fonctions accessibles sur le cloud particle : Reset.

`v1.0` : Noob version

    - Un seul fichier main `.ino`
