
#include "contiki.h"
#include "contiki-net.h"
#include "dev/gpio-hal.h"
#include "contiki-default-conf.h"
#include "dev/leds.h"
#include "dev/rgb-led/rgb-led.h"

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN  2

#define LED_RED   1
#define LED_GREEN 2
#define LED_BLUE  3

PROCESS(dht11_led_process, "DHT11 and LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

static uint8_t read_dht11(uint8_t *humidity, uint8_t *temperature) {
    // Simulate reading DHT11 data
    *humidity = 50;
    *temperature = 25;
    return 0; // Simulate successful read
}

PROCESS_THREAD(dht11_led_process, ev, data) {
    static struct etimer timer;
    static uint8_t humidity, temperature;

    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        if (read_dht11(&humidity, &temperature) == 0) {
            // Turn on the RGB LED
            rgb_led_set(LED_RED, 255);
            rgb_led_set(LED_GREEN, 0);
            rgb_led_set(LED_BLUE, 0);
        }

        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void) {
    // Initialize the RGB LED
    rgb_led_init();

    // Start the Contiki process
    process_start(&dht11_led_process, NULL);

    return 0;
}
