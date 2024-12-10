
#include "contiki.h"
#include "dev/leds.h"
#include "dev/dht11.h"
#include "dev/nrf52840/nrf52840-gpio.h"
#include <stdio.h>

#define DHT11_PIN 2  // Assuming DHT11 is connected to GPIO pin 2
#define RGB_LED_PIN 3  // Assuming RGB LED is connected to GPIO pin 3

PROCESS(dht11_rgb_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

static void read_dht11(void) {
    int temperature, humidity;
    if (dht11_read(DHT11_PIN, &temperature, &humidity) == DHT11_SUCCESS) {
        printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
    } else {
        printf("Failed to read DHT11 sensor\n");
    }
}

static void turn_on_rgb_led(void) {
    nrf52840_gpio_set(RGB_LED_PIN);
}

PROCESS_THREAD(dht11_rgb_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    // Initialize GPIO pins
    nrf52840_gpio_init();
    nrf52840_gpio_dir_output(RGB_LED_PIN);
    nrf52840_gpio_dir_input(DHT11_PIN);

    // Set timer to 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read DHT11 sensor
        read_dht11();

        // Turn on RGB LED
        turn_on_rgb_led();

        // Reset timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void) {
    // Initialize Contiki
    contiki_init();

    // Start the process
    process_start(&dht11_rgb_process, NULL);

    // Enter the Contiki main loop
    while (1) {
        process_run();
    }

    return 0;
}
