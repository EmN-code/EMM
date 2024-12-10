#include "sys/process.h"
#include "sys/etimer.h"
#include "contiki.h"
#include "contiki-default-conf.h"
#include <stdio.h>

#define DHT11_PIN 0 // Define the pin connected to the DHT11
#define RGB_LED_PIN 1 // Define the pin connected to the RGB LED

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
    static struct etimer timer;
    static int dht11_pin = DHT11_PIN;
    static int rgb_led_pin = RGB_LED_PIN;

    PROCESS_BEGIN();

    // Set a timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        // Wait for the timer event
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor
        int temperature = read_dht11(dht11_pin);

        // Turn on RGB LED based on temperature reading
        if (temperature > 0) {
            turn_on_rgb_led(rgb_led_pin);
        }

        // Reset the timer for the next interval
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int read_dht11(int pin) {
    // Simulate sensor data reading
    return 25; // Example temperature value
}

void turn_on_rgb_led(int pin) {
    // Simulate turning on the LED
    printf("RGB LED on pin %d is turned on.\n", pin);
}