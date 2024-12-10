#include "contiki.h"
#include "dev/leds.h"
#include "dev/leds.h"
#include "dev/leds.h"
#include "sensors.h"
#include "dht11_sensor.h"

#include <stdio.h>
#define LEDS_RED LEDS_RED
#define LEDS_GREEN LEDS_GREEN
#define LEDS_BLUE LEDS_BLUE
#define RGB_LED_WHITE (LEDS_RED | LEDS_GREEN | LEDS_BLUE)

PROCESS(dht11_example_process, "DHT11 Sensor Example");
AUTOSTART_PROCESSES(&dht11_example_process);

PROCESS_THREAD(dht11_example_process, ev, data)
{
    static struct etimer timer;

    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        if (dht11_sensor.value(DHT11_STATUS) == DHT11_STATUS_OKAY) {
            uint8_t temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
            uint8_t humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

            printf("Temperature: %u C Humidity: %u %%\n", temperature, humidity);

            leds_on(RGB_LED_WHITE);
        } else {
            printf("Error reading the DHT11 sensor\n");
        }

        etimer_reset(&timer);
    }

    PROCESS_END();
}