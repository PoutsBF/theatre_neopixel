/******************************************************************************
    Gestion d'un encodeur rotatif                   Stéphane Lepoutère (c) 2020

    Observe l'arrivée de changement d'état sur les 2 entrées (interruption)
    Gère le débounce
*/

#include <Arduino.h>
#include <encoder.h>

// Initialisation des variables statiques
uint8_t encoder::BP0_pin;
uint8_t encoder::BP1_pin;

int8_t encoder::decalage;

/// @brief Création de l'objet
/// @param BP0 patte A de l'encodeur
/// @param BP1 patte B de l'encodeur
encoder::encoder(uint8_t BP0, uint8_t BP1)
{
    // Fige les pattes
    BP0_pin = BP0;
    BP1_pin = BP1;
    // Configure les interruptions
    attachInterrupt(digitalPinToInterrupt(BP0_pin), interruption, CHANGE);
    attachInterrupt(digitalPinToInterrupt(BP1_pin), interruption, CHANGE);
}

/// @brief Initialisation spécifique
/// @param  void
void encoder::init(void)
{
}

/// @brief Interception de l'interruption
/// @param  void
void encoder::interruption(void)
{
    // Gestion du rebond (debounce)
    static unsigned long BP_rebond     = 0;

    // Variable stockant l'état lu et précédent, pour la détection du sens de rotation
    // Voir readme pour l'usage
    static uint8_t etat = 0x00;

    // Fige le temps
    unsigned long time = millis();

    // Lit l'état des entrées A et B
    uint8_t BP0_val = ! digitalRead(BP0_pin);
    uint8_t BP1_val = ! digitalRead(BP1_pin);

    // Teste si on est dans une fenêtre d'occlusion pour le débounce
    if ((time - BP_rebond) > BP_delai_rebond)
    {
        BP_rebond = time;   // Déclenche la fenêtre d'exclusion

        etat |= BP0_val ? (0b00000100) : (0);       // Si l'entrée B active, mets le bit 3
        etat |= BP1_val ? (0b00001000) : (0);       // Si l'entrée A active, mets le bit 4

        switch (etat)
        {
            case 1: case 7: case 8: case 14:        // 4 possibilités de décalage positif
                decalage++;
                break;
            case 2: case 4: case 11: case 13:       // 4 possibilités de décalage négatif
                decalage--;
                break;
            // case 3: case 12:                     // Décalages doubles non pris en considération
            //     decalage += 2;
            //     break;
            // case 6: case 9:
            //     decalage -= 2;
            //     break;
        };

        etat = etat >> 2;
    }
}

/// @brief Lit la valeur du décalage actuel
/// @return le décalage relatif (+/-)
int8_t encoder::lecture()
{
    // Note la valeur actuelle avant de la remettre à 0 (pour le relatif)
    int8_t retour = decalage;
    decalage = 0;

    // Renvoie la bonne valeur
    return retour;
}

//-------------------------------------------------- That's all, Folks !!! ----