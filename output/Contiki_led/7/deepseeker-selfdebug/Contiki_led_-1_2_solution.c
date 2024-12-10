
#include "contiki.h"
#include "dev/leds.h"
#include "sys/etimer.h"
#include <stdio.h>

// Define the DHT11 sensor pin (assuming it's connected to a specific pin on the NRF52840)
#define DHT11_PIN 2

// Define the RGB LED colors
#define RGB_LED_RED    (1 << 0)
#define RGB_LED_GREEN  (1 << 1)
#define RGB_LED_BLUE   (1 << 2)

// Function to read DHT11 sensor data (dummy implementation as the actual driver is missing)
void read_dht11(int *temperature, int *humidity) {
    // Dummy values for demonstration
    *temperature = 25;
    *humidity = 60;
}

PROCESS(dht11_rgb_led_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_led_process);

PROCESS_THREAD(dht11_rgb_led_process, ev, data)
{
    static struct etimer timer;
    static int temperature, humidity;

    PROCESS_BEGIN();

    // Set the timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read DHT11 sensor data
        read_dht11(&temperature, &humidity);

        // Print the temperature and humidity values
        printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);

        // Turn on the RGB LED based on the temperature value
        if (temperature < 20) {
            leds_set(RGB_LED_BLUE);
        } else if (temperature >= 20 && temperature < 30) {
            leds_set(RGB_LED_GREEN);
        } else {
            leds_set(RGB_LED_RED);
        }

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}
