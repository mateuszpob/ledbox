#include <avr/io.h>
#include <util/delay.h>

#define RED_PIN PB0
#define GREEN_PIN PB1
#define BLUE_PIN PB2
#define BUTTON_PIN PB3

void setup() {
    // Ustawienie pinów RGB jako wyjścia
    DDRB |= (1 << RED_PIN) | (1 << GREEN_PIN) | (1 << BLUE_PIN);

    // Ustawienie pinu przycisku jako wejście z rezystorem pull-up
    DDRB &= ~(1 << BUTTON_PIN); // Ustaw jako wejście
    PORTB |= (1 << BUTTON_PIN); // Włącz pull-up
}

int main() {
    setup();

    // Włącz kolor czerwony (RED_PIN na HIGH, pozostałe na LOW)
    PORTB |= (1 << RED_PIN) | (1 << GREEN_PIN) | (1 << BLUE_PIN);
    // PORTB &= ~((1 << BLUE_PIN));

    // Główna pętla - nic nie robi, tylko utrzymuje kolor
    while (1) {
        // Możesz tutaj dodać dodatkowe testy, np. miganie diodą
        _delay_ms(500);
    }

    return 0;
}
