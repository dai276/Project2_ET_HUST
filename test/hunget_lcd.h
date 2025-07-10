#ifndef HUNGET_LCD_H
#define HUNGET_LCD_H

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

// Control pins
#define LCD_RS_PORT   PORTD
#define LCD_RS_DDR    DDRD
#define LCD_RS        PD6
#define LCD_RW_PORT   PORTD
#define LCD_RW_DDR    DDRD
#define LCD_RW        PD5
#define LCD_EN_PORT   PORTD
#define LCD_EN_DDR    DDRD
#define LCD_EN        PD7

// Data bus
#define LCD_DATA_PORT PORTC
#define LCD_DATA_DDR  DDRC

// Initialize LCD in 8-bit, 2-line, 5x8 font
static inline void LCD_Init(void) {
	// configure data pins
	LCD_DATA_DDR = 0xFF;
	// configure control pins
	LCD_RS_DDR  |= (1<<LCD_RS);
	LCD_RW_DDR  |= (1<<LCD_RW);
	LCD_EN_DDR  |= (1<<LCD_EN);
	_delay_ms(40);

	// Function Set: 8-bit, 2 line, 5x8
	LCD_RS_PORT &= ~(1<<LCD_RS);
	LCD_RW_PORT &= ~(1<<LCD_RW);
	LCD_DATA_PORT = 0x38;
	LCD_EN_PORT |=  (1<<LCD_EN); _delay_us(1);
	LCD_EN_PORT &= ~(1<<LCD_EN); _delay_ms(2);

	// Display ON, Cursor OFF, Blink OFF
	LCD_DATA_PORT = 0x0C;
	LCD_EN_PORT |=  (1<<LCD_EN); _delay_us(1);
	LCD_EN_PORT &= ~(1<<LCD_EN); _delay_ms(2);

	// Clear display
	LCD_DATA_PORT = 0x01;
	LCD_EN_PORT |=  (1<<LCD_EN); _delay_ms(2);

	// Entry mode: Increment cursor
	LCD_DATA_PORT = 0x06;
	LCD_EN_PORT |=  (1<<LCD_EN); _delay_ms(2);
}

// Send a command
static inline void LCD_Cmd(uint8_t cmd) {
	LCD_RS_PORT &= ~(1<<LCD_RS);
	LCD_RW_PORT &= ~(1<<LCD_RW);
	LCD_DATA_PORT = cmd;
	LCD_EN_PORT |=  (1<<LCD_EN); _delay_us(1);
	LCD_EN_PORT &= ~(1<<LCD_EN); _delay_ms(2);
}

// Send a character
static inline void LCD_Char(uint8_t data) {
	LCD_RS_PORT |=  (1<<LCD_RS);
	LCD_RW_PORT &= ~(1<<LCD_RW);
	LCD_DATA_PORT = data;
	LCD_EN_PORT |=  (1<<LCD_EN); _delay_us(1);
	LCD_EN_PORT &= ~(1<<LCD_EN); _delay_ms(1);
}

// Print string
static inline void LCD_String(const char *s) {
	while (*s) LCD_Char((uint8_t)*s++);
}

// Print string at position
static inline void LCD_String_xy(uint8_t row, uint8_t pos, const char *s) {
	uint8_t addr = (row==1?0x00:0x40) + pos;
	LCD_Cmd(0x80 | addr);
	LCD_String(s);
}

// Clear display
static inline void LCD_Clear(void) {
	LCD_Cmd(0x01);
	_delay_ms(2);
}

#endif // HUNGET_LCD_H