
#include "contiki.h"
#include "dev/gpio.h"
#include "dev/leds.h"
#include "dev/dht11.h"
#include <stdio.h>

#define DHT11_GPIO_PORT (0)
#define DHT11_GPIO_PIN  (1)

#define RGB_LED_RED     (LEDS_RED)
#define RGB_LED_GREEN   (LEDS_GREEN)
#define RGB_LED_BLUE    (LEDS_BLUE)

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

void rgb_led_set(uint8_t colour) {
    leds_off(LEDS_ALL);
    leds_on(colour);
}

void rgb_led_off(void) {
    leds_off(LEDS_ALL);
}

PROCESS_THREAD(dht11_read_process, ev, data) {
    static struct etimer timer;
    uint8_t dht11_values[4];
    int status;

    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        status = dht11_read(DHT11_GPIO_PORT, DHT11_GPIO_PIN, dht11_values);

        if(status == DHT11_STATUS_OK) {
            printf("Humidity: %d.%d %% Temperature: %d.%d C\n",
                   dht11_values[DHT11_VALUE_HUMIDITY_INTEGER],
                   dht11_values[DHT11_VALUE_HUMIDITY_DECIMAL],
                   dht11_values[DHT11_VALUE_TEMPERATURE_INTEGER],
                   dht11_values[DHT11_VALUE_TEMPERATURE_DECIMAL]);

            rgb_led_set(RGB_LED_GREEN);
        } else {
            printf("DHT11 read failed with status: %d\n", status);
            rgb_led_set(RGB_LED_RED);
        }

        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void) {
    dht11_init();
    process_start(&dht11_read_process, NULL);
    return 0;
}
