#include "contiki.h"
#include "dht11-sensor.h"
#include "dev/gpio-hal.h"
#include <stdio.h>

#define SAMPLE_INTERVAL (CLOCK_SECOND * 5)

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
  static struct etimer timer;
  static int temperature;
  static int humidity;

  PROCESS_BEGIN();

  // Make sure the sensor is activated
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 5);

  while(1) {
    // Set timer to sample interval
    etimer_set(&timer, SAMPLE_INTERVAL);

    // Activate the sensor and read temperature and humidity
    dht11_sensor.configure(SENSORS_ACTIVE, 1);
    temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
    humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

    printf("Temperature: %d, Humidity: %d\n", temperature, humidity);

    // Simulating RGB LED on
    printf("RGB LED: ON\n");
    // Here would normally invoke rgb_led_on(); but it's being simulated

    // Wait until timer expires
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
  }

  PROCESS_END();
}