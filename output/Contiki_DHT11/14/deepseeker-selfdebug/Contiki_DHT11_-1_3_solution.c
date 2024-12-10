
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "dev/dht11.h"
#include <stdio.h>

#define DHT11_PIN 2

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

static struct etimer et;

void gpio_init(void) {
    gpio_hal_arch_pin_set_output(DHT11_PIN);
}

PROCESS_THREAD(dht11_read_process, ev, data) {
    static int temperature, humidity;

    PROCESS_BEGIN();

    gpio_init();

    while(1) {
        etimer_set(&et, CLOCK_SECOND * 5);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        if(dht11_read(DHT11_PIN, &temperature, &humidity) == 0) {
            printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
        } else {
            printf("Failed to read DHT11 sensor\n");
        }
    }

    PROCESS_END();
}

int main(void) {
    contiki_init();
    return 0;
}
