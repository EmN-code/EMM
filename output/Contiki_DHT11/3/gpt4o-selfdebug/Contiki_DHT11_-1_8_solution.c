// Description of the problem and approach
// This program initializes the nRF52840 board and reads the DHT11 sensor every 5 seconds.
// The DHT11 sensor is connected to GPIO pin 2. The program is implemented using the Contiki OS.
// Include necessary Contiki and nRF52840 specific header files in alphabetical order.

#include "contiki.h"
#include "dev/leds.h"
#include "dev/dht11-sensor.h"
#include "nrf.h"
#include "nrf_gpio.h"
#include <stdio.h>

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
  static struct etimer timer;
  PROCESS_BEGIN();

  // Configuring GPIO pin for DHT11
  nrf_gpio_cfg_input(2, NRF_GPIO_PIN_NOPULL);

  // Initialize DHT11 sensor
  SENSORS_ACTIVATE(dht11_sensor);

  while(1) {
    // Set a timer for 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    // Read DHT11 values
    int temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
    int humidity = dht11_sensor.value(DHT11_SENSOR_HUMIDITY);

    // Print the values
    printf("Temperature: %d C, Humidity: %d %\n", temperature, humidity);

    // Wait for timer event
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
  }

  PROCESS_END();
}