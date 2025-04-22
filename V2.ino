#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <MQ135.h>

#define DHTPIN 3
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

Adafruit_BMP280 bmp;
#define MQ135_PIN A0
MQ135 mq135_sensor(MQ135_PIN);

void setup() {
    Serial.begin(9600);
    dht.begin();

    if (!bmp.begin(0x76)) {
        Serial.println("BMP280 no detectado.");
        while (1);
    }

    delay(2000); // Espera para estabilizar sensores
}

void loop() {
    float humedad = dht.readHumidity();
    float tempDHT = dht.readTemperature();
    float tempBMP = bmp.readTemperature();
    float presion = bmp.readPressure() / 100.0F;
    float ppmCorregido = mq135_sensor.getCorrectedPPM(tempDHT, humedad);

    if (!isnan(humedad) && !isnan(tempDHT)) {
        // Enviar datos en una sola línea
        Serial.print(tempDHT); Serial.print(",");
        Serial.print(humedad); Serial.print(",");
        Serial.print(tempBMP); Serial.print(",");
        Serial.print(presion); Serial.print(",");
        Serial.println(ppmCorregido);
    } else {
        Serial.println("error");
    }

    delay(10000);
}
