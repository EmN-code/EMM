#include "sensors.h"
#include "dht11-sensor.h"
#include <stdio.h>

#define RGB_LED_RED (1 << 0)
#define RGB_LED_GREEN (1 << 1)
#define RGB_LED_BLUE (1 << 2)
#define RGB_LED_MAGENTA (RGB_LED_RED | RGB_LED_BLUE)

void init_led(void) {
    // Implement LED initialization here
}

void set_led_color(int color) {
    // Implement setting LED color here
}
}

int main(void) {
    init_led();
    while (1) {
        // Reading from DHT11
        int humidity, temperature;
        if (dht11_sensor.value(DHT11_STATUS_OKAY) == DHT11_STATUS_OKAY) {
            humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
            temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
            printf("Humidity: %d%% Temperature: %d°C\n", humidity, temperature);
            
            // Turn on Magenta LED for 5 seconds
            set_led_color(RGB_LED_MAGENTA);
            clock_delay(5000);
        }
        else {
            printf("Failed to read from DHT11\n");
        }
        clock_delay(5000);
    }
    return 0;
}