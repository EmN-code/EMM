
#include "contiki.h"
#include "dev/leds.h"
#include "dev/dht11-sensor.h"
#include <stdio.h>

#define INTERVAL (5 * CLOCK_SECOND)

PROCESS(dht11_led_process, "DHT11 and LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

PROCESS_THREAD(dht11_led_process, ev, data)
{
    static struct etimer timer;
    static int temperature, humidity;

    PROCESS_BEGIN();

    SENSORS_ACTIVATE(dht11_sensor);

    etimer_set(&timer, INTERVAL);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        if (dht11_sensor.value(SENSORS_ACTIVE) == 1) {
            temperature = dht11_sensor.value(SENSOR_VALUE_TEMP);
            humidity = dht11_sensor.value(SENSOR_VALUE_HUMIDITY);

            printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);

            leds_set(LEDS_RED); // Turn on the RGB LED
        }

        etimer_reset(&timer);
    }

    PROCESS_END();
}
