#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BUTTON_PIN PB3
#define RED_PIN PB0
#define GREEN_PIN PB1
#define BLUE_PIN PB2

volatile uint8_t current_color = 0;  // Aktualny kolor

void setup() {
    // Ustawienie pinów RGB jako wyjścia
    DDRB |= (1 << RED_PIN) | (1 << GREEN_PIN) | (1 << BLUE_PIN);
    
    // Ustawienie pinu przycisku jako wejście z rezystorem pull-up
    DDRB &= ~(1 << BUTTON_PIN); // Ustaw jako wejście
    PORTB |= (1 << BUTTON_PIN); // Włącz pull-up

    // Włącz przerwanie zewnętrzne dla przycisku
    GIMSK |= (1 << PCIE);       // Włącz przerwania od zmiany stanu pinów
    PCMSK |= (1 << BUTTON_PIN); // Włącz przerwanie dla konkretnego pinu
    sei();                      // Globalne włączenie przerwań
}

void setColor(uint8_t color) {
    switch (color) {
        case 0: OCR0A = 255; OCR0B = 0;   OCR1A = 0;   break; // Czerwony
        case 1: OCR0A = 0;   OCR0B = 255; OCR1A = 0;   break; // Zielony
        case 2: OCR0A = 0;   OCR0B = 0;   OCR1A = 255; break; // Niebieski
        case 3: OCR0A = 255; OCR0B = 255; OCR1A = 0;   break; // Żółty
        case 4: OCR0A = 255; OCR0B = 0;   OCR1A = 255; break; // Fioletowy
        case 5: OCR0A = 0;   OCR0B = 255; OCR1A = 255; break; // Cyan
        case 6: OCR0A = 255; OCR0B = 255; OCR1A = 255; break; // Biały
        case 7: OCR0A = 255; OCR0B = 128; OCR1A = 0;   break; // Pomarańczowy
        case 8: OCR0A = 255; OCR0B = 0;   OCR1A = 128; break; // Różowy
    }
}

// Funkcja główna
int main() {
    setup();

    // Konfiguracja PWM
    TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0A1) | (1 << COM0B1); // Fast PWM dla OCR0A i OCR0B
    TCCR0B = (1 << CS01); // Preskaler 8
    TCCR1 = (1 << CTC1) | (1 << PWM1A) | (1 << COM1A1) | (1 << CS11); // Fast PWM dla OCR1A, preskaler 8

    while (1) {
        // Główna pętla robi tylko tyle, że czeka na przerwania
    }
    return 0;
}

// Obsługa przerwania dla przycisku
ISR(PCINT0_vect) {
    if (!(PINB & (1 << BUTTON_PIN))) {  // Sprawdzenie, czy przycisk jest wciśnięty
        _delay_ms(50); // Debouncing
        if (!(PINB & (1 << BUTTON_PIN))) { // Ponowna weryfikacja
            current_color = (current_color + 1) % 9; // Następny kolor
            setColor(current_color); // Ustaw kolor
        }
    }
}

