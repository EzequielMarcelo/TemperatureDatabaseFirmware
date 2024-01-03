// ---- Auxiliary libraries ----
#include <Arduino.h>

//---- Hardware Mapping ----
#define PIN_TEMPERATURE A0

//---- Scope of functions -----
float GetTempCelsiusLM35(int adcValue);

//---- Initial Settings ----
void setup() 
{
    Serial.begin(115200);
}

//---- Main loop ----
void loop() 
{
    int adc = analogRead(PIN_TEMPERATURE);
    float temperature = GetTempCelsiusLM35(adc);  
    Serial.println(temperature);                   //debug
}

//---- Development of functions ----
float GetTempCelsiusLM35(int adcValue)
{
    float voltage = (adcValue * 5.0) / 1023.0;  //Voltage = (ADC * maximum input voltage) / maximum ADC value
    return voltage / 0.010;                     //The lm35 sensor is linear: 10mV/°C
}