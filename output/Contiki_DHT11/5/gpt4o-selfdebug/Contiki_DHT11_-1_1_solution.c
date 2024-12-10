#include "contiki.h"
#include "dev/dht11-sensor.h"
#include "dev/gpio-hal.h"
#include <stdio.h>

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
  static struct etimer timer;
  static int temperature, humidity;

  PROCESS_BEGIN();

  // Activate the DHT11 sensor
  SENSORS_ACTIVATE(dht11_sensor);

  // Set the GPIO pin for the DHT11 sensor
  GPIO_HAL_PIN_CFG(GPIO_HAL_PIN(2), GPIO_HAL_MODE_INPUT);
  dht11_sensor.configure(SENSORS_HW_INIT, GPIO_HAL_PIN(2));

  while(1) {
    // Set timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    // Wait for the timer event
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read temperature and humidity values
    temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
    humidity = dht11_sensor.value(DHT11_SENSOR_HUMID);

    // Print the sensor values
    printf("Temperature: %d
", temperature);
    printf("Humidity: %d
", humidity);
  }

  SENSORS_DEACTIVATE(dht11_sensor);
  PROCESS_END();
}
