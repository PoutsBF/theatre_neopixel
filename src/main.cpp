//-----------------------------------------------------------------------------
//------ Stéphane Lepoutère ------------------------------------- 2022 --------
//-----------------------------------------------------------------------------
//------------------ Eclairage théatre légo -----------------------------------
//-----------------------------------------------------------------------------

#include <Arduino.h>

#include <WS2812FX.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define LED_PIN2 4
#define LED_PIN 3

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT2 8
#define LED_COUNT 15

// Potentiomètres et leur led
#define POT_1 A1
#define POT_2 A3
#define POT_LED_1 A2
#define POT_LED_2 A4

unsigned long led_interval = 5000;
WS2812FX leds = WS2812FX(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

// setup() function -- runs once at startup --------------------------------
void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ;        // wait for serial port to connect. Needed for Leonardo
    leds.init();
    leds.setBrightness(50);
    leds.setSpeed(1000);
    leds.setColor(0x0000FF);
    leds.setMode(FX_MODE_FIREWORKS_RANDOM);
    leds.start();

    pinMode(POT_1, INPUT);
    pinMode(POT_2, INPUT);

    pinMode(POT_LED_1, OUTPUT);
    pinMode(POT_LED_2, OUTPUT);
}

// loop() function -- runs repeatedly as long as board is on ---------------

void loop()
{
#define TIMER_MS 5000
#define TIMER_ADC 500

    static unsigned long last_change = 0;
    static unsigned long last_adc = 0;
    static unsigned long now = 0;

    uint8_t modeLed = leds.getMode();

    now = millis();

    leds.service();

    if (now - last_change > TIMER_MS)
    {
        leds.setMode((leds.getMode() + 1) % leds.getModeCount());
        Serial.print("Mode : ");
        Serial.println(leds.getModeName(leds.getMode()));
        last_change = now;
    }

    if (now - last_adc > TIMER_ADC)
    {
        last_adc = now;
        uint16_t val_adc = analogRead(POT_1);
        digitalWrite(POT_LED_1, (uint8_t)(0x00FF & (val_adc >> 2)));

        val_adc = ((val_adc) << 2) + 10;
        Serial.print("pot 1 : ");
        Serial.println(val_adc);

        leds.setSpeed(val_adc);

        val_adc = analogRead(POT_2);
        val_adc = (uint8_t)(0x00FF & (val_adc >> 2));
        digitalWrite(POT_LED_2, val_adc);
        Serial.print("pot 2 : ");
        Serial.println(val_adc);
        leds.setBrightness(val_adc);
    }
}
