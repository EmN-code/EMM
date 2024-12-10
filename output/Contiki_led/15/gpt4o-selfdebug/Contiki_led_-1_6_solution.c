#include "dht11_sensor.h"
#include "stdio.h"
#include "shell.h"
#include "gpio.h"
#include "clock.h"
#include "contiki.h"

PROCESS(dht11_reader_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&dht11_reader_process);

static struct etimer timer;

PROCESS_THREAD(dht11_reader_process, ev, data) {
    PROCESS_BEGIN();

    // Configure GPIO pins for the DHT11 sensor
    gpio_init(DHT11_CONFIGURE_GPIO_PORT);
    gpio_init(DHT11_CONFIGURE_GPIO_PIN);

    while (1) {
        // Wait for 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read DHT11 sensor data
        int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
        int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

        // Print the sensor data
        printf("Humidity: %d, Temperature: %d\n", humidity, temperature);

        // Turn on RGB LED (assuming GPIO_PIN for LED is defined)
        gpio_set(GPIO_PIN);

        // Wait 1 second and then turn off the LED
        etimer_set(&timer, CLOCK_SECOND);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        gpio_clear(GPIO_PIN);
    }

    PROCESS_END();
}