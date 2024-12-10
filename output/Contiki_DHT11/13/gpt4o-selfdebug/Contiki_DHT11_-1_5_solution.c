#include "sys/etimer.h"
#include "sys/ctimer.h"
#include "dev/dht11-sensor.h"
#include "sys/clock.h"
#include "contiki.h"
#include "dev/gpio.h"

// Define the GPIO port and pin for the DHT11 sensor
#define DHT11_GPIO_PORT DHT11_CONFIGURE_GPIO_PORT
#define DHT11_GPIO_PIN DHT11_CONFIGURE_GPIO_PIN

// Process to read the DHT11 sensor
PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

static struct etimer et;

PROCESS_THREAD(dht11_process, ev, data) {
    PROCESS_BEGIN();

    // Initialize the GPIO pin for the DHT11 sensor
    gpio_set_pin_output(DHT11_GPIO_PORT, DHT11_GPIO_PIN);

    while(1) {
        // Set the timer for 5 seconds
        etimer_set(&et, CLOCK_SECOND * 5);

        // Wait for the timer event
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Activate the sensor to get humidity and temperature readings
        SENSORS_ACTIVATE(dht11_sensor);
        int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
        SENSORS_DEACTIVATE(dht11_sensor);

        // Output the readings
        printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);
    }

    PROCESS_END();
}

int main() {
    // Initialize Contiki processes
    process_init();
    process_start(&etimer_process, NULL);
    process_start(&dht11_process, NULL);

    // Start the main loop
    while(1) {
        // Execute processes
        process_run();
    }
    return 0;
}