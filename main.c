#include <avr/io.h>
#include <math.h> // Wymagane dla funkcji matematycznych

#define RED_PIN PB0

void setup() {
    // Ustawienie pinu RED_PIN jako wyjścia
    DDRB |= (1 << RED_PIN);

    // Konfiguracja PWM dla RED_PIN (PB0) - Timer0
    TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0A1); // Fast PWM, wyjście na OC0A (PB0)
    TCCR0B = (1 << CS01); // Preskaler 8
}

void delay_ms(uint16_t ms) {
    while (ms--) {
        // Ta pętla zajmuje ok. 1 ms dla 8 MHz
        for (uint16_t i = 0; i < 400; i++) {
            __asm__ __volatile__("nop");
        }
    }
}

void fade(uint16_t total_time, float easing_factor) {
    uint16_t steps = 255; // Ilość kroków w cyklu (8-bitowy PWM)
    uint16_t delay_per_step = total_time / steps; // Czas na jeden krok

    for (uint16_t i = 0; i < steps; i++) {
        // Funkcja easing: zastosowanie potęgi (easing_factor) do wartości i
        float normalized = (float)i / steps; // Normalizacja do zakresu 0-1
        uint8_t brightness = (uint8_t)(pow(normalized, easing_factor) * 255);

        OCR0A = brightness; // Ustaw jasność
        delay_ms(delay_per_step); // Opóźnienie dla czasu całkowitego
    }
}

int main() {
    setup();

    while (1) {
        fade(100, 2.0); // Rozjaśnianie: czas 3000ms, easing_factor = 2.0 (kwadratowy)
        OCR0A = 0;
        delay_ms(1000);  // Krótkie zatrzymanie
    }

    return 0;
}