#include <Arduino.h>

const int FAN_PWM_PIN = 9;

int levels[] = {
    0,
    64,
    128,
    192,
    256,
    320
};

int selected = 0;

void setup() {
    pinMode(FAN_PWM_PIN, OUTPUT);

    TCCR1A = 0;
    TCCR1B = 0;

    TCCR1A |= (1 << COM1A1) | (1 << WGM11);
    TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS10);

    ICR1 = 320;

    OCR1A = levels[selected];
}

void loop() {
}