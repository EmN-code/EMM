
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "sys/etimer.h"
#include "dev/leds.h"

// Define the GPIO pins for the RGB LED
#define RGB_LED_RED    GPIO_PIN(0, 13)
#define RGB_LED_GREEN  GPIO_PIN(0, 14)
#define RGB_LED_BLUE   GPIO_PIN(0, 15)

// Define the GPIO pin for the DHT11 sensor
#define DHT11_PIN      GPIO_PIN(0, 16)

// Define the RGB LED colors
#define RGB_LED_CYAN    (RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_MAGENTA (RGB_LED_RED | RGB_LED_BLUE)

// Define the debounce duration for the button
#define BUTTON_HAL_DEBOUNCE_DURATION (CLOCK_SECOND >> 6)

// Function to read DHT11 sensor
static void read_dht11(void) {
    // Simulate reading the DHT11 sensor
    // In a real implementation, you would read the sensor data here
    int temperature = 25; // Example temperature value
    int humidity = 60;    // Example humidity value

    // Turn on the RGB LED based on the sensor data
    if (temperature > 25) {
        leds_set(RGB_LED_RED);
    } else if (humidity > 50) {
        leds_set(RGB_LED_GREEN);
    } else {
        leds_set(RGB_LED_BLUE);
    }
}

PROCESS(dht11_process, "DHT11 Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
    static struct etimer timer;

    PROCESS_BEGIN();

    // Initialize the GPIO pins for the RGB LED
    gpio_hal_arch_pin_output(RGB_LED_RED);
    gpio_hal_arch_pin_output(RGB_LED_GREEN);
    gpio_hal_arch_pin_output(RGB_LED_BLUE);

    // Initialize the GPIO pin for the DHT11 sensor
    gpio_hal_arch_pin_input(DHT11_PIN);

    // Set the timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor and turn on the RGB LED
        read_dht11();

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}
