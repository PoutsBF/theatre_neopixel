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

    unsigned long time = millis();
    uint8_t BP0_val = ! digitalRead(BP0_pin);
    uint8_t BP1_val = ! digitalRead(BP1_pin);

    if ((time - BP_rebond) > BP_delai_rebond)
    {
        BP_rebond = time;
        etat |= 0b10000000;
        etat |= BP0_val ? (0b10000100) : (0);
        etat |= BP1_val ? (0b10001000) : (0);
    }

    if(etat & 0b10000000)
    {
        etat &= 0b01111111;

        switch (etat)
        {
            case 1: case 7: case 8: case 14:
                decalage++;
                break;
            case 2: case 4: case 11: case 13:
                decalage--;
                break;
            // case 3: case 12:
            //     decalage += 2;
            //     break;
            // case 6: case 9:
            //     decalage -= 2;
            //     break;
        };

        etat = etat >> 2;
    }
}

int8_t encoder::lecture()
{
    int8_t retour = decalage;
    decalage = 0;
    return retour;
}

//-------------------------------------------------- That's all, Folks !!! ----