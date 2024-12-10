
#include "contiki.h"
#include "dev/dht11-sensor.h"  // Ensure this header file is available
#include "dev/leds.h"
#include <stdio.h>

PROCESS(dht11_led_process, "DHT11 and LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

PROCESS_THREAD(dht11_led_process, ev, data)
{
    static struct etimer timer;

    PROCESS_BEGIN();

    // Configure the DHT11 sensor
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0);  // Replace with actual GPIO port
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 1);   // Replace with actual GPIO pin
    dht11_sensor.configure(SENSORS_HW_INIT, 0);

    // Set the timer for 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor
        dht11_sensor.configure(SENSORS_ACTIVE, 1);
        int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
        int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

        // Print the readings
        printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);

        // Turn on the RGB LED
        leds_set(LEDS_RED | LEDS_GREEN | LEDS_BLUE);  // Assuming RGB_LED_WHITE is defined as LEDS_RED | LEDS_GREEN | LEDS_BLUE

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}
