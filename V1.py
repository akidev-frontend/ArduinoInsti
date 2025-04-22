from fastapi import FastAPI
from pydantic import BaseModel

app = FastAPI()

# Variables internas
datos_actuales = {
    "temperatura": 2.0,
    "humedad": 8.0
}

# Modelo de entrada para actualizar datos
class DatosEntrada(BaseModel):
    temperatura: float
    humedad: float

# POST para actualizar las variables
@app.post("/api/actualizar")
def actualizar_datos(data: DatosEntrada):
    datos_actuales["temperatura"] = data.temperatura
    datos_actuales["humedad"] = data.humedad
    return {"mensaje": "Datos actualizados"}

# GET para obtener los datos actuales
@app.get("/api/datos")
def obtener_datos():
    return datos_actuales
