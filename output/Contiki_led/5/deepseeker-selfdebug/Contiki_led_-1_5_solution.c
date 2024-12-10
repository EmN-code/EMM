
#include "contiki.h"
#include "dht11-sensor.h"
#include "dev/leds.h"
#include "sys/etimer.h"
#include <stdio.h>

#define INTERVAL_SECONDS 5

PROCESS(dht11_led_process, "DHT11 and LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

static struct etimer timer;

void read_dht11(void) {
    int humidity_int, humidity_dec, temperature_int, temperature_dec;
    int status = sensors_read(&dht11_sensor, DHT11_VALUE_HUMIDITY_INTEGER, &humidity_int);
    if (status == DHT11_STATUS_OKAY) {
        sensors_read(&dht11_sensor, DHT11_VALUE_HUMIDITY_DECIMAL, &humidity_dec);
        sensors_read(&dht11_sensor, DHT11_VALUE_TEMPERATURE_INTEGER, &temperature_int);
        sensors_read(&dht11_sensor, DHT11_VALUE_TEMPERATURE_DECIMAL, &temperature_dec);
        printf("Humidity: %d.%d %%, Temperature: %d.%d C\n", humidity_int, humidity_dec, temperature_int, temperature_dec);
        rgb_led_set(RGB_LED_WHITE); // Turn on the RGB LED
    } else {
        printf("Failed to read DHT11 sensor\n");
    }
}

PROCESS_THREAD(dht11_led_process, ev, data) {
    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * INTERVAL_SECONDS);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        read_dht11();
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void) {
    process_init();
    process_start(&dht11_led_process, NULL);
    process_run();
    return 0;
}
