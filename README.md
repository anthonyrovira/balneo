# balneo

Un projet Particle nommé balneo

## Description du projet

Il s'agit d'un système de gestion de la qualité d'air installée dans une salle d'eau. Le système relève la température, l'humidité et le taux de co2 dans l'air. Une ventilation de la pièce est assurée et régulée en fonction de l'humidité et du taux de co2 captés.
Le système possède également un afficheur OLED permettant d'afficher les valeurs issues des capteurs et un LED RGB en façade pour indiquer l'indice de Qualité d'Air Intérieure calculé.
Enfin, un capteur de présence permet d'analyser l'occupation de la pièce et de gérer la mise en veille de l'IHM.

## Notes de versions

`v2.1` : Mise à jour majeure => 20/12/2020

    - Revue globale du code et corrections de bugs mineurs
    - Création de la structure Timing.h pour la gestion des données de temporisations
    - Afficher les valeurs de capteurs sur l'écran lors d'une présence
    - Gérer la tempo des fonctions `fadingLed`
    - Gestion de la remise à zéro du compteur de présence.
    - Fonction de retour de qualité d'air (humidité et co2)
    - Calcul des temps de présence dans la pièce
    - Calcul des temps pour chaque changement d'état de la QAI dans la pièce

`v2.0` : Step up in progress version

    - Création de classes et d'objets pour organiser la structure du code.
    - Intégration d'un véritable workflow géré via un `switch` dans la boucle principale.
    - Données remontées sur le cloud particle : Température, Humidité, CO2, Présence, Compteur de présence.
    - Fonctions accessibles sur le cloud particle : Reset.

`v1.0` : Noob version

    - Un seul fichier main `.ino`
