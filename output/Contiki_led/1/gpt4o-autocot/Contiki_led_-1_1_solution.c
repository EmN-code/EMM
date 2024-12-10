#include <stdio.h>
#include "sensors_sensor.h"
#include "etimer.h"

// Placeholder functions for the RGB LED (assuming a function exists to control RGB LED)
void rgb_led_on() {
    printf("RGB LED is now ON.\n");
}

void rgb_led_off() {
    printf("RGB LED is now OFF.\n");
}

int main() {
    // Initialize DHT11 sensor and RGB LED
    const struct sensors_sensor *sensor = &dht11_sensor;
    sensor->configure(SENSORS_ACTIVE, 1);

    // Set up and start a timer
    static struct etimer my_timer;
    etimer_set(&my_timer, 5 * CLOCK_SECOND);

    while(1) {
        // Wait for the timer event
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&my_timer));

        // Read the DHT11 sensor
        int temperature = sensor->value(0);
        int humidity = sensor->value(1);

        printf("DHT11 Sensor Readings: Temp: %d, Humidity: %d\n", temperature, humidity);

        // Turn on the RGB LED
        rgb_led_on();

        // Reset the timer for the next interval
        etimer_reset(&my_timer);
    }

    return 0;
}