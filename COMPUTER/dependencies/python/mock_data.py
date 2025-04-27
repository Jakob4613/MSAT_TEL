# Import libraries
import serial
from datetime import datetime
import influxdb_client
import json
import time
from influxdb_client.client.write_api import SYNCHRONOUS
import numpy as np


# Set up connection with the influxdb
INFLUX_BUCKET = "Telemetry"
INFLUX_ORG = "Mulerius Satellites"
INFLUX_TOKEN = "B0MalACawFHF2KPJHC5c-JA3c1Ywn5VfEH5J6KanLQb_4ShSPD2pNxRMIpadGwhX8HYexXthrEqdsqdE8rDWXg=="
INFLUX_URL="http://localhost:8086"
client = influxdb_client.InfluxDBClient(url=INFLUX_URL, token=INFLUX_TOKEN, org=INFLUX_ORG)
write_api = client.write_api(write_options=SYNCHRONOUS)

state_reference = 0
START = datetime.now().timestamp()

# Acquire a line from the USB serial feed, parse it into TinyFLUX db.
while True:
	USB_DB_epochtime = datetime.now().timestamp()


	if datetime.now().timestamp() - START > 10:
		state_reference = 1
	if datetime.now().timestamp() - START > 20:
		state_reference = 2
	if datetime.now().timestamp() - START > 30:
		state_reference = 3
	if datetime.now().timestamp() - START > 40:
		state_reference = 4
	if datetime.now().timestamp() - START > 50:
		state_reference = 5

	data = json.loads("{}")
	data["USB_DB_buffer"] = 0
	data["USB_DB_epochtime"] = USB_DB_epochtime
	data["COO"] = float(np.random.normal(450, 2))
	data["TMP"] = float(np.random.normal(25.6, 0.2))
	data["APM"] = float(np.random.normal(960.51, 0.2))
	data["ACX"] = float(np.random.normal(0, 1))
	data["ACY"] = float(np.random.normal(0, 1))
	data["ACZ"] = float(np.random.normal(0, 1))
	data["GYX"] = float(np.random.normal(0, 100))
	data["GYY"] = float(np.random.normal(0, 100))
	data["GYZ"] = float(np.random.normal(0, 100))
	data["MGX"] = float(np.random.normal(0, 500))
	data["MGY"] = float(np.random.normal(0, 500))
	data["MGZ"] = float(np.random.normal(0, 500))
	data["BVO"] = float(np.random.normal(9, 0.2))
	data["CVO"] = float(np.random.normal(9, 0.2))
	data["SPD"] = float(0)
	data["UPT"] = float(datetime.now().timestamp() - START)
	data["RGS"] = float(0)
	data["RAC"] = float(0)
	data["RBR"] = float(0)
	data["RBW"] = float(0)
	data["HAG"] = float(0)
	data["APR"] = float(960)


	if state_reference == 0:
		data["NST"] = 0

	elif state_reference == 1:
		data["NST"] = 1

	elif state_reference == 2:
		data["NST"] = 2

	elif state_reference == 3:
		data["NST"] = 3

	elif state_reference == 4:
		data["NST"] = 4

	elif state_reference == 5:
		data["NST"] = 5


	# Construct an influxdb point. Include measurement for each key and value.
	point = influxdb_client.Point("telemetry_data")
	for key, value in data.items():
		point.field(key, value)
	
	# Write the influxdb point to the influxdb database.
	write_api.write(bucket=INFLUX_BUCKET, record=[point])

	time.sleep(0.2)