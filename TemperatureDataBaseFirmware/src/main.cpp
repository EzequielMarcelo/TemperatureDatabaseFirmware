// ---- Auxiliary libraries ----
#include <Arduino.h>

//---- Hardware Mapping ----
#define PIN_TEMPERATURE A0

//---- Scope of functions -----
float GetTempCelsiusLM35(int adcValue);
void SendTempSerial(unsigned long delay_ms);
void SerialBuffering(void);
void Decode(String data);

//---- Initial Settings ----
void setup() 
{
    Serial.begin(115200);
}

//---- Main loop ----
void loop() 
{
    SendTempSerial(1000);    //Sends temperature every second
    SerialBuffering();       //waiting for data from the serial
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
void SerialBuffering(void)
{
    if(Serial.available())
    {
        //buffer format => command:data\n
        String buffer = Serial.readStringUntil('\n');    //similar to SerialPort.readLine in C#
        Decode(buffer);
    }
} 
void Decode(String buffer)
{
        int dataStartIndex = buffer.indexOf(':') + 1;
    char command;
    String stringData;

    if(dataStartIndex)                                  //check if the separator was received
        {
        command = buffer.charAt(0);
        stringData = buffer.substring(dataStartIndex);
        Serial.println(buffer);                        //Debug 
    }
} 