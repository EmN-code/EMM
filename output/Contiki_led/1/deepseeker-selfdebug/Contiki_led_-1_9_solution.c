
#include "contiki.h"
#include "dev/dht11-sensor.h"  // Ensure this header file is available
#include "dev/leds.h"          // For controlling the RGB LED
#include <stdio.h>

// Define the RGB LED colors
#define RGB_LED_RED   0x01
#define RGB_LED_GREEN 0x02
#define RGB_LED_BLUE  0x04

// Define the RGB LED colors combinations
#define RGB_LED_WHITE  (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_YELLOW (RGB_LED_RED | RGB_LED_GREEN)
#define RGB_LED_CYAN   (RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_MAGENTA (RGB_LED_RED | RGB_LED_BLUE)

// Define the DHT11 sensor value types
#define DHT11_VALUE_HUMIDITY_INTEGER       (0)
#define DHT11_VALUE_HUMIDITY_DECIMAL       (1)
#define DHT11_VALUE_TEMPERATURE_INTEGER    (2)
#define DHT11_VALUE_TEMPERATURE_DECIMAL    (3)

// Define the DHT11 sensor status
#define DHT11_STATUS_OKAY               (0)
#define DHT11_STATUS_TIMEOUT            (1)
#define DHT11_STATUS_CHECKSUM_FAILED    (2)

// Define the DHT11 sensor configuration
#define DHT11_CONFIGURE_GPIO_PORT   (0)
#define DHT11_CONFIGURE_GPIO_PIN    (1)

// Function to read DHT11 sensor data
static int read_dht11(int *humidity, int *temperature) {
    int status = dht11_sensor.status(&dht11_sensor);
    if (status != DHT11_STATUS_OKAY) {
        return status;
    }
    *humidity = dht11_sensor.value(&dht11_sensor, DHT11_VALUE_HUMIDITY_INTEGER);
    *temperature = dht11_sensor.value(&dht11_sensor, DHT11_VALUE_TEMPERATURE_INTEGER);
    return DHT11_STATUS_OKAY;
}

// Process to read DHT11 sensor and control RGB LED
PROCESS(dht11_rgb_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

PROCESS_THREAD(dht11_rgb_process, ev, data) {
    static struct etimer timer;
    static int humidity, temperature;
    static int status;

    PROCESS_BEGIN();

    // Initialize the DHT11 sensor
    dht11_sensor.configure(&dht11_sensor, DHT11_CONFIGURE_GPIO_PORT, 0);
    dht11_sensor.configure(&dht11_sensor, DHT11_CONFIGURE_GPIO_PIN, 1);

    // Set the timer to 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor
        status = read_dht11(&humidity, &temperature);
        if (status == DHT11_STATUS_OKAY) {
            printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);

            // Turn on the RGB LED based on temperature
            if (temperature < 20) {
                leds_set(RGB_LED_BLUE);
            } else if (temperature < 30) {
                leds_set(RGB_LED_GREEN);
            } else {
                leds_set(RGB_LED_RED);
            }
        } else {
            printf("Failed to read DHT11 sensor, status: %d\n", status);
        }

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void) {
    // Initialize the Contiki system
    contiki_init();

    // Start the process
    process_start(&dht11_rgb_process, NULL);

    // Enter the main loop
    while (1) {
        process_run();
    }

    return 0;
}
