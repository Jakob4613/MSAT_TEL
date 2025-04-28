# Import libraries
from pyfiglet import Figlet
import serial
import keyboard
import serial.tools.list_ports
from datetime import datetime
import influxdb_client
import json
import time
from utils import *
from influxdb_client.client.write_api import SYNCHRONOUS


# Speed calculation variables.
ACCELEROMETER_SENSITIVITY = 16 #g
GYROSCOPE_SENSITIVITY = 5 #TODO!
RUNNING_AVG_SPD_COUNT = 4
last_HAG = 0
last_time = 0
list_speeds = [0] * RUNNING_AVG_SPD_COUNT


# Set up connection with the influxdb
INFLUX_BUCKET = "Telemetry"
INFLUX_ORG = "Mulerius Satellites"
INFLUX_TOKEN = "B0MalACawFHF2KPJHC5c-JA3c1Ywn5VfEH5J6KanLQb_4ShSPD2pNxRMIpadGwhX8HYexXthrEqdsqdE8rDWXg=="
INFLUX_URL = "http://localhost:8086"
INFLUX_MEASUREMENT = "telemetry_data"
client = influxdb_client.InfluxDBClient(url=INFLUX_URL, token=INFLUX_TOKEN, org=INFLUX_ORG)
write_api = client.write_api(write_options=SYNCHRONOUS)
START_TIMESTAMP = datetime.now().timestamp()
output_path = str(int(START_TIMESTAMP)) + "_MSAT1.csv"


# Set up connection with the serial feed.
SER_BAUDRATE = 115200
serial_connected = False


# Greet the user and ask the COM port of the usbant.
f = Figlet(font='slant')
print(f.renderText('MSAT1 DATA', ))

print("To which port is the USB-ANT device connected? The following COM ports are available:")
ports = serial.tools.list_ports.comports()
for port, desc, hwid in sorted(ports):
	print("{}: {}".format(port, desc))

SER_PORT = input("Which COM port do you wish to select? ")
print("Telemetry started logging. Press Q to terminate the program and export the received data.")
print("____________________________________________________________\n")


# Acquire a line from the USB serial feed, parse it into TinyFLUX db.
while True:
	USB_DB_epochtime = datetime.now().timestamp()

	# Check if the board is connected, if disconnected: retry every second.
	while not serial_connected:
		try:
			ser = serial.Serial(port=SER_PORT, baudrate=SER_BAUDRATE, timeout=None)
			serial_connected = True
			print(str(USB_DB_epochtime) + " - SUCCESS: Serial device connected")
		except Exception as error:
			print(str(USB_DB_epochtime) + " - ERROR: Serial device not detected on " + SER_PORT)
			print(error)
			time.sleep(1)


	# Read the serial feed and check the buffer & current time.
	try:
		USB_DB_buffer = ser.in_waiting
		if USB_DB_buffer > 0:
			received_line = str(ser.readline())[2:][:-5]

			# Read the json_line coming over the serial stream.
			# Store this json line in a variable used for writing to influxdb later.
			try:
				# Load the received package.
				data_timestamp = datetime.now().timestamp()
				data = json.loads(received_line)

				# Append usb-influxdb interface verbose information.
				data["USB_DB_buffer"] = USB_DB_buffer
				data["USB_DB_epochtime"] = data_timestamp

				# Extract telemetry data
				data["BVO"] = ((data["BVO"] * 3.3) / 255) * 11
				data["CVO"] = ((data["CVO"] * 3.3) / 255) * 11
				data["ACX"] = data["ACX"] / ACCELEROMETER_SENSITIVITY
				data["ACY"] = data["ACY"] / ACCELEROMETER_SENSITIVITY
				data["ACZ"] = data["ACZ"] / ACCELEROMETER_SENSITIVITY
				data["GYX"] = data["GYX"] / GYROSCOPE_SENSITIVITY
				data["GYY"] = data["GYY"] / GYROSCOPE_SENSITIVITY
				data["GYZ"] = data["GYZ"] / GYROSCOPE_SENSITIVITY
				data["APM"] = ((data["APM"] / 6) + 95000) / 100
				data["APR"] = ((data["APR"] / 6) + 95000) / 100
				data["TMP"] = data["TMP"] / 1
				data["HAG"] = data["HAG"] / 120
				data["SPD"] = ((data["HAG"] - last_HAG) / (data_timestamp - last_time)) * 3.6


				# take running average for the speed, to make it less jittery.
				for i in range(len(list_speeds)-1):
					list_speeds[i] = list_speeds[i+1]
				list_speeds[-1] = data["SPD"]
				data["SPD_RAVG"] = mean(list_speeds)

				# Keep track of the previous HAG and time for inferring the vertical speed.
				last_HAG = data["HAG"]
				last_time = data_timestamp

			
			# If the incoming line could not be parsed.
			except Exception as error:
				if received_line == " - WARNING: Malformed telemetry-package received.":
					print(str(USB_DB_epochtime) + received_line)
				else:
					print(str(USB_DB_epochtime) + " - WARNING: Malformed input string: " + received_line)
				
				print(error)
				continue


			# Try to write the Received telemetry package to the influxdb database.
			try:
				# Construct an influxdb point. Include measurement for each key and value.
				point = influxdb_client.Point(INFLUX_MEASUREMENT)
				for key, value in data.items():
					point.field(key, value)
				
				# Write the influxdb point to the influxdb database.
				write_api.write(bucket=INFLUX_BUCKET, record=[point])
			
			except Exception as error:
				print(str(USB_DB_epochtime) + " - ERROR: Could not write measurement to the InfluxDB backend.")
				print(error)


	# If any serial exception occurred, set the serial connected to false.
	# This triggers the reconnection routine.
	except Exception as error:
		print(str(USB_DB_epochtime) + " - ERROR: Serial error. Reconnecting.")
		print(error)
		serial_connected = False


	# IF during the loop the q button is pressed by the user, exit and store the telemetry log.
	if keyboard.is_pressed('q'):
		export_telemetry(START_TIMESTAMP, output_path, INFLUX_BUCKET, INFLUX_ORG, INFLUX_TOKEN, INFLUX_URL, INFLUX_MEASUREMENT)


#json input
#{"COO":65280,"APM":0,"ACX":0,"ACY":0,"ACZ":0,"GYX":0,"GYY":0,"GYZ":65535,"MGX":65535,"MGY":65535,"MGZ":65535,"TMP":255,"BVO":255,"CVO":255,"RBW":1,"RBR":1,"RAC":1,"RGS":1,"NST":15,"SPD":511,"UPT":2047,"HAG":16383,"APR":15423}
