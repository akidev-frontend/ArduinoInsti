#uvicorn main:app --reload
#http://localhost:8000/api/datos
from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from threading import Thread
import serial
import time

app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# Variables compartidas
datos_actuales = {
    "temp_dht": 0.0,
    "humedad": 0.0,
    "temp_bmp": 0.0,
    "presion": 0.0,
    "ppm_mq135": 0.0
}

# Hilo para leer datos del Arduino
def leer_arduino():
    try:
        arduino = serial.Serial("COM11", 9600, timeout=10)  # Cambia a tu puerto
        time.sleep(2)
        print("Conectado a Arduino")

        while True:
            linea = arduino.readline().decode().strip()
            print(">>", linea)

            if linea == "error":
                continue

            partes = linea.split(",")
            if len(partes) == 5:
                try:
                    datos_actuales["temp_dht"] = float(partes[0])
                    datos_actuales["humedad"] = float(partes[1])
                    datos_actuales["temp_bmp"] = float(partes[2])
                    datos_actuales["presion"] = float(partes[3])
                    datos_actuales["ppm_mq135"] = float(partes[4])
                except ValueError:
                    print("Datos mal formateados:", linea)
    except serial.SerialException as e:
        print("Error de conexión con Arduino:", e)

# Iniciar el hilo lector
Thread(target=leer_arduino, daemon=True).start()

# Endpoint para obtener los datos
@app.get("/api/datos")
def obtener_datos():
    return datos_actuales
