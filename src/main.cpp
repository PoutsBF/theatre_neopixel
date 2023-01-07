//-----------------------------------------------------------------------------
//------ Stéphane Lepoutère ------------------------------------- 2022 --------
//-----------------------------------------------------------------------------
//------------------ Eclairage théatre légo -----------------------------------
//-----------------------------------------------------------------------------

#include <Arduino.h>

#include <Encoder.h>
#include <WS2812FX.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define LED_PIN2 4
#define LED_PIN 3

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT2 32
#define LED_COUNT 15

// Potentiomètres et leur led
#define POT_1 A1
#define POT_2 A3
#define POT_LED_1 A2
#define POT_LED_2 A4

unsigned long led_interval = 5000;
WS2812FX leds_chapeau = WS2812FX(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);
WS2812FX leds_theatre = WS2812FX(LED_COUNT2, LED_PIN2, NEO_GRBW + NEO_KHZ800);
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

encoder monEncodeur(0, 1);

// setup() function -- runs once at startup --------------------------------
void setup()
{
    Serial.begin(115200);

    // attend que le port série soit connecté ou 2 secondes (module autonome)
    while ((!Serial) && (millis() < 2000));

    // Initialise le module led en arc
    leds_theatre.init();        // première étape
    leds_theatre.setBrightness(50);
    leds_theatre.setSpeed(5000);
    leds_theatre.setSegment();
    leds_theatre.setColor(0x0000FF);
    leds_theatre.setMode(FX_MODE_DUAL_SCAN);
    leds_theatre.start();

    // Initialise le module led en arc
    leds_chapeau.init();        // première étape
    // leds_chapeau.setBrightness(50);
    // leds_chapeau.setSpeed(5000);
    // leds_chapeau.setSegment();
    // leds_chapeau.setColor(0x0000FF);
    // leds_chapeau.setMode(FX_MODE_THEATER_CHASE_RAINBOW);

    leds_chapeau.setSegment(0, 0, 6, FX_MODE_THEATER_CHASE_RAINBOW, RED, 5000, NO_OPTIONS);
    leds_chapeau.setSegment(1, 7, 14, FX_MODE_THEATER_CHASE_RAINBOW, RED, 5000, REVERSE);

    leds_chapeau.start();

    // Initialise les entrées des potentiomètres et leur led
    pinMode(POT_1, INPUT);
    pinMode(POT_2, INPUT);
    pinMode(POT_LED_1, OUTPUT);
    pinMode(POT_LED_2, OUTPUT);
}

// loop() function -- runs repeatedly as long as board is on ---------------
void loop()
{
    // Fréquence de lecture des potentiomètres
#define TIMER_ADC 500

    static unsigned long last_adc = 0;      // Dernière lecture des potentiomètre
    static uint8_t pos_encoder = 10;        // Position de l'encodeur

    unsigned long now;           // Lecture du temps dans la boucle

    now = millis();         // Lecture du temps dans cette boucle

    leds_theatre.service();         // Service des leds du tour
    leds_chapeau.service();         // Service des leds du dessus

    // Relève la présence de décalage sur l'encodeur rotatif
    int8_t decalage = monEncodeur.lecture();

    // Test le décalage
    if (decalage)
    {
        pos_encoder += decalage;    // Modifie la position
        if (pos_encoder >= leds_theatre.getModeCount()) // test si dans la liste des modes
        {
            // Si en dehors, repart à 0
            pos_encoder = 0;
        }
        // Change le mode
        leds_theatre.setMode(pos_encoder);

        // Log sur la sortie série
        Serial.print("Mode : ");
        Serial.print(leds_theatre.getMode());
        Serial.print(" - ");
        Serial.println(leds_theatre.getModeName(leds_theatre.getMode()));
    }

    // Test si fréquence de lecture de l'adc écoulée
    if (now - last_adc > TIMER_ADC)
    {
        // Oui, sauve la nouvelle heure
        last_adc = now;

        // Lit le potentiomètre pour la vitesse
        uint16_t val_adc = analogRead(POT_1);
        // Calcul pour positionner entre 10 (rapide) et 4106 (lent)
        val_adc = ((val_adc) << 2) + 10;
        // Fixe la nouvelle vitesse
        leds_theatre.setSpeed(val_adc);

        // Lit le potentiomètre de luminosité
        val_adc = analogRead(POT_2);
        // Calcul pour le positionner entre 0 et 255
        val_adc = (uint8_t)(0x00FF & (val_adc >> 2));
        // Fixe la nouvelle luminosité
        leds_theatre.setBrightness(val_adc);
        Serial.print("Luminosité : ");
        Serial.println(val_adc);
    }
}

//-------------------------- That's all, Folks ! ------------------------------