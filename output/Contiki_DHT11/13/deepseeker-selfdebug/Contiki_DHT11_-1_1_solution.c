
#include "contiki.h"
#include "dev/gpio.h"
#include "dev/dht11.h"
#include <stdio.h>

#define DHT11_PIN 2

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

static void dht11_init(void) {
    gpio_set_input(DHT11_PIN);
}

static void dht11_read(void) {
    int temperature, humidity;
    if (dht11_read_data(DHT11_PIN, &temperature, &humidity) == 0) {
        printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
    } else {
        printf("Failed to read DHT11 sensor\n");
    }
}

PROCESS_THREAD(dht11_read_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    dht11_init();

    while (1) {
        etimer_set(&timer, CLOCK_SECOND * 5);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        dht11_read();
    }

    PROCESS_END();
}

int main(void) {
    clock_init();
    process_init();
    process_start(&etimer_process, NULL);
    process_start(&dht11_read_process, NULL);
    autostart_start(autostart_processes);

    while (1) {
        process_run();
    }

    return 0;
}
