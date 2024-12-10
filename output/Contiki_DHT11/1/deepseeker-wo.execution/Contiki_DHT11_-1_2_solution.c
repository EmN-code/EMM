
#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki-conf.h"
#include "contiki-default-conf.h"
#include "coap-observe-client.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define DHT11_GPIO_PIN 2
#define READ_INTERVAL (5 * CLOCK_SECOND)

static struct etimer read_timer;

static void dht11_init(void) {
    // Initialize GPIO for DHT11
    nrf_gpio_cfg_output(DHT11_GPIO_PIN);
    nrf_gpio_pin_set(DHT11_GPIO_PIN);
}

static bool dht11_read(uint8_t *humidity, uint8_t *temperature) {
    // Simulate reading from DHT11
    // In a real implementation, you would read the actual data from the sensor
    *humidity = 50;
    *temperature = 25;
    return true;
}

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data) {
    static uint8_t humidity, temperature;

    PROCESS_BEGIN();

    dht11_init();

    etimer_set(&read_timer, READ_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&read_timer));

        if(dht11_read(&humidity, &temperature)) {
            printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);
        } else {
            printf("Failed to read DHT11 sensor\n");
        }

        etimer_reset(&read_timer);
    }

    PROCESS_END();
}

int main(void) {
    process_init();
    process_start(&etimer_process, NULL);
    process_start(&dht11_read_process, NULL);
    autostart_start(autostart_processes);

    while(1) {
        process_run();
    }

    return 0;
}
