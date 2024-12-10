#include "contiki.h"
#include "dev/leds.h"
#include "contiki-net.h"
#include <stdio.h>

#define DHT11_VALUE_TEMPERATURE_INTEGER    (2)
#define DHT11_VALUE_HUMIDITY_INTEGER       (0)

PROCESS(dht11_led_process, "DHT11 and LED process");
AUTOSTART_PROCESSES(&dht11_led_process);

PROCESS_THREAD(dht11_led_process, ev, data)
{
    static struct etimer timer;
    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        int temperature = DHT11_VALUE_TEMPERATURE_INTEGER; // Simulating temperature
        int humidity = DHT11_VALUE_HUMIDITY_INTEGER; // Simulating humidity

        printf("Temperature: %d, Humidity: %d\n", temperature, humidity);

        leds_on(LEDS_RED);

        etimer_reset(&timer);
    }

    PROCESS_END();
}