#include <Adafruit_NeoPixel.h>
#define PIN 8
Adafruit_NeoPixel strip(32, PIN, NEO_RGB + NEO_KHZ800);

int green = 0;
int blue = 0;
int red = 255;
int mode = 1;
const int pin_button1 = 11;
const int pin_button2 = 10;
const int pin_button3 = 9;
bool button_state1 = 0; // om knappen er presset ned
bool button_state2 = 0;
bool button_state3 = 0;
bool button1_on = 0; // signal til spillet
bool button2_on = 0;
bool lastBS1 = 0;
bool lastBS2 = 0;
unsigned long button1_start_time = 0; // tid siden knapp ble trykket
unsigned long button2_start_time = 0; // tid siden knapp ble trykket
unsigned long time_tolerance = 0;     // hvor lenge knappetrykket skal vare
unsigned long led_delay = 50;         // hvor langt mellom hver led aktivering
unsigned long prev_case = 0;
int index = 0;

void setup()
{
    pinMode(pin_button1, INPUT);
    pinMode(pin_button2, INPUT);
    pinMode(pin_button3, INPUT);
    pinMode(PIN, OUTPUT);
    strip.begin();
    strip.clear();
    Serial.begin(9600);
    randomSeed(analogRead(0));
}

void main_game()
{
    switch (mode)
    {
    case 1: // modus 1
        if (millis() - prev_case > led_delay)
        {
            prev_case = millis();
            strip.clear();
            index++;
            strip.setPixelColor(index, green, red, blue);
            strip.show();

            if (index == 32 && button_state1 == HIGH)
            {
                Serial.print("du klarte det");
                mode = 2;
                strip.clear();
                update_color();
                Serial.print("1");
                break;
            }
            else if (index == 32) // om man ikke klarer det
            {
                for (int i = 0; i < 32; i++)
                { // gjør hele rekka rød
                    strip.setPixelColor(i, 0, 255, 0); // gjør rekken rød
                }
                strip.show();
                delay(1000); // venter litt
                index = 0;
                strip.clear(); //rens led
                update_color(); //bytt farge
                Serial.print("2");
            }
        }
    case 2: // modus 2
        if (millis() - prev_case > led_delay)
        {
            prev_case = millis();
            strip.clear();
            strip.setPixelColor(index, green, red, blue);
            index--;
            strip.show();

            if (index == 0 && button_state1 == HIGH) // sprett tilbake
            {
                Serial.print("du klarte det");
                mode = 1;
                strip.setPixelColor(0, 0, 0, 0);
                update_color();
                break;
            }
            else if (index == 0) // mekka ikke.
            {
                for (int i = 0; i < 32; i++)
                { // gjør hele rekka rød
                    strip.setPixelColor(i, 0, 255, 0);
                }
                strip.show();
                delay(1000);
                mode = 1;
                strip.clear();
                update_color();
            }
        }
    }
}

void button_update()
{ // opdaterer knappene t
    button_state1 = digitalRead(pin_button1);
    button_state2 = digitalRead(pin_button2);
}
// hvis knapp nede og tid siden sist nede er mer enn.
// la knapp være på en tid
void update_color()
{
    red = random(255);
    green = random(255);
    blue = random(255);
}

void loop()
{
    button_update();
    main_game();
}
