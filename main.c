#include <avr/io.h>
#include <util/delay.h>

#define RED_PIN PB0
#define GREEN_PIN PB1
#define BLUE_PIN PB2

void setup() {
    // Ustawienie pinów RGB jako wyjścia
    DDRB |= (1 << RED_PIN) | (1 << GREEN_PIN) | (1 << BLUE_PIN);

    // Konfiguracja PWM dla pinu RED_PIN (PB0) - Timer0
    TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0A1); // Fast PWM, wyjście na OC0A (PB0)
    TCCR0B = (1 << CS01); // Preskaler 8

    // Wyłącz pozostałe kolory
    PORTB &= ~((1 << GREEN_PIN) | (1 << BLUE_PIN));
}

int main() {
    setup();

    // Ustaw jasność czerwonego koloru (wartość 0-255, gdzie 255 = maksymalna jasność)
    OCR0A = 0; // Połowa jasności (możesz zmienić na np. 255 dla pełnej jasności)

    // Główna pętla - nic nie robi poza utrzymaniem ustawień PWM
    while (1) {
        for (uint8_t i = 0; i < 255; i++) {
            OCR0A = i;       // Zwiększ jasność
            _delay_ms(5);   // Opóźnienie
        }
    }

    return 0;
}