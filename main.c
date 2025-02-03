#include <avr/io.h>
#include <avr/interrupt.h>

// Definicje pinów
#define RED_PIN PB0
#define GREEN_PIN PB1
#define BLUE_PIN PB2

#define SW0_PIN PB4
#define SW1_PIN PB3

volatile uint8_t red_state = 0;
volatile uint8_t blue_state = 0;

void delay_ms(uint16_t ms) {
    while (ms--) {
        // Ta pętla zajmuje ok. 1 ms dla 8 MHz
        for (uint16_t i = 0; i < 400; i++) {
            __asm__ __volatile__("nop");
        }
    }
}

void setup() {
    // Ustawienie diod jako wyjścia
    DDRB |= (1 << RED_PIN) | (1 << BLUE_PIN);

    // Przyciski jako wejścia z pull-up
    DDRB &= ~((1 << SW0_PIN) | (1 << SW1_PIN));
    PORTB |= (1 << SW0_PIN) | (1 << SW1_PIN);

    // Włączenie przerwań od zmiany stanu pinów (PCINT)
    GIMSK |= (1 << PCIE);        // Włącz przerwania dla PCINT
    PCMSK |= (1 << SW0_PIN) | (1 << SW1_PIN); // Włącz przerwanie dla PB4 i PB3
    sei();  // Globalne włączenie przerwań
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
        // Nic nie robimy, obsługa przycisków odbywa się w przerwaniach
    }
}

// Przerwanie od zmiany stanu na pinach
ISR(PCINT0_vect) {
    // Debounce - krótka zwłoka
    delay_ms(200);
    
    if (!(PINB & (1 << SW0_PIN))) { // Jeśli SW0 (PB4) wciśnięty
        red_state ^= 1; // Zmiana stanu (toggle)
        if (red_state) PORTB |= (1 << RED_PIN); // Włącz czerwoną diodę
        else PORTB &= ~(1 << RED_PIN); // Wyłącz czerwoną diodę
    }

    if (!(PINB & (1 << SW1_PIN))) { // Jeśli SW1 (PB3) wciśnięty
        blue_state ^= 1; // Zmiana stanu (toggle)
        if (blue_state) PORTB |= (1 << BLUE_PIN); // Włącz niebieską diodę
        else PORTB &= ~(1 << BLUE_PIN); // Wyłącz niebieską diodę
    }
}
