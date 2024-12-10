#include "sensors.h"
#include "net/gpio.h"
#include "contiki-default-conf.h"

// Define the GPIO pin where DHT11 is connected
gpio_t gpio_dht11 = GPIO_PIN(0, 2);

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

static struct etimer et;

PROCESS_THREAD(dht11_process, ev, data)
{
    PROCESS_BEGIN();

    // Initialize the GPIO for DHT11
    gpio_init(gpio_dht11, GPIO_IN);

    while (1) {
        // Set a timer for 5 seconds
        etimer_set(&et, CLOCK_SECOND * 5);

        // Wait for the timer event
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);

        // Activate the DHT11 sensor
        SENSORS_ACTIVATE(dht11_sensor);

        // Read the values from the DHT11 sensor
        int temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
        int humidity = dht11_sensor.value(DHT11_SENSOR_HUMIDITY);

        // Print the read values
        printf("Temperature: %d.%d C\n", temperature / 10, temperature % 10);
        printf("Humidity: %d.%d %%\n", humidity / 10, humidity % 10);

        // Deactivate the DHT11 sensor
        SENSORS_DEACTIVATE(dht11_sensor);
    }

    PROCESS_END();
}

int main() {
    process_start(&dht11_process, NULL);
    return 0;
}