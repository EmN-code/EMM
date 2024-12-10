
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "sys/etimer.h"
#include <stdio.h>

#define DHT11_PIN GPIO_PIN(2)
#define DHT11_VALUE_HUMIDITY_INTEGER (0)
#define DHT11_VALUE_HUMIDITY_DECIMAL (1)
#define DHT11_VALUE_TEMPERATURE_INTEGER (2)
#define DHT11_VALUE_TEMPERATURE_DECIMAL (3)
#define DHT11_STATUS_OKAY (0)
#define DHT11_STATUS_CHECKSUM_FAILED (2)

static uint8_t dht11_data[5];

static void dht11_init(void) {
    gpio_hal_set_output(DHT11_PIN);
    gpio_hal_set(DHT11_PIN);
    clock_delay(1000);
    gpio_hal_clear(DHT11_PIN);
    clock_delay(20000);
    gpio_hal_set(DHT11_PIN);
    clock_delay(40);
    gpio_hal_set_input(DHT11_PIN);
}

static uint8_t dht11_read(void) {
    uint8_t i, j;
    for(j = 0; j < 5; j++) {
        dht11_data[j] = 0;
        for(i = 0; i < 8; i++) {
            while(!gpio_hal_read(DHT11_PIN));
            clock_delay(40);
            if(gpio_hal_read(DHT11_PIN)) {
                dht11_data[j] |= (1 << (7 - i));
            }
            while(gpio_hal_read(DHT11_PIN));
        }
    }

    if((dht11_data[0] + dht11_data[1] + dht11_data[2] + dht11_data[3]) == dht11_data[4]) {
        return DHT11_STATUS_OKAY;
    } else {
        return DHT11_STATUS_CHECKSUM_FAILED;
    }
}

PROCESS(dht11_process, "DHT11 Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data) {
    static struct etimer timer;
    PROCESS_BEGIN();

    dht11_init();

    while(1) {
        etimer_set(&timer, CLOCK_SECOND * 5);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        if(dht11_read() == DHT11_STATUS_OKAY) {
            printf("Humidity: %d.%d %% Temperature: %d.%d C\n",
                   dht11_data[DHT11_VALUE_HUMIDITY_INTEGER],
                   dht11_data[DHT11_VALUE_HUMIDITY_DECIMAL],
                   dht11_data[DHT11_VALUE_TEMPERATURE_INTEGER],
                   dht11_data[DHT11_VALUE_TEMPERATURE_DECIMAL]);
        } else {
            printf("Checksum failed\n");
        }
    }

    PROCESS_END();
}
