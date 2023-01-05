/******************************************************************************
    Gestion des Boutons Poussoirs                   Stéphane Lepoutère (c) 2020

    Observe l'arrivée de changement d'état sur les BP (interruption)
    Gère le débounce
    Détecte les appuis courts, longs, doubles clics et l'état instantané
    2020-11-07 : à faire : modification de la philosophie du double clic :
        un double clic est considéré comme un clic qui arrive dans un délai 
        court après le précédent. Le premier clic est sensé avoir été pris 
        en compte.
*/

#pragma once
#ifndef GESTIONBP_H
#define GESTIONBP_H
#include <Arduino.h>

//-----------------------------------------------------------------------------
//---- Définition des constantes ----
const unsigned long BP_delai_rebond = 5;

#define BP_PILE_TAILLE      16

//-----------------------------------------------------------------------------
//---- Définition des structures ----
struct BP_struct_msg
{
    uint8_t idBP;
    uint8_t idMsg;
};

struct BP_struct_pile
{
    uint8_t idBP;
    uint8_t etat;
    unsigned long time;
};
//-----------------------------------------------------------------------------
//---- Définition des structures ----

class encoder
{
    public:
        encoder(uint8_t BP0, uint8_t BP1);
        void init(void);
        static void interruption(void);
        int8_t lecture();

    private:
        static uint8_t BP0_pin;
        static uint8_t BP1_pin;

        static int8_t decalage;
};

#endif