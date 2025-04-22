#include <DHT.h> // Librería para el sensor DHT
#include <Adafruit_Sensor.h> // Librería base para sensores Adafruit
#include <Adafruit_BMP280.h> // Librería para el sensor BMP280
#include <MQ135.h> // Librería para el sensor MQ-135

// Configuración del sensor DHT11
#define DHTPIN 3      // Pin digital donde está conectado el DHT11
#define DHTTYPE DHT11 // Definimos el tipo de sensor como DHT11
DHT dht(DHTPIN, DHTTYPE); // Inicializamos el sensor DHT

// Configuración del sensor BMP280
Adafruit_BMP280 bmp; // Inicializamos el sensor BMP280

// Configuración del sensor MQ-135
#define MQ135_PIN A0 // Pin analógico donde está conectado el sensor MQ-135
MQ135 mq135_sensor(MQ135_PIN);

void setup() {
    Serial.begin(9600); // Iniciamos la comunicación serial

    // Inicializamos el sensor DHT11
    dht.begin();

    // Inicializamos el sensor BMP280
    if (!bmp.begin(0x76)) { // Dirección I2C del BMP280 (puede ser 0x76 o 0x77)
        Serial.println("Error al inicializar el sensor BMP280");
        while (1); // Detenemos el programa si no se encuentra el sensor
    }

    // Inicializamos el sensor MQ-135
    Serial.println("Calibrando el sensor MQ-135...");
    delay(2000); // Espera 20 segundos para estabilizar el sensor
    Serial.println("Calibración completada.");
}

void loop() {
    // Lectura del sensor DHT11
    float humedad = dht.readHumidity();
    float temperaturaDHT = dht.readTemperature(); // Por defecto en grados Celsius

    // Verificamos si las lecturas del DHT11 son válidas
    if (isnan(humedad) || isnan(temperaturaDHT)) {
        Serial.println("Error al leer el sensor DHT11");
    } else {
        // Mostramos los valores del DHT11
        Serial.print("Humedad: ");
        Serial.print(humedad);
        Serial.print("%  Temperatura (DHT11): ");
        Serial.print(temperaturaDHT);
        Serial.println("°C");
    }

    // Lectura del sensor BMP280
    float temperaturaBMP = bmp.readTemperature(); // Temperatura en grados Celsius
    float presion = bmp.readPressure() / 100.0F;  // Presión en hPa
    float altitud = bmp.readAltitude(1013.25);    // Altitud en metros (ajustar presión al nivel del mar si es necesario)

    // Mostramos los valores del BMP280
    Serial.print("Temperatura (BMP280): ");
    Serial.print(temperaturaBMP);
    Serial.println("°C");

    Serial.print("Presión: ");
    Serial.print(presion);
    Serial.println(" hPa");

    Serial.print("Altitud: ");
    Serial.print(altitud);
    Serial.println(" m");

    // Lectura del sensor MQ-135
    float rzero = mq135_sensor.getRZero();
    float correctedRZero = mq135_sensor.getCorrectedRZero(temperaturaDHT, humedad); // Ajustar según temperatura y humedad
    float ppm = mq135_sensor.getPPM();
    float correctedPPM = mq135_sensor.getCorrectedPPM(temperaturaDHT, humedad); // Ajustar según temperatura y humedad

    // Mostramos los valores del MQ-135
    Serial.print("RZero: ");
    Serial.println(rzero);

    Serial.print("RZero corregido: ");
    Serial.println(correctedRZero);

    Serial.print("PPM: ");
    Serial.println(ppm);

    Serial.print("PPM corregido: ");
    Serial.println(correctedPPM);

    Serial.println("-----------------------------");

    delay(2000); // Espera 2 segundos antes de la próxima lectura
}