
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "dev/leds.h"
#include "sys/etimer.h"
#include <stdio.h>

// Define the GPIO pin for the DHT11 sensor
#define DHT11_GPIO_PIN 1

// Define the RGB LED colors
#define RGB_LED_RED   0x01
#define RGB_LED_GREEN 0x02
#define RGB_LED_BLUE  0x04

// Define the RGB LED color for yellow
#define RGB_LED_YELLOW (RGB_LED_RED | RGB_LED_GREEN)

// Function to read DHT11 sensor data
static void read_dht11(void) {
    // Placeholder for DHT11 reading logic
    // This should be replaced with actual DHT11 reading code
    printf("DHT11 data read\n");
}

// Function to turn on the RGB LED
static void turn_on_rgb_led(void) {
    leds_set(RGB_LED_YELLOW);
}

PROCESS(dht11_rgb_led_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_led_process);

PROCESS_THREAD(dht11_rgb_led_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    // Configure the GPIO pin for the DHT11 sensor
    gpio_hal_pin_set_output(DHT11_GPIO_PIN);

    // Set the timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor
        read_dht11();

        // Turn on the RGB LED
        turn_on_rgb_led();

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}
