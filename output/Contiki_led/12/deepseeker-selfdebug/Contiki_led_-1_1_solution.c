
#include "contiki.h"
#include "dev/dht11.h"
#include "dev/leds.h"
#include <stdio.h>

// Define the DHT11 sensor
extern const struct sensors_sensor dht11_sensor;

// Define the RGB LED colors
#define LED_RED     LED1_TOGGLE
#define LED_GREEN   LED2_TOGGLE
#define LED_BLUE    LED3_TOGGLE

// Process to read DHT11 and control RGB LED
PROCESS(dht11_rgb_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

// Function to read DHT11 sensor
static void read_dht11(void) {
    int temp, hum;
    if (dht11_sensor.value(&dht11_sensor, 0, &temp, &hum) == 0) {
        printf("Temperature: %d C, Humidity: %d %%\n", temp, hum);
    } else {
        printf("Failed to read DHT11 sensor\n");
    }
}

// Function to control RGB LED
static void control_rgb_led(void) {
    // Example: Turn on the red LED
    leds_toggle(LED_RED);
}

// Process definition
PROCESS_THREAD(dht11_rgb_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    // Initialize the DHT11 sensor
    dht11_sensor.configure(&dht11_sensor, SENSORS_ACTIVE, 1);

    // Set the timer to 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read DHT11 sensor
        read_dht11();

        // Control RGB LED
        control_rgb_led();

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
