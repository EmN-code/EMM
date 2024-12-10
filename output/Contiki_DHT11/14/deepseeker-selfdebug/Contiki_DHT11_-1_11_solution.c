
#include "contiki.h"
#include "dht11-sensor.h"
#include <stdio.h>

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 2

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

static void configure_dht11(void) {
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);
}

static void read_dht11(void) {
    int status = dht11_sensor.status();
    if (status == DHT11_STATUS_OKAY) {
        int humidity_int = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
        int humidity_dec = dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL);
        int temperature_int = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        int temperature_dec = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL);

        printf("Humidity: %d.%d %%\n", humidity_int, humidity_dec);
        printf("Temperature: %d.%d C\n", temperature_int, temperature_dec);
    } else {
        printf("Error reading DHT11: %d\n", status);
    }
}

PROCESS_THREAD(dht11_read_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    configure_dht11();

    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        read_dht11();

        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void) {
    contiki_init();
    return 0;
}
