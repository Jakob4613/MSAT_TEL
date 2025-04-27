from influxdb_client import InfluxDBClient
import csv
from collections import defaultdict
from datetime import datetime


def export_telemetry(START_TIMESTAMP, output_file, bucket, org, token, url, measurement):

    # Calculate the amount of minutes that have been logged, so we only extract that part from the influxDB database.
    minutes_logged = int((datetime.now().timestamp() - START_TIMESTAMP) / 60) + 1

    # --- InfluxDB Client ---
    client = InfluxDBClient(url=url, token=token, org=org)
    query_api = client.query_api()

    query = f'''
    from(bucket: "{bucket}")
      |> range(start: -{minutes_logged}m)
      |> filter(fn: (r) => r._measurement == "{measurement}")
      |> pivot(rowKey:["_time"], columnKey: ["_field"], valueColumn: "_value")
    '''

    # --- Execute Query ---
    result = query_api.query(query)

    # --- Organize Records ---
    rows_by_time = []
    columns_set = set()

    for table in result:
        for record in table.records:
            values = record.values
            row = {"time": values["_time"].isoformat()}
            for k, v in values.items():
                if k.startswith("_") or k in ["result", "table", "_measurement", "_start", "_stop", "_field", "_value"]:
                    continue
                row[k] = v
                columns_set.add(k)
            rows_by_time.append(row)

    # Ensure columns are sorted and consistent
    columns = ["time"] + sorted(c for c in columns_set if c != "time")

    # --- Write to CSV ---
    with open(output_file, mode='w', newline='') as csv_file:
        writer = csv.DictWriter(csv_file, fieldnames=columns)
        writer.writeheader()
        for row in rows_by_time:
            writer.writerow({col: row.get(col, "") for col in columns})

    print(f"Program quitting. Saved the collected telemetry-log in {output_file} for the last {minutes_logged} minutes.")
    exit()