# AVR_WEATHER_STATION

Weather station based on 8-bit AVR and popular, cheap sensors. Actual used AVR is Atmega328P, not Atmega8 like on the schematic. The Winstar LCD has been taken from ECR device, so I don't know exact model and driver version, but slightly modified version of my HD44780 (mainly added E2 line and modified timings) library works fine. In the future, the station will be extended by wifi module to fetch weather forecasts.

## Weather station

![station](/station.jpg "Weather station")

## Circuit diagram

![station](/weather_schematics.png "Circuit diagram")
