
#include "contiki.h"
#include "dev/leds.h"
#include "dev/dht11.h"
#include "dev/nrf52840/nrf52840-gpio.h"
#include <stdio.h>

#define DHT11_PIN 2  // GPIO pin connected to DHT11
#define RGB_LED_PIN 3  // GPIO pin connected to RGB LED

PROCESS(dht11_rgb_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

static void init_dht11(void) {
    dht11_init(DHT11_PIN);
}

static void init_rgb_led(void) {
    nrf52840_gpio_set_output(RGB_LED_PIN);
    nrf52840_gpio_clear(RGB_LED_PIN);
}

static void read_dht11(void) {
    int temperature, humidity;
    if (dht11_read(&temperature, &humidity) == DHT11_OK) {
        printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
    } else {
        printf("Failed to read DHT11\n");
    }
}

static void turn_on_rgb_led(void) {
    nrf52840_gpio_set(RGB_LED_PIN);
}

PROCESS_THREAD(dht11_rgb_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    init_dht11();
    init_rgb_led();

    etimer_set(&timer, CLOCK_SECOND * 5);  // Set timer for 5 seconds

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        read_dht11();
        turn_on_rgb_led();

        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void) {
    process_init();
    process_start(&dht11_rgb_process, NULL);
    autostart_start(autostart_processes);

    while (1) {
        process_run();
    }

    return 0;
}
