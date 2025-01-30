# Nazwa projektu
PROJECT = main

# Narzędzia
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

# Mikrokontroler i zegar
MCU = attiny45
F_CPU = 8000000UL

# Opcje kompilatora
CFLAGS = -Wall -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU)

# Pliki wyjściowe
TARGET = $(PROJECT).elf
HEX = $(PROJECT).hex

# Kompilacja
all: $(HEX)

$(TARGET): $(PROJECT).c
	$(CC) $(CFLAGS) -o $(TARGET) $(PROJECT).c

$(HEX): $(TARGET)
	$(OBJCOPY) -O ihex -R .eeprom $(TARGET) $(HEX)

# Programowanie
flash: $(HEX)
	$(AVRDUDE) -p $(MCU) -c usbasp -U flash:w:$(HEX):i

# Czyszczenie
clean:
	rm -f $(TARGET) $(HEX)

