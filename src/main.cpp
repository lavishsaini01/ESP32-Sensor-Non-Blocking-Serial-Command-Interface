#include <Arduino.h>
#include "DHT.h"

#define DHTPIN 35
#define DHTTYPE DHT11

const int LDR_PIN = 12;
const int POT_PIN = 4;
const int NUM_SAMPLES = 10;

unsigned long interval = 2000;
unsigned long lastmillis = 0;

// Separate timer configuration for DHT11 (minimum 2000 ms required)
unsigned long lastDHTMillis = 0;
const unsigned long DHT_INTERVAL = 2000;

// Command buffer configuration
const int BUFFER_SIZE = 32;
char cmdBuffer[BUFFER_SIZE];
unsigned int bufIndex = 0;

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);


void reandandReport_LDR(int PIN)
{
    int reading = digitalRead(PIN);

    if (reading == HIGH)
    {
      Serial.print("DARK DETECTED  |  DO:");
    }
    else {
      Serial.print("LIGHT DETECTED  |  DO:");
    }
    Serial.print(reading);
}


void readandReport_ADC (int NUM_SAMPLES, int POT_PIN)
{
    long total = 0;
    int maximum_ADC_value =4095;
    int adcValue = analogRead(POT_PIN);

    for(int i = 0; i<NUM_SAMPLES; i++ ){

      total += analogRead(POT_PIN);
    }

    float average = float(total)/NUM_SAMPLES;

    float percentage = (average / maximum_ADC_value) * 100;
    float voltage = (average / maximum_ADC_value) * 3.3 ;

    // Serial.print("  ||  ADC = ");
    // Serial.print(adcValue);

    Serial.print(" | Average ADC = ");
    Serial.print(average);

    Serial.print(" | Percentage = ");
    Serial.print(percentage);
    Serial.print("%");

    Serial.print(" | Voltage = ");
    Serial.print(voltage);
    Serial.println("V");

}


void readandReport_DHT11()
{
  float humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float tempC = dht.readTemperature();
  // Read temperature as Fahrenheit
  float tempF = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again)
  if (isnan(humidity) || isnan(tempC) || isnan(tempF)) {
    Serial.println("ERROR: Failed to read from DHT11 sensor! Check wiring.");
    return;
  }

  // Compute heat index in Fahrenheit
  // float hif = dht.computeHeatIndex(tempF, humidity);
  // Compute heat index in Celsius
  float hic = dht.computeHeatIndex(tempC, humidity, false);

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("% | Temperature: ");
  Serial.print(tempC);
  Serial.print("°C (");
  Serial.print(tempF);
  Serial.print("°F) | Heat Index: ");
  Serial.print(hic);
  Serial.println("°C");
}


void processCommand(String command)
{
    
    // String command = Serial.readStringUntil('\n');
    // command.trim();
    Serial.println(command);

        if (command == "help")
        {
            Serial.println("Available Commands :");
            Serial.println("help");
            Serial.println("status");
            Serial.println("hello");
         
            Serial.println("set interval");
        }

        else if (command == "status")
        {
            Serial.print("Current interval: ");
            Serial.print(interval);
            Serial.println(" ms");
        }
    
        else if (command.startsWith("set interval "))
        {
            String interval_string = command.substring(13);
            interval_string.trim(); // Removes hidden newline or carriage return characters

            bool isValid = true;
            if (interval_string.length() == 0) {
                isValid = false;
            } 
            else {
                for (unsigned int i = 0; i < interval_string.length(); i++) {
                    if (!isDigit(interval_string[i])) {
                        isValid = false;
                        break;
                    }
                }
            }

            if (isValid) {
                long newinterval = interval_string.toInt();
                if (newinterval >= 100 && newinterval <= 10000) {
                    interval = newinterval;
                    Serial.print("New interval : ");
                    Serial.print(newinterval);
                    Serial.println(" ms");
                } 
                else {
                    Serial.println("Error: Value out of range (must be 100 to 10000 ms).");
                }
            } 
            else {
                Serial.println("Error: Invalid input. Only integers are accepted.");
            }
        }
    }



void commands()
{
    // Process all currently available characters without blocking the loop
    while (Serial.available() > 0)
    {
        char c = Serial.read();

        // Check if the character is a newline or carriage return (command complete)
        if (c == '\n' || c == '\r')
        {
            if (bufIndex > 0)
            {
                cmdBuffer[bufIndex] = '\0'; // Null-terminate the string
                String command = String(cmdBuffer);
                command.trim();
                
                processCommand(command);
                
                bufIndex = 0; // Reset buffer index for the next command
            }
        }
        else
        {
            // Check buffer size to prevent overflow before writing
            if (bufIndex < BUFFER_SIZE - 1)
            {
                cmdBuffer[bufIndex++] = c;
            }
            else
            {
                Serial.println("Error: Command buffer overflow!");
                bufIndex = 0; // Reset buffer on overflow
            }
        }
    }
}


void setup()
{
  Serial.begin(115200);
  pinMode(LDR_PIN, INPUT);
  dht.begin();
  delay(2000); // DHT11 requires 1-2 seconds to stabilize on boot
}


void loop()
{
   commands();

   if (millis() - lastmillis >= interval)
   {
        lastmillis= millis();
        reandandReport_LDR(LDR_PIN);
        readandReport_ADC(NUM_SAMPLES, POT_PIN);
   }

   if (millis() - lastDHTMillis >= DHT_INTERVAL)
    {
        lastDHTMillis = millis();
        readandReport_DHT11();
    }
}