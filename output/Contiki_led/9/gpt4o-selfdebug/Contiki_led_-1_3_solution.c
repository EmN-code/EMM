#include "contiki.h"
#include "dev/leds.h"
#include "dev/sht11-sensor.h"
#include "dev/sensor.h"
#include "core/net/ipv6/uip.h"
#include <stdio.h>

#define RGB_LED_RED LEDS_RED
#define RGB_LED_GREEN LEDS_GREEN
#define RGB_LED_BLUE LEDS_BLUE

PROCESS(dht11_led_process, "DHT11 LED process");
AUTOSTART_PROCESSES(&dht11_led_process);

static void rgb_led_on(void) {
    leds_on(RGB_LED_RED);
    leds_on(RGB_LED_GREEN);
    leds_on(RGB_LED_BLUE);
}

PROCESS_THREAD(dht11_led_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Assuming the sensor is configured and accessible
        SENSORS_ACTIVATE(sht11_sensor);

        int temperature_value = sht11_sensor.value(SHT11_READ_TEMP);
        int humidity_value = sht11_sensor.value(SHT11_READ_RHUM);

        printf("Temperature: %d, Humidity: %d\n", temperature_value, humidity_value);

        // Turn on the LED
        rgb_led_on();

        etimer_reset(&timer);
    }

    PROCESS_END();
}
