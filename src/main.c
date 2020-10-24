/*
 * main.c
 *
 *  Created on: 3 maj 2020
 *      Author: Daniel
 *
 *		Weather station with date, time,
 *		indoor and outdoor temperature,
 *		humidity and air pressure.
 *
 *		uC: Atmega328P @ 8MHz
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "LCD.h"
#include "DS18B20.h"
#include "DHT11.h"
#include "rtc.h"
#include "BMP280.h"
#include "brightnessControl.h"

struct ds18b20_data externalTemp;
struct dht11_data tempHumidity;
struct rtc_data time;
struct bmp280_data tempPressure;

void updateSensors();
void updateTime();
void printSensorsScreen(char[24], char);

int main() {
	uint8_t counter = 0;
	char text[24];
	char timeColon = ' ';

	ds18b20_init();
	dht11_init();
	rtc_Init();
	bmp280_init();
	lcd_init();
	bcInit();

	char day[5] = "Sob";
	time.day = 10;
	time.month = 9;
	time.year = 20;
	time.minute = 22;
	time.second = 0;
	time.hour = 10;
	time.dayOfWeek = 4;
	time.dayOfWeekName = day;
	//rtcSetTime(&time);

	updateSensors();
	lcd_clear();

	sei();

	while (1) {
		if (counter % 2 == 0) {
			timeColon = ' ';
		} else {
			timeColon = ':';
		}

		if (counter % 5 == 0) {
			updateTime();
			adjustBrightness();
		}
		if (counter >= 100) {
			updateSensors();
			counter = 0;
			lcd_clear();

		}
		counter++;

		printSensorsScreen(text, timeColon);
		_delay_ms(1000);
	}
}

void printSensorsScreen(char text[24], char timeColon) {
	lcd_goTo(9, 0);
	sprintf(text, "%02u%c%02u", time.hour, timeColon, time.minute);
	lcd_write(text);
	lcd_goTo(4, 1);
	sprintf(text, "%02u/%02u/20%02u, %s", time.day, time.month, time.year,
			time.dayOfWeekName);
	lcd_write(text);

	lcd_goTo(0, 2);
	sprintf(text, "%c:%c%d,%01u%cC", LCD_OUT_TEMP_CHAR,
			((externalTemp.tempInteger < 0) ? '-' : ' '),
			abs(externalTemp.tempInteger), externalTemp.tempFraction,
			LCD_DEG_CHAR);
	lcd_write(text);

	lcd_goTo(11, 2);
	sprintf(text, "%c:%c%d,%01u%cC", LCD_IN_TEMP_CHAR,
			((tempPressure.tempInteger < 0) ? '-' : ' '),
			abs(tempPressure.tempInteger), tempPressure.tempFraction / 10,
			LCD_DEG_CHAR);

	lcd_write(text);
	lcd_goTo(0, 3);
	sprintf(text, "%c: %u%%     %c: %u,%uhPa", LCD_HUM_CHAR,
			tempHumidity.rhInteger,
			LCD_PRESS_CHAR, tempPressure.pressureInteger,
			tempPressure.pressureFraction);
	lcd_write(text);
}

void updateSensors() {
	dht11_getData(&tempHumidity);
	ds18b20_getTemperature(&externalTemp);
	bmp280_getData(&tempPressure);
}

void updateTime() {
	rtc_getTime(&time);
}
