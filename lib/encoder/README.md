# Gestion d'un encodeur rotatif par interruption

Stéphane Lepoutère (c) 2023

- Observe l'arrivée de changement d'état sur les 2 entrées (interruption)
- Gère le débounce

## TODO 2023-01-07 - structure de l'objet

pour une utilisation de plusieurs encodeurs, retirer les propriétés suivantes du type "static" pour utiliser un tableau de "this" et s'adresser aux objets adéquats suivant l'origine de l'interruption

## TODO 2023-01-07 - périmètre technique

gérer l'entrée du bouton du dessus

## Détection du sens de rotation

Merci à [Paul Stoffregen](https://github.com/PaulStoffregen/Encoder) pour l'idée de gestion des rotations sans beaucoup de tests

                               _______         _______       
                   Pin1 ______|       |_______|       |______ Pin1
      negative <---         _______         _______         __      --> positive
                    Pin2 __|       |_______|       |_______|   Pin2

|new|new|old|old|Résultat|
|:---:|:---:|:---:|:---:|:-------|
|A|B|A-|B-| |
|0|0|0|0|Pas de déplacement|
|0|0|0|1|+1|
|0|0|1|0|-1|
|0|0|1|1|+2  (Suppose un front sur la patte A seulement)|
|0|1|0|0|-1|
|0|1|0|1|Pas de déplacement|
|0|1|1|0|-2  (Suppose un front sur la patte A seulement)|
|0|1|1|1|+1|
|1|0|0|0|+1|
|1|0|0|1|-2  (Suppose un front sur la patte A seulement)|
|1|0|1|0|Pas de déplacement|
|1|0|1|1|-1|
|1|1|0|0|+2  (Suppose un front sur la patte A seulement)|
|1|1|0|1|-1|
|1|1|1|0|+1|
|1|1|1|1|Pas de déplacement|
