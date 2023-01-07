/******************************************************************************
    Gestion d'un encodeur rotatif                   Stéphane Lepoutère (c) 2020

    Observe l'arrivée de changement d'état sur les 2 entrées (interruption)
    Gère le débounce
*/

#pragma once
#include <Arduino.h>

//-----------------------------------------------------------------------------
//---- Définition des constantes ----
const unsigned long BP_delai_rebond = 5;

//-----------------------------------------------------------------------------
//---- Définition de la classe ----

/// @brief Objet encapsulant l'encodeur rotatif
class encoder
{
    public:
        encoder(uint8_t BP0, uint8_t BP1);  // constructeur
        void init(void);                    // Objet d'initialisation 
        int8_t lecture();                   // Renvoie le décalage relatif

        static void interruption(void);     // A ne pas utiliser

    private:
        static uint8_t BP0_pin;             // Garde la patte A
        static uint8_t BP1_pin;             // Garde la patte B

        static int8_t decalage;             // Décalage relatif en cours
};

//-------------------------------------------------- That's all, Folks !!! ----