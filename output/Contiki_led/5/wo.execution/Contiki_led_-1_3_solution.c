#include "dht11-sensor.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"
#include "button-hal.h"
#include "contiki-net.h"
#include <stdio.h>

PROCESS(nrf52840_process, "NRF52840 DHT11 LED Process");
AUTOSTART_PROCESSES(&nrf52840_process);

PROCESS_THREAD(nrf52840_process, ev, data)
{
    static struct etimer timer;
    PROCESS_BEGIN();

    printf("Starting NRF52840 DHT11 LED Process\n");
    SENSORS_ACTIVATE(dht11_sensor);

    while(1) {
        /* Set timer to 5 seconds */
        etimer_set(&timer, CLOCK_SECOND * 5);

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
        int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

        if (humidity != -1 && temperature != -1) {
            printf("Humidity: %d, Temperature: %d\n", humidity, temperature);
            // Turn on RGB LED here based on sensor values
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }
    }

    PROCESS_END();
}