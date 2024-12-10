#include "contiki.h"
#include "dev/gpio-hal.h"

#define DHT11_SENSOR_PORT GPIO_PORT(0)
#define DHT11_SENSOR_PIN  GPIO_PIN(2)

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

// Function to simulate reading DHT11 Data
static int read_dht11_data(void) {
    // Simulate reading temperature and humidity
    int humidity = 60;
    int temperature = 25;
    printf("Humidity: %d%%, Temperature: %dC\n", humidity, temperature);
    return 0;
}

PROCESS_THREAD(dht11_process, ev, data)
{
    static struct etimer timer;

    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Call function to read data from DHT11 (Simulated)
        read_dht11_data();

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}