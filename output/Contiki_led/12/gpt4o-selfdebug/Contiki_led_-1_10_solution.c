#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki.h"
#include "dev/leds.h"
#include <stdio.h>

PROCESS(dht11_led_process, "DHT11 and LED process");
AUTOSTART_PROCESSES(&dht11_led_process);

PROCESS_THREAD(dht11_led_process, ev, data)
{
    static struct etimer timer;
    PROCESS_BEGIN();

    while (1) {
        etimer_set(&timer, CLOCK_SECOND * 5);

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Here we would read the DHT11 sensor using dht11_read() method
        // int temperature = dht11_read(DHT11_VALUE_TEMPERATURE_INTEGER);
        // int humidity = dht11_read(DHT11_VALUE_HUMIDITY_INTEGER);
        printf("Reading DHT11 sensor\n");

        // Simulate turning the RGB LED magenta
        printf("Turning on RGB LED Magenta\n");
        leds_on(RGB_LED_MAGENTA);

        // Delay to simulate the LED being on
        etimer_set(&timer, CLOCK_SECOND * 1);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        leds_off(RGB_LED_MAGENTA);
    }

    PROCESS_END();
}