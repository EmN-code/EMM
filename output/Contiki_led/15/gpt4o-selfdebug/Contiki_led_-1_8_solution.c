#include "sensors.h"
#include "dht11_sensor.h"
#include "contiki.h"

PROCESS(dht11_sensor_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_sensor_process);

// Function to initialize RGB LED
void init_rgb_led(void) {
    // Initialize the RGB LED here
    // This function depends on your specific RGB LED setup
}

// Function to turn on RGB LED
void turn_on_rgb_led(void) {
    // Code to turn on the RGB LED
    // This function depends on your specific RGB LED setup
}

PROCESS_THREAD(dht11_sensor_process, ev, data)
{
    static struct etimer timer;
    static int temperature, humidity;
    
    PROCESS_BEGIN();

    // Initialize the DHT11 sensor
    SENSORS_ACTIVATE(dht11_sensor);
    init_rgb_led();

    while(1) {
        etimer_set(&timer, CLOCK_SECOND * 5);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read humidity and temperature values
        humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
        temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

        if (humidity != -1 && temperature != -1) {
            // Turn on the RGB LED
            turn_on_rgb_led();
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }
    }

    PROCESS_END();
}