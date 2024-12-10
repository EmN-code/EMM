
#include "contiki.h"
#include "dev/leds.h"
#include "dev/dht11.h"
#include "dev/nrf52840/nrf52840-gpio.h"
#include <stdio.h>

#define DHT11_PIN 2  // Assuming DHT11 is connected to GPIO pin 2
#define RGB_LED_PIN 3  // Assuming RGB LED is connected to GPIO pin 3

PROCESS(dht11_rgb_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

static struct etimer timer;

void turn_on_rgb_led() {
    nrf52840_gpio_set(RGB_LED_PIN);
}

PROCESS_THREAD(dht11_rgb_process, ev, data) {
    PROCESS_BEGIN();

    // Initialize GPIO pins
    nrf52840_gpio_init();
    nrf52840_gpio_dir(DHT11_PIN, NRF_GPIO_PIN_DIR_INPUT);
    nrf52840_gpio_dir(RGB_LED_PIN, NRF_GPIO_PIN_DIR_OUTPUT);

    // Initialize DHT11 sensor
    dht11_init(DHT11_PIN);

    while(1) {
        etimer_set(&timer, CLOCK_SECOND * 5);  // Set timer for 5 seconds

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        int16_t temperature, humidity;
        if(dht11_read(&temperature, &humidity) == DHT11_SUCCESS) {
            printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
            turn_on_rgb_led();
        } else {
            printf("Failed to read DHT11 sensor\n");
        }

        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main() {
    process_start(&dht11_rgb_process, NULL);
    return 0;
}
