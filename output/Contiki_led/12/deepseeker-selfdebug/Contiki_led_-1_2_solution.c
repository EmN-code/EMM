
#include "contiki.h"
#include "dev/leds.h"
#include "sys/etimer.h"
#include <stdio.h>

// Define the RGB LED colors
#define RGB_LED_RED     (1 << 0)
#define RGB_LED_GREEN   (1 << 1)
#define RGB_LED_BLUE    (1 << 2)
#define RGB_LED_WHITE   (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_CYAN    (RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_MAGENTA (RGB_LED_RED | RGB_LED_BLUE)

// Define the DHT11 sensor (assuming it's available in the platform)
extern const struct sensors_sensor dht11_sensor;

// Timer for reading the DHT11 sensor every 5 seconds
static struct etimer dht11_timer;

// Function to read the DHT11 sensor
void read_dht11(void) {
    int temp, hum;
    if (dht11_sensor.value(&dht11_sensor, 0, &temp, &hum) == 0) {
        printf("DHT11: Temperature = %d, Humidity = %d\n", temp, hum);
    } else {
        printf("Failed to read DHT11 sensor\n");
    }
}

// Function to turn on the RGB LED
void turn_on_rgb_led(void) {
    leds_set(RGB_LED_WHITE);
}

PROCESS(dht11_rgb_led_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_led_process);

PROCESS_THREAD(dht11_rgb_led_process, ev, data) {
    PROCESS_BEGIN();

    // Initialize the DHT11 sensor
    dht11_sensor.configure(&dht11_sensor, SENSORS_ACTIVE, 1);

    // Set the timer for 5 seconds
    etimer_set(&dht11_timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&dht11_timer));

        // Read the DHT11 sensor
        read_dht11();

        // Turn on the RGB LED
        turn_on_rgb_led();

        // Reset the timer for the next reading
        etimer_reset(&dht11_timer);
    }

    PROCESS_END();
}
