#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

// Definicje pinów
#define RED_PIN PB0
#define SW0_PIN PB4

volatile uint8_t fade_active = 0; // Flaga do sterowania rozjaśnianiem

void delay_ms(uint16_t ms) {
    while (ms--) {
        for (uint16_t i = 0; i < 400; i++) {
            __asm__ __volatile__("nop");
        }
    }
}

void setup() {
    // Ustawienie diody jako wyjście
    DDRB |= (1 << RED_PIN);

    // Konfiguracja PWM dla RED_PIN (PB0) - Timer0
    TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0A1); // Fast PWM, wyjście na OC0A
    TCCR0B = (1 << CS01); // Preskaler 8

    // Przycisk jako wejście z pull-up
    DDRB &= ~(1 << SW0_PIN);
    PORTB |= (1 << SW0_PIN);

    // Włączenie przerwań od zmiany stanu SW0 (PCINT)
    GIMSK |= (1 << PCIE);
    PCMSK |= (1 << SW0_PIN);
    sei(); // Globalne włączenie przerwań
}

void fade(uint16_t total_time, float easing_factor) {
    uint16_t steps = 255;
    uint16_t delay_per_step = total_time / steps;

    for (uint16_t i = 0; i < steps; i++) {
        if (!fade_active) return; // Jeśli przycisk zostanie wciśnięty, natychmiast przerwij

        float normalized = (float)i / steps;
        uint8_t brightness = (uint8_t)(pow(normalized, easing_factor) * 255);

        OCR0A = brightness; // Ustaw jasność
        delay_ms(delay_per_step);
    }
}

// Przerwanie od zmiany stanu na SW0
ISR(PCINT0_vect) {
    delay_ms(50); // Proste debounce
    if (!(PINB & (1 << SW0_PIN))) { // SW0 wciśnięty
        fade_active = 0; // Przerwij rozjaśnianie
        TCCR0A &= ~((1 << COM0A1) | (1 << COM0A0)); // Wyłącz PWM
        PORTB &= ~(1 << RED_PIN); // Wymuś stan niski
    } else { // SW0 puszczony
        fade_active = 1; // Aktywuj rozjaśnianie
    }
}

int main() {
    setup();
    if (!(PINB & (1 << SW0_PIN))) { // SW0 wciśnięty
        fade_active = 0; // Przerwij rozjaśnianie
        TCCR0A &= ~((1 << COM0A1) | (1 << COM0A0)); // Wyłącz PWM
        PORTB &= ~(1 << RED_PIN); // Wymuś stan niski
    } else { // SW0 puszczony
        fade_active = 1; // Aktywuj rozjaśnianie
    }

    while (1) {
        if (fade_active) {
            fade(1000, 2.0);
            // fade_active = 0; // Zapobiega wielokrotnemu uruchamianiu w pętli
        }
    }
}
