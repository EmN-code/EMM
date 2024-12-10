#include "contiki-default-conf.h"
#include <stdio.h>
#include "dev/gpio.h"
#include "dev/dht11.h"

#define DHT11_PIN 2

static struct etimer timer;

PROCESS(dht11_process, "DHT11 Sensor Reading Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data) {
  PROCESS_BEGIN();

  // Initialize GPIO for DHT11
  gpio_set_output_enable(DHT11_PIN, 1);
  gpio_set_value(DHT11_PIN, 1);

  while(1) {
    etimer_set(&timer, CLOCK_SECOND * 5);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    
    // Read data from DHT11
    int16_t temperature, humidity;
    if (dht11_read(DHT11_PIN, &temperature, &humidity) == DHT11_SUCCESS) {
      printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
    } else {
      printf("Failed to read from DHT11 sensor.\n");
    }
  }

  PROCESS_END();
}