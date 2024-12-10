#include <contiki.h>
#include <contiki-net.h>
#include <sensors.h>
#include "dht11.h"
#include "dev/gpio.h"

#define DHT11_PIN 2

static struct etimer timer;

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data) {
    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        
        int temperature = 0;
        int humidity = 0;

        if (dht11_read(DHT11_PIN, &temperature, &humidity) == DHT11_SUCCESS) {
            printf("Temperature: %d C, Humidity: %d%%\n", temperature, humidity);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }

        etimer_reset(&timer);
    }

    PROCESS_END();
}