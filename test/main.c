#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "hunget_lcd.h"
#include "hunget_adc.h"

int main(void) {
	char buf[16];
	uint16_t raw, milliv, temp_int, temp_frac;
	float temp;

	// 1) Kh?i t?o LCD và ADC
	LCD_Init();    // PC0–PC7 data, PD5(RW), PD6(RS), PD7(EN)
	ADC_Init();    // PA1 input, Vref=AVcc, prescaler=128

	while (1) {
		// 2) ??c raw ADC t? kênh 1 (PA1)
		raw     = ADC_ReadChannel(1);

		// 3) raw ? mV (0…5000)
		milliv  = (uint16_t)((uint32_t)raw * 5000UL / 1024UL);

		 /*4) Tách °C: ph?n nguyên và th?p phân
		temp_int  = milliv / 10;    // 10 mV/°C
		temp_frac = milliv % 10;    // ph?n th?p phân */
/* 4) Tính nhi?t ??: làm tròn g?n nh?t
temp_int = (milliv + 5) / 10;    // 10 mV/°C, c?ng 5 ?? làm tròn*/
// 4) Tính nhi?t ?? và làm tròn ??n 0.5°C
temp = (float)milliv / 10.0; // Nhi?t ?? (°C)
temp = (int)(temp * 2 + 0.5) / 2.0; // Làm tròn ??n 0.5°C
temp_int = (uint16_t)temp; // Ph?n nguyên
temp_frac = (uint16_t)((temp - temp_int) * 10); // Ph?n th?p phân (0 ho?c 5)
		// 5) Hi?n th? nhãn Vout và giá tr?
		LCD_Clear();
		LCD_String_xy(1, 0, "Vout:");
		snprintf(buf, sizeof(buf), "%4u mV", milliv);
		LCD_String_xy(1, 6, buf);

		/* 6) Hi?n th? nhãn Nhiet do và giá tr?
		LCD_String_xy(2, 0, "Nhiet do:");
		snprintf(buf, sizeof(buf), "%2u.%1u C", temp_int, temp_frac);
		LCD_String_xy(2, 10, buf);*/

/* 6) Hi?n th? nhãn Nhiet do và giá tr? (làm tròn)
LCD_String_xy(2, 0, "Nhiet do:");
snprintf(buf, sizeof(buf), "%2u C", temp_int);
LCD_String_xy(2, 10, buf);*/
		        // 6) Hi?n th? nhãn Nhiet do và giá tr? (XX.X C)
		        LCD_String_xy(2, 0, "Nhiet do:");
		        snprintf(buf, sizeof(buf), "%2u.%1u C", temp_int, temp_frac);
		        LCD_String_xy(2, 10, buf);
		_delay_ms(500);
	}

	return 0;
}