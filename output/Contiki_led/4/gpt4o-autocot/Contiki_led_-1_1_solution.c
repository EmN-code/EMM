#include "sensors/dht11.h"
#include "contiki.h"
#include <stdio.h>

PROCESS(dht11_sensor_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_sensor_process);

PROCESS_THREAD(dht11_sensor_process, ev, data) {
  static struct etimer timer;
  PROCESS_BEGIN();

  while(1) {
    // Set the timer for 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    // Wait for the timer to expire
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Activate the DHT11 sensor
    SENSORS_ACTIVATE(dht11_sensor);

    // Read temperature and humidity from DHT11
    int temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
    int humidity = dht11_sensor.value(DHT11_SENSOR_HUMIDITY);

    if(temperature != -1 && humidity != -1) {
      printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
    } else {
      printf("Failed to read from DHT11 sensor\n");
    }

    // Deactivate the DHT11 sensor
    SENSORS_DEACTIVATE(dht11_sensor);

    // Turn on the RGB LED
    rgb_led_on();
  }

  PROCESS_END();
}