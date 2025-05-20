// Function to fetch data from the API and update the HTML
async function fetchDataAndUpdateHTML() {
    try {
        const response = await fetch("http://localhost:8000/api/datos");
        const TemperaturaGlobal = document.getElementById("Temperatura-global");
        const Humitat = document.getElementById("Humitat");
        const Pressió = document.getElementById("Pressió-atmosferica");
        const PPM = document.getElementById("PPM");
        // Check if the response is OK
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }

        // Parse the JSON data
        const data = await response.json();
        console.log(data);

        TemperaturaGlobal.innerHTML = "Temperatura: " + ((data.temp_dht + data.temp_bmp) / 2).toFixed(2) + " Cº";
        Humitat.innerHTML = "Humitat: " + data.humedad + "%";
        Pressió.innerHTML = "Pressió atmosferica: " + data.presion + " hPa";
        PPM.innerHTML = "PPM: " + (data.ppm_mq135);

        
        

    } catch (error) {
        console.error("Error fetching data:", error);
    }
}

// Call the function when the page loads
document.addEventListener("DOMContentLoaded", fetchDataAndUpdateHTML);