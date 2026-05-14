#include <Arduino.h>
#include <EEPROM.h>

const int FAN_PWM_PIN = 9;
const int BUTTON_PIN = 2;
const int MEM_ADDRESS = 0;

int levels[] = {
    130,
    168,
    206,
    244,
    282,
    320,
};

int levelsSize = sizeof(levels) / sizeof(levels[0]);

int selected = 0;
bool buttonPressed = false;
bool saveSelection = false;
bool standby = false;
unsigned long lastInput = 0;
const unsigned long inputDelay = 50;
const unsigned long saveDelay = 8000;
const unsigned long standbyDelay = 3000;

void setup() {
    pinMode(FAN_PWM_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    TCCR1A = 0;
    TCCR1B = 0;

    TCCR1A |= (1 << COM1A1) | (1 << WGM11);
    TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS10);

    ICR1 = 320;

    byte storedSelected = EEPROM.read(MEM_ADDRESS);

    if (storedSelected >= levelsSize) {
        selected = 0;
    } else {
        selected = storedSelected;
    }

    OCR1A = levels[selected];
}

void loop() {

    unsigned long now = millis();

    if ((now - inputDelay) > lastInput) {

        int buttonState = digitalRead(BUTTON_PIN);

        if (!buttonPressed && buttonState == LOW) {
            buttonPressed = true;
            lastInput = now;
        }

        if (buttonPressed && (now - standbyDelay) > lastInput) {
            standby = true;
            OCR1A = 0;
        }

        if (buttonPressed && buttonState == HIGH) {

            if ((now - standbyDelay) <= lastInput) {
                
                if (!standby) {
                    selected++;

                    if (selected >= levelsSize) {
                        selected = 0;
                    }
                }

                OCR1A = levels[selected];
                saveSelection = true;
                standby = false;
            }

            buttonPressed = false;
            lastInput = now;
        }
    }

    if (saveSelection && (now - saveDelay) > lastInput) {
        EEPROM.update(MEM_ADDRESS, selected);
        saveSelection = false;
    }
}