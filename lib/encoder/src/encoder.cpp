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

#include <Arduino.h>
#include <encoder.h>

uint8_t encoder::BP0_pin;
uint8_t encoder::BP1_pin;

int8_t encoder::decalage;

encoder::encoder(uint8_t BP0, uint8_t BP1)
{
    BP0_pin = BP0;
    BP1_pin = BP1;
    attachInterrupt(digitalPinToInterrupt(BP0_pin), interruption, CHANGE);
    attachInterrupt(digitalPinToInterrupt(BP1_pin), interruption, CHANGE);
}

void encoder::init(void)
{
    // pinMode(BP0_pin, INPUT_PULLUP);
    // pinMode(BP1_pin, INPUT_PULLUP);
}

void encoder::interruption(void)
{
    static uint8_t BP_p                = 0;
    static unsigned long BP_rebond     = 0;

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

//                           _______         _______       
//               Pin1 ______|       |_______|       |______ Pin1
// negative <---         _______         _______         __      --> positive
//               Pin2 __|       |_______|       |_______|   Pin2

		//	new	new	old	old
		//	pin2	pin1	pin2	pin1	Result
		//	----	----	----	----	------
		//	0	0	0	0	no movement
		//	0	0	0	1	+1
		//	0	0	1	0	-1
		//	0	0	1	1	+2  (assume pin1 edges only)
		//	0	1	0	0	-1
		//	0	1	0	1	no movement
		//	0	1	1	0	-2  (assume pin1 edges only)
		//	0	1	1	1	+1
		//	1	0	0	0	+1
		//	1	0	0	1	-2  (assume pin1 edges only)
		//	1	0	1	0	no movement
		//	1	0	1	1	-1
		//	1	1	0	0	+2  (assume pin1 edges only)
		//	1	1	0	1	-1
		//	1	1	1	0	+1
		//	1	1	1	1	no movement
//-------------------------------------------------- That's all, Folks !!! ----