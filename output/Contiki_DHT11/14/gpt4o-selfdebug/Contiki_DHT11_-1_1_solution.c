#include <contiki.h>
#include <sys/etimer.h>
#include <stdio.h>
#include <sensors.h>

PROCESS(dht11_reader, "DHT11 Sensor Reader");
AUTOSTART_PROCESSES(&dht11_reader);

PROCESS_THREAD(dht11_reader, ev, data)
{
  static struct etimer timer;
  PROCESS_BEGIN();

  // Initialize the dht11 sensor
  SENSORS_ACTIVATE(dht11_sensor);

  while (1) {
    // Set the timer for 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    // Wait until the timer expires
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read the sensor data
    int temperature = dht11_sensor.value(0);
    int humidity = dht11_sensor.value(1);

    if (temperature != -1 && humidity != -1) {
      printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);
    } else {
      printf("Failed to read from DHT11 sensor\n");
    }
  }

  SENSORS_DEACTIVATE(dht11_sensor);
  PROCESS_END();
}
