#include "sys/timer.h"  // For timers
#include "sensors.h"  // Interfaces with the sensor functionalities
#include "dht11-sensor.h"  // DHT11 sensor driver
#include <stdio.h>  // Standard input-output

// Define RGB LED color (assuming pins are defined)
#define RGB_LED_RED 0x01
#define RGB_LED_GREEN 0x02
#define RGB_LED_BLUE 0x04
#define RGB_LED_MAGENTA (RGB_LED_RED | RGB_LED_BLUE)

void turn_on_led(uint8_t color) {
    // Code to interact with hardware LED to set its color
    printf("LED Color: %d\n", color);  // Here for demonstration
}

int main(void) {
    struct etimer timer;
    uint8_t temp_integer, temp_decimal, hum_integer, hum_decimal;
    uint8_t status;
    
    // Initialize sensor
    SENSORS_ACTIVATE(dht11_sensor);

    while (1) {
        // Set timer for 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);

        // Read sensor
        status = dht11_sensor.value(SENSOR_READ);
        if (status == DHT11_STATUS_OKAY) {
            temp_integer = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
            temp_decimal = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL);
            hum_integer = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
            hum_decimal = dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL);

            // Turn on LED (here just as an example, turn on magenta)
            turn_on_led(RGB_LED_MAGENTA);
        }

        // Wait for timer expiry
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    }

    // Deactivate sensor
    SENSORS_DEACTIVATE(dht11_sensor);

    return 0;
}