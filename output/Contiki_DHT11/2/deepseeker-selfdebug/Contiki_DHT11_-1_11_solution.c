
#include "contiki.h"
#include "dht11-sensor.h"
#include "dev/gpio.h"
#include <stdio.h>

#define DHT11_GPIO_PIN 2

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

static void configure_dht11(void) {
    gpio_set_input(DHT11_GPIO_PIN);
}

static int read_dht11_value(int type) {
    return dht11_sensor.value(type);
}

PROCESS_THREAD(dht11_read_process, ev, data) {
    static struct etimer timer;
    static int humidity_integer, temperature_integer;

    PROCESS_BEGIN();

    configure_dht11();

    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        humidity_integer = read_dht11_value(DHT11_VALUE_HUMIDITY_INTEGER);
        temperature_integer = read_dht11_value(DHT11_VALUE_TEMPERATURE_INTEGER);

        printf("Humidity: %d %%\n", humidity_integer);
        printf("Temperature: %d C\n", temperature_integer);

        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void) {
    contiki_init();
    return 0;
}
