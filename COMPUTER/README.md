# Functionality
The core responsibility of the computer code is reading the stream of serial data from the USBANT and storing this in a database. In this reading process certain measurements are also transformed. These transformations depend on the units and scalars applied on the flight computer, which in turn depend on the resolution and range of the variable and the amount of dedicated bytes in the telemetry package. Additionally it takes care of dashboarding the acquired telemetry data. When the software is properly terminated, a CSV file with the acquired telemetry data during the session is exported.
 - Receiving serial data from USBANT
 - Transforming certain telemetry measurements to make the units correct.
 - Storing received telemetry packets in a database
 - Displaying database in a dashboard
 - Write an export of the acquired telemetry packets.

# Requirements
Although only tested on a Lenovo P51 laptop with python 3.12 installed, the code should be executable on every windows machine, given the appropriate requirements are fulfilled. In short:
 - Download a standalone executable of Grafana online, and place it in the Grafana folder, such that the `/bin/grafana.exe` file is located under the grafana folder.
 - Download a standalone executable of InfluxDB, and place it in the InfluxDB folder, such that the `influxdb.exe` file is located directly under the InfluxDB folder.
 - Install an appropriate version of Python and install all of the required packages listed in the requirements.txt file.

# Execution
Connect the USBANT device to a usb port of your computer. This makes sure that the telemetry backend can receive telemetry data transmitted by the satellite. Additionally make sure an appropriate 2.4Ghz antenna is connected to the SMA connector of the USBANT.
Launch the batch script file named `STARTUP_TELEMETRY.bat`. This opens up 3 different command prompt. The first prompt is from the python-backend, and this is responsible for decoding the received json packages from the serial connection and writing them to the InfluxDB database. Another prompt is opened for an InfluxDB server, which is responsible for receiving and storing the telemetry packets. The last prompt runs the Grafana server, which is responsible for hosting web-based dashboards. 
Wait until both the Grafana and InfluxDB software seems to be up and running. This can take up to a minute.
Use a webbrowser to open up http://localhost:8086 to open the web-interface of InfluxDB. In here, you need to make sure that a bucket called `telemetry` is available. If not, create the bucket.
In the InfluxDB web-interface, check your credentials, API key, organization name and other credentials that are listed in the `python-backend/usb_influxdb_interface.py` file. Edit the credentials in this python file according to the InfluxDB setup.
Interact with the command prompt running the python-backend. This prompt lists all of the available COM (serial) ports. If the USBANT is correctly connected it will be listed. Instruct the python-backend about the COM port you want to use by returning `COMX`, where X is the specific COM-port.
Use a webbrowser to open http://localhost:3000 to open the web-interface of Grafana. After registering/logging in, go to data-sources under the preferences and check if there is an instance of InfluxDB. If this is not the case, create a new instance using the credentials that you have extracted in step 5.
Open the dashboards section of Grafana, create your dashboards and monitor the telemetry. For this last step it may be beneficial to look up some basic tutorials of Grafana to better understand how it functions.
When you’re done using the telemetry software, first terminate the python-backend by pressing Q. This will take a while, as all of the InfluxDB-registered data-packets are exported to a CSV and are stored in the python-backend folder. When the terminal is closed, you can close both the Grafana and InfluxDB terminal.


# Improvements
 - Currently, the python-backend writes all of the received telemetry packages to the InfluxDB database. At times, this can be happening at a rate of 60 to 70 Hz. This slows down Grafana significantly, and limits rendering larger portions of data. This is not easily fixed as reducing the amount of datapoints written to influxDB effectively throws out packets by not storing them. 