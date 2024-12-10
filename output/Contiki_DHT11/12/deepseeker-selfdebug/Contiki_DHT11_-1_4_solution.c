
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "dev/dht11-sensor.h"
#include <stdio.h>

#define DHT11_GPIO_PIN 2

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

static void dht11_init(void) {
    gpio_hal_init();
    gpio_hal_set_input(DHT11_GPIO_PIN);
}

static int dht11_read(void) {
    int status = dht11_sensor.status(&dht11_sensor);
    if (status == DHT11_STATUS_OKAY) {
        int humidity = dht11_sensor.value(&dht11_sensor, DHT11_VALUE_HUMIDITY_INTEGER);
        int temperature = dht11_sensor.value(&dht11_sensor, DHT11_VALUE_TEMPERATURE_INTEGER);
        printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);
    } else {
        printf("DHT11 read failed with status: %d\n", status);
    }
    return status;
}

PROCESS_THREAD(dht11_read_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    dht11_init();

    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        dht11_read();

        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void) {
    contiki_init();
    return 0;
}
