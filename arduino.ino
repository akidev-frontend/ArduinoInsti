#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <MQ135.h>
#include <LiquidCrystal.h> // Incluir la librería para la pantalla LCD

#define DHTPIN 3
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

Adafruit_BMP280 bmp;
#define MQ135_PIN A0
MQ135 mq135_sensor(MQ135_PIN);

// Configuración de los pines de la pantalla LCD
LiquidCrystal lcd(12, 11, 9, 10, 7, 6); // RS, E, D4, D5, D6, D7

unsigned long previousMillis = 0;
const long interval = 10000;     // intervalo de 10 segundos para actualizar datos
int displayMode = 0;             // para alternar entre distintas vistas

void setup() {
    Serial.begin(9600);
    dht.begin();
    lcd.begin(16, 2); // Inicializar la pantalla LCD con 16 columnas y 2 filas
    lcd.print("Iniciando..."); // Mensaje inicial en la pantalla

    if (!bmp.begin(0x76)) {
        Serial.println("BMP280 no detectado.");
        lcd.clear();
        lcd.print("BMP280 Error");
        while (1);
    }

    delay(2000); // Espera para estabilizar sensores
    lcd.clear();
}

void loop() {
    unsigned long currentMillis = millis();
    
    // Lectura cada 10 segundos
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        
        float humedad = dht.readHumidity();
        float tempDHT = dht.readTemperature();
        float tempBMP = bmp.readTemperature();
        float presion = bmp.readPressure() / 100.0F;
        float ppmCorregido = mq135_sensor.getCorrectedPPM(tempDHT, humedad);

        if (!isnan(humedad) && !isnan(tempDHT)) {
            // Enviar datos en una sola línea al monitor serie (para Python)
            Serial.print(tempDHT); Serial.print(",");
            Serial.print(humedad); Serial.print(",");
            Serial.print(tempBMP); Serial.print(",");
            Serial.print(presion); Serial.print(",");
            Serial.println(ppmCorregido);

            // Mostrar datos en la pantalla LCD alternando entre 2 vistas
            lcd.clear();
            
            // Alternamos entre 3 modos de visualización
            switch (displayMode) {
                case 0:
                    // Temperatura y humedad
                    lcd.setCursor(0, 0);
                    lcd.print("Temp: ");
                    lcd.print(tempDHT);
                    lcd.print(" C");
                    
                    lcd.setCursor(0, 1);
                    lcd.print("Hum: ");
                    lcd.print(humedad);
                    lcd.print(" %");
                    break;
                    
                case 1:
                    // Presión y temperatura BMP
                    lcd.setCursor(0, 0);
                    lcd.print("Pres: ");
                    lcd.print(presion);
                    lcd.print(" hPa");
                    
                    lcd.setCursor(0, 1);
                    lcd.print("TempB: ");
                    lcd.print(tempBMP);
                    lcd.print(" C");
                    break;
                    
                case 2:
                    // Calidad del aire
                    lcd.setCursor(0, 0);
                    lcd.print("Calidad Aire:");
                    
                    lcd.setCursor(0, 1);
                    lcd.print("CO2: ");
                    lcd.print(ppmCorregido);
                    lcd.print(" ppm");
                    break;
            }
            
            // Cambiar al siguiente modo para la próxima actualización
            displayMode = (displayMode + 1) % 3;
            
        } else {
            Serial.println("error");
            lcd.clear();
            lcd.print("Error lectura");
            lcd.setCursor(0, 1);
            lcd.print("sensores");
        }
    }
    
    // Aquí puedes agregar código para verificar entradas o hacer otras tareas
    // mientras esperas la siguiente actualización de 10 segundos
}