
#include "contiki.h"
#include "dev/dht11.h"  // Include the DHT11 sensor header file
#include "dev/leds.h"   // Include the LED control header file
#include <stdio.h>

// Define the RGB LED colors
#define RGB_LED_RED    (LEDS_RED)
#define RGB_LED_GREEN  (LEDS_GREEN)
#define RGB_LED_BLUE   (LEDS_BLUE)

// Define the RGB LED color combinations
#define RGB_LED_CYAN    (RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_MAGENTA (RGB_LED_RED | RGB_LED_BLUE)
#define RGB_LED_YELLOW  (RGB_LED_RED | RGB_LED_GREEN)
#define RGB_LED_WHITE   (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)

// Define the DHT11 sensor
extern const struct sensors_sensor dht11_sensor;

// Function to read DHT11 sensor data
void read_dht11(void) {
    int16_t temperature, humidity;
    if (dht11_sensor.value(&dht11_sensor, DHT11_VALUE_HUMIDITY_INTEGER, &humidity) == 0) {
        printf("Humidity: %d\n", humidity);
    }
    if (dht11_sensor.value(&dht11_sensor, DHT11_VALUE_TEMPERATURE_DECIMAL, &temperature) == 0) {
        printf("Temperature: %d\n", temperature);
    }
}

// Function to control the RGB LED
void control_rgb_led(void) {
    leds_set(RGB_LED_WHITE);  // Turn on the RGB LED in white color
}

// Process to handle the periodic reading of DHT11 and controlling the RGB LED
PROCESS(dht11_rgb_led_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_led_process);

PROCESS_THREAD(dht11_rgb_led_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 5);  // Set timer for 5 seconds

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        read_dht11();  // Read DHT11 sensor data
        control_rgb_led();  // Control the RGB LED

        etimer_reset(&timer);  // Reset the timer
    }

    PROCESS_END();
}

int main(void) {
    // Initialize the Contiki system
    contiki_init();

    // Start the process
    process_start(&dht11_rgb_led_process, NULL);

    return 0;
}
