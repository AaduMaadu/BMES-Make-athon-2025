import serial
import requests
import time

# Serial port configuration
ser = serial.Serial('COM8', 9600)  #Arduino's serial port
time.sleep(2)  # Wait for the serial connection to initialize

# Server URL
server_url = "http://localhost:8080"  # Replace with your server URL

def send_data_to_server(temperature, humidity):
    payload = {
        "temperature": temperature,
        "humidity": humidity
    }
    try:
        response = requests.post(server_url, data=payload)
        print(f"Data sent to server. Response: {response.status_code}")
    except Exception as e:
        print(f"Error sending data: {e}")

while True:
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').strip()
        print(f"Received from Arduino: {line}")

        # Parse the data (e.g., "Temperature: 25.5, Humidity: 60.0")
        # if "Temperature:" in line and "Humidity:" in line:
        #     parts = line.split(", ")
        #     temperature = float(parts[0].split(": ")[1])
        #     humidity = float(parts[1].split(": ")[1])

        #     # Send data to the server
        #     send_data_to_server(temperature, humidity)

        send_data_to_server

    time.sleep(2)  # Wait 2 seconds before next read