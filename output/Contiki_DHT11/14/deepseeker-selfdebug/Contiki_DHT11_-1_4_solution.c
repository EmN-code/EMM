
#include <stdio.h>
#include <dev/gpio-hal.h>
#include <contiki-default-conf.h>
#include <contiki-lib.h>
#include <contiki-net.h>
#include <contiki.h>

#define DHT11_GPIO_PIN 2
#define DHT11_CONFIGURE_GPIO_PORT 0

#define DHT11_STATUS_OKAY 0
#define DHT11_STATUS_CHECKSUM_FAILED 2

#define DHT11_VALUE_HUMIDITY_INTEGER 0
#define DHT11_VALUE_HUMIDITY_DECIMAL 1
#define DHT11_VALUE_TEMPERATURE_INTEGER 2

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

static uint8_t dht11_data[5];

static void dht11_init(void) {
    gpio_hal_set_output(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PIN);
    gpio_hal_set(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PIN);
    clock_delay(1000);
    gpio_hal_clear(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PIN);
    clock_delay(20000);
    gpio_hal_set(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PIN);
    clock_delay(40);
    gpio_hal_set_input(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PIN);
}

static uint8_t dht11_read(void) {
    uint8_t i, j = 0;
    for (i = 0; i < 5; i++) {
        dht11_data[i] = 0;
    }
    for (i = 0; i < 40; i++) {
        while (gpio_hal_read(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PIN) == 0);
        clock_delay(40);
        if (gpio_hal_read(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PIN) == 1) {
            dht11_data[j / 8] |= (1 << (7 - (j % 8)));
        }
        j++;
        while (gpio_hal_read(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PIN) == 1);
    }
    if (dht11_data[4] == ((dht11_data[0] + dht11_data[1] + dht11_data[2] + dht11_data[3]) & 0xFF)) {
        return DHT11_STATUS_OKAY;
    } else {
        return DHT11_STATUS_CHECKSUM_FAILED;
    }
}

PROCESS_THREAD(dht11_read_process, ev, data) {
    static struct etimer timer;
    PROCESS_BEGIN();

    dht11_init();

    while (1) {
        etimer_set(&timer, CLOCK_SECOND * 5);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        if (dht11_read() == DHT11_STATUS_OKAY) {
            printf("Humidity: %d.%d %% Temperature: %d C\n",
                   dht11_data[DHT11_VALUE_HUMIDITY_INTEGER],
                   dht11_data[DHT11_VALUE_HUMIDITY_DECIMAL],
                   dht11_data[DHT11_VALUE_TEMPERATURE_INTEGER]);
        } else {
            printf("Checksum error!\n");
        }
    }

    PROCESS_END();
}

int main(void) {
    process_start(&dht11_read_process, NULL);
    return 0;
}
