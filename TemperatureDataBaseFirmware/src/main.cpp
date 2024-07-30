// ---- Auxiliary libraries ----
#include <Arduino.h>

//---- Hardware Mapping ----
#define PIN_TEMPERATURE A0
#define LOAD_PIN 11

#define LENTH_WINDOW 50
float bufferAvg[LENTH_WINDOW];

//---- Scope of functions -----
float GetTempCelsiusLM35(int adcValue);
void SendTempSerial(unsigned long delay_ms);
void SerialBuffering(void);
void Decode(String data);
void SelectCommand(char command, String data);
void UpdateCelsiusTempBuffer(float newTemp);
float GetAverageTempCelsius();

//---- Initial Settings ----
void setup() 
{
    Serial.begin(115200);
    pinMode(LOAD_PIN, OUTPUT);
}

//---- Main loop ----
void loop() 
{
    int adcValue = analogRead(PIN_TEMPERATURE);

    UpdateCelsiusTempBuffer(GetTempCelsiusLM35(adcValue));
    SendTempSerial(1000);    //Sends temperature every second
    SerialBuffering();       //waiting for data from the serial
}

//---- Development of functions ----
float GetTempCelsiusLM35(int adcValue)
{
    float voltage = (adcValue * 5.0) / 1023.0;  //Voltage = (ADC * maximum input voltage) / maximum ADC value
    return voltage / 0.010;                     //The lm35 sensor is linear: 10mV/°C
}
void UpdateCelsiusTempBuffer(float newTemp)
{
    static uint8_t buffer_id = 0;

    bufferAvg[buffer_id++] = newTemp;
    
    if(buffer_id == LENTH_WINDOW)
        buffer_id = 0;
}
float GetAverageTempCelsius()
{    
    float sum = 0;

    for(uint8_t i = 0; i < LENTH_WINDOW; i++)
        sum += bufferAvg[i];

    return sum/(float)LENTH_WINDOW;
}
void SendTempSerial(unsigned long delay_ms)
{
    static unsigned long timeLastRead = 0;

    if((millis() - timeLastRead) >= delay_ms)
    {
        float temperature = GetAverageTempCelsius();
        
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
        //Serial.println(buffer);                        //Debug 
        SelectCommand(command, stringData);
    }
} 
void SelectCommand(char command, String data)
{
    switch (command)
    {
        case 'L':
        {
            int dutyCycle = data.toInt();
            analogWrite(LOAD_PIN, dutyCycle);
            break;
        }
            
        default:
            break;
    }
}