
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "sys/etimer.h"
#include "sys/clock.h"

// Define the RGB LED colors
#define RGB_LED_RED   0x01
#define RGB_LED_GREEN 0x02
#define RGB_LED_BLUE  0x04
#define RGB_LED_WHITE  (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_MAGENTA (RGB_LED_RED | RGB_LED_BLUE)
#define RGB_LED_YELLOW  (RGB_LED_RED | RGB_LED_GREEN)

// Define the GPIO pins for the RGB LED
#define RGB_LED_PIN_RED   22
#define RGB_LED_PIN_GREEN 23
#define RGB_LED_PIN_BLUE  24

// Function to set the RGB LED color
void rgb_led_set(uint8_t colour) {
    gpio_hal_set(RGB_LED_PIN_RED, (colour & RGB_LED_RED) ? 1 : 0);
    gpio_hal_set(RGB_LED_PIN_GREEN, (colour & RGB_LED_GREEN) ? 1 : 0);
    gpio_hal_set(RGB_LED_PIN_BLUE, (colour & RGB_LED_BLUE) ? 1 : 0);
}

// Simulated DHT11 reading function
int dht11_read_temperature() {
    // Simulate reading temperature from DHT11
    return 25; // Return a fixed temperature value
}

PROCESS(dht11_rgb_led_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_led_process);

PROCESS_THREAD(dht11_rgb_led_process, ev, data) {
    static struct etimer et;
    static int temperature;

    PROCESS_BEGIN();

    // Initialize GPIO pins for the RGB LED
    gpio_hal_init();
    gpio_hal_dir(RGB_LED_PIN_RED, GPIO_HAL_PIN_OUTPUT);
    gpio_hal_dir(RGB_LED_PIN_GREEN, GPIO_HAL_PIN_OUTPUT);
    gpio_hal_dir(RGB_LED_PIN_BLUE, GPIO_HAL_PIN_OUTPUT);

    while (1) {
        // Set the timer to 5 seconds
        etimer_set(&et, CLOCK_SECOND * 5);

        // Wait for the timer to expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Read the temperature from the DHT11 sensor
        temperature = dht11_read_temperature();

        // Turn on the RGB LED based on the temperature
        if (temperature < 20) {
            rgb_led_set(RGB_LED_BLUE);
        } else if (temperature >= 20 && temperature < 30) {
            rgb_led_set(RGB_LED_GREEN);
        } else {
            rgb_led_set(RGB_LED_RED);
        }
    }

    PROCESS_END();
}

int main() {
    // Start the Contiki system
    contiki_init();
    return 0;
}
