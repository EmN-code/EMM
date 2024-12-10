// This program will read data from a DHT11 sensor every 5 seconds on an NRF52840
// and turn on an RGB LED. Ensure that your NRF52840 board is properly configured
// and connected with a DHT11 sensor and an RGB LED.

#include "pwm.h"
#include "dht11-sensor.h"
#include "contiki.h"

PROCESS(dht11_read_process, "DHT11 read and LED control process");
AUTOSTART_PROCESSES(&dht11_read_process);

static struct etimer timer;

PROCESS_THREAD(dht11_read_process, ev, data) {
    PROCESS_BEGIN();
    
    // Initialize the DHT11 sensor
    SENSORS_ACTIVATE(dht11_sensor);

    while (1) {
        // Set timer for 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);

        // Wait for the timer event
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read temperature and humidity from DHT11 sensor
        int temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
        int humidity = dht11_sensor.value(DHT11_SENSOR_HUMIDITY);

        if (temperature != -1 && humidity != -1) {
            printf("Temperature: %d.%d C, Humidity: %d.%d\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);

            // Turn on RGB LED to indicate data reading
            // (Assume PWM correctly initialized for RGB control)
            pwm_enable(PIN_RGB_RED);
            pwm_enable(PIN_RGB_GREEN);
            pwm_enable(PIN_RGB_BLUE);

            // Example: Set LED to white color
            pwm_set_duty_cycle(PIN_RGB_RED, 255);
            pwm_set_duty_cycle(PIN_RGB_GREEN, 255);
            pwm_set_duty_cycle(PIN_RGB_BLUE, 255);
        }
        else {
            printf("Failed to read from DHT11 sensor\n");
        }
    }
    
    PROCESS_END();
}
