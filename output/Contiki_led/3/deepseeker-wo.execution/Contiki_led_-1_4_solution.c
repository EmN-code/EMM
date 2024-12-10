
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "dev/leds.h"
#include "sys/rtimer.h"
#include "random.h"

#define DHT11_GPIO_PIN 2  // Example GPIO pin for DHT11
#define RGB_LED_PIN 3     // Example GPIO pin for RGB LED

static volatile uint8_t dht11_data[5];

void read_dht11(void) {
    gpio_hal_set_input(DHT11_GPIO_PIN);
    clock_delay_usec(18000);  // Start signal
    gpio_hal_set_output(DHT11_GPIO_PIN);
    clock_delay_usec(40);
    gpio_hal_set_input(DHT11_GPIO_PIN);

    for (int i = 0; i < 5; i++) {
        dht11_data[i] = 0;
        for (int j = 0; j < 8; j++) {
            while (!gpio_hal_read(DHT11_GPIO_PIN));  // Wait for high
            clock_delay_usec(40);
            if (gpio_hal_read(DHT11_GPIO_PIN)) {
                dht11_data[i] |= (1 << (7 - j));
            }
            while (gpio_hal_read(DHT11_GPIO_PIN));  // Wait for low
        }
    }

    // Check checksum
    uint8_t checksum = dht11_data[0] + dht11_data[1] + dht11_data[2] + dht11_data[3];
    if (checksum != dht11_data[4]) {
        // Handle checksum error
        return;
    }

    // Extract data
    uint8_t humidity_integer = dht11_data[0];
    uint8_t temperature_integer = dht11_data[2];

    // Turn on RGB LED
    gpio_hal_set(RGB_LED_PIN);
}

PROCESS(dht11_process, "DHT11 Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data) {
    static struct etimer et;

    PROCESS_BEGIN();

    etimer_set(&et, CLOCK_SECOND * 5);  // Set timer to 5 seconds

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        read_dht11();
        etimer_reset(&et);
    }

    PROCESS_END();
}
