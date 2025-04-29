# Imports
from pyfiglet import Figlet
import serial
import keyboard
import serial.tools.list_ports
from datetime import datetime
import influxdb_client
import json
import time
from statistics import mean
from utils import *
from influxdb_client.client.write_api import SYNCHRONOUS


# Constants
ACCELEROMETER_SENSITIVITY = 2048 
GYROSCOPE_SENSITIVITY = 65.5
RUNNING_AVG_SPD_COUNT = 4
R1 = 2400
R2 = 1000
VOLTAGE_SCALING = (R1 + R2) / R2 * 3.3 * 257 / 65535
START_TIMESTAMP = datetime.now().timestamp()
output_path = f"{int(START_TIMESTAMP)}_MSAT1.csv"

# InfluxDB Setup
INFLUX_BUCKET = "Telemetry"
INFLUX_ORG = "Mulerius Satellites"
INFLUX_TOKEN = "B0MalACawFHF2KPJHC5c-JA3c1Ywn5VfEH5J6KanLQb_4ShSPD2pNxRMIpadGwhX8HYexXthrEqdsqdE8rDWXg=="
INFLUX_URL = "http://localhost:8086"
INFLUX_MEASUREMENT = "telemetry_data"
client = influxdb_client.InfluxDBClient(url=INFLUX_URL, token=INFLUX_TOKEN, org=INFLUX_ORG)
write_api = client.write_api(write_options=SYNCHRONOUS)

# Serial setup
SER_BAUDRATE = 115200
serial_connected = False

# Variables for speed averaging
last_HAG = 0
last_time = START_TIMESTAMP

# Welcome message
print(Figlet(font='slant').renderText('MSAT1 DATA'))
print("Available COM Ports:")
for port, desc, _ in sorted(serial.tools.list_ports.comports()):
    print(f"{port}: {desc}")
SER_PORT = input("Select COM port: ")
print("Telemetry started logging. Press Q to terminate and export data.")
print("____________________________________________________________\n")

# Main loop
while True:
    current_time = datetime.now().timestamp()

    # Reconnect logic
    while not serial_connected:
        try:
            ser = serial.Serial(port=SER_PORT, baudrate=SER_BAUDRATE, timeout=None)
            serial_connected = True
            print(f"{current_time} - SUCCESS: Serial device connected")
        except Exception as e:
            print(f"{current_time} - ERROR: Serial device not detected on {SER_PORT}")
            print(e)
            time.sleep(1)

    # Read from serial
    try:
        if ser.in_waiting > 0:
            received_line = str(ser.readline())[2:][:-5]

            try:
                data_timestamp = datetime.now().timestamp()
                data = json.loads(received_line)

                # Append metadata
                data["USB_DB_buffer"] = ser.in_waiting
                data["USB_DB_epochtime"] = data_timestamp

                # Unit conversions
                data["BVO"] = data["BVO"] * VOLTAGE_SCALING
                data["CVO"] = data["CVO"] * VOLTAGE_SCALING
                data["ACX"] /= ACCELEROMETER_SENSITIVITY
                data["ACY"] /= ACCELEROMETER_SENSITIVITY
                data["ACZ"] /= ACCELEROMETER_SENSITIVITY
                data["GYX"] /= GYROSCOPE_SENSITIVITY
                data["GYY"] /= GYROSCOPE_SENSITIVITY
                data["GYZ"] /= GYROSCOPE_SENSITIVITY
                data["APM"] = (data["APM"] / 6 + 95000) / 100
                data["APR"] = (data["APR"] / 6 + 95000) / 100
                data["TMP"] /= 5
                data["HAG"] /= 120
                data["UPT"] = data["UPT"] * 10

                delta_time = data_timestamp - last_time
                data["SPD"] = ((data["HAG"] - last_HAG) / delta_time) * 3.6 if delta_time > 0 else 0

                last_HAG = data["HAG"]
                last_time = data_timestamp

            except Exception as e:
                if received_line.startswith(" - WARNING"):
                    print(f"{current_time}{received_line}")
                else:
                    print(f"{current_time} - WARNING: Malformed input string: {received_line}")
                print(e)
                continue

            try:
                point = influxdb_client.Point(INFLUX_MEASUREMENT)
                for k, v in data.items():
                    point.field(k, v)
                write_api.write(bucket=INFLUX_BUCKET, record=[point])
            except Exception as e:
                print(f"{current_time} - ERROR: Failed to write to InfluxDB.")
                print(e)

    except Exception as e:
        print(f"{current_time} - ERROR: Serial error. Reconnecting.")
        print(e)
        serial_connected = False

    if keyboard.is_pressed('q'):
        export_telemetry(
            START_TIMESTAMP,
            output_path,
            INFLUX_BUCKET,
            INFLUX_ORG,
            INFLUX_TOKEN,
            INFLUX_URL,
            INFLUX_MEASUREMENT
        )
        break
