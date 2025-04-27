@ECHO OFF
TITLE MS Telemetry

:: Constants
set "DATADIR=C:\tmp\"
set "WORKDIR=%cd%"

:: Start influxdb service
ECHO Starting influxdb...
cd %WORKDIR%\dependencies\influxdb
start cmd.exe /k "influxd.exe"

:: Start Grafana server
ECHO Starting Grafana...
cd %WORKDIR%\dependencies\grafana\bin
start cmd.exe /k "grafana-server.exe"

:: Start usb interface
ECHO Starting Serial interface
cd %WORKDIR%\dependencies\python
start cmd.exe /k "python usb_influxdb_interface.py"

:: Return workfolder
cd %WORKDIR%