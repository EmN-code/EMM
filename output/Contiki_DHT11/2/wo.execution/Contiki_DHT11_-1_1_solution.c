#include "contiki.h"
#include "dev/gpio.h"
#include "dev/sensor.h"
#include <stdio.h>

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

static struct etimer timer;

#define DHT11_PIN 2

PROCESS_THREAD(dht11_process, ev, data)
{
  PROCESS_BEGIN();
  
  // Initialize the GPIO pin connected to the DHT11
  gpio_set_input(DHT11_PIN);

  while (1) {
    // Set timer for 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    // Simulate reading from DHT11
    int temp = random_rand() % 100; // Random temperature value for demonstration
    int hum = random_rand() % 100;  // Random humidity value for demonstration

    // Print the simulated sensor value
    printf("Temperature: %d C, Humidity: %d%%\n", temp, hum);

    // Wait for the timer to expire
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
  }

  PROCESS_END();
}
