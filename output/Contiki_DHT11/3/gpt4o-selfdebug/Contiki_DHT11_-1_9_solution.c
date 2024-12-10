#include <contiki.h>
#include <stdio.h>
#include <stdlib.h>
// Mock header file for DHT11 sensor
#ifndef DHT11_SENSOR_H
#define DHT11_SENSOR_H

#define DHT11_READ_INTERVAL (CLOCK_SECOND * 5)

static struct etimer et;

// Function to simulate reading the DHT11 sensor
int dht11_read(int gpio_pin) {
    printf("Reading DHT11 sensor on GPIO pin %d...\n", gpio_pin);
    // Simulate reading temperature and humidity
    int temperature = rand() % 40; // random temperature between 0 and 39
    int humidity = rand() % 100;    // random humidity between 0 and 99
    printf("Temperature: %d C, Humidity: %d%%\n", temperature, humidity);
    return 0;
}

#endif /* DHT11_SENSOR_H */

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data) {
    PROCESS_BEGIN();

    etimer_set(&et, DHT11_READ_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        dht11_read(2); // GPIO pin 2
        etimer_reset(&et);
    }

    PROCESS_END();
}