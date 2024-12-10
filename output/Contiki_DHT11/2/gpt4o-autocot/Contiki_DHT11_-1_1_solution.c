# This code reads data from a DHT11 sensor connected to GPIO 2 on an NRF52840 microcontroller every five seconds using the Contiki OS.

#include "sensors.h"
#include "contiki.h"

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data) {
  static struct etimer timer;

  PROCESS_BEGIN();

  // Initialize the DHT11 sensor
  SENSORS_ACTIVATE(dht11_sensor);

  while(1) {
    // Set the timer to expire after 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    // Wait for the timer to expire
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read the sensor value
    int temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
    int humidity = dht11_sensor.value(DHT11_SENSOR_HUMIDITY);

    // Check if the values are valid
    if(temperature != -1 && humidity != -1) {
      printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
             temperature / 10, temperature % 10,
             humidity / 10, humidity % 10);
    }
    else {
      printf("Failed to read from DHT11 sensor!\n");
    }
  }

  PROCESS_END();
}