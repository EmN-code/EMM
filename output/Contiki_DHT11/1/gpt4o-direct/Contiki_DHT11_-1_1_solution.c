#include <stdio.h>
#include "sys/etimer.h"
#include "contiki.h"
#include "board-peripherals.h"

PROCESS(dht11_process, "DHT11 Reading Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
  static struct etimer timer;
  static int temperature, humidity;

  PROCESS_BEGIN();

  printf("DHT11 Sensor Reading Starting\n");

  // Initialize timer to 5-second intervals
  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Restore the timer
    etimer_reset(&timer);

    // Simulate reading from DHT11 on GPIO 2
    nrf_gpio_cfg_input(2, NRF_GPIO_PIN_PULLUP);

    if(dht11_read(&temperature, &humidity) == DHT11_SUCCESS) {
        printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);
    } else {
        printf("Failed to read from DHT11 sensor\n");
    }
  }

  PROCESS_END();
}