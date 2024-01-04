// ---- Auxiliary libraries ----
#include <Arduino.h>

//---- Hardware Mapping ----
#define PIN_TEMPERATURE A0

//---- Scope of functions -----
float GetTempCelsiusLM35(int adcValue);
void SendTempSerial(unsigned long delay_ms);

//---- Initial Settings ----
void setup() 
{
    Serial.begin(115200);
}

//---- Main loop ----
void loop() 
{
    SendTempSerial(1000);    //Sends temperature every second
}

//---- Development of functions ----
float GetTempCelsiusLM35(int adcValue)
{
    float voltage = (adcValue * 5.0) / 1023.0;  //Voltage = (ADC * maximum input voltage) / maximum ADC value
    return voltage / 0.010;                     //The lm35 sensor is linear: 10mV/°C
}
void SendTempSerial(unsigned long delay_ms)
{
    static unsigned long timeLastRead = 0;

    if((millis() - timeLastRead) >= delay_ms)
    {
        int adcValue = analogRead(PIN_TEMPERATURE);
        float temperature = GetTempCelsiusLM35(adcValue);
        
        Serial.print("T:"); 
        Serial.println(temperature);    
        timeLastRead = millis();
    }    
}