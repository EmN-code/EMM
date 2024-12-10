#include "contiki.h"
#include "dev/leds.h"  // Required for LED operations
#include "sys/etimer.h"  // Required for timer operations

#define DHT11_GPIO_PIN 2

PROCESS(dht11_reader_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&dht11_reader_process);

// Function to simulate reading from DHT11 (Humidity and Temperature Sensor)
int read_dht11(uint8_t pin, int value_type) {
    // Simulate reading from DHT11; returns some dummy value
    if (value_type == DHT11_VALUE_HUMIDITY_INTEGER) {
        return 50;  // Example humidity value
    } else if (value_type == DHT11_VALUE_TEMPERATURE_INTEGER) {
        return 22;  // Example temperature value
    }
    return 0;
}

PROCESS_THREAD(dht11_reader_process, ev, data) {
    static struct etimer timer;
    static int humidity, temperature;

    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 5);  // Set timer for 5 seconds

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        humidity = read_dht11(DHT11_GPIO_PIN, DHT11_VALUE_HUMIDITY_INTEGER);
        temperature = read_dht11(DHT11_GPIO_PIN, DHT11_VALUE_TEMPERATURE_INTEGER);

        printf("Humidity: %d\% Temperature: %d\n", humidity, temperature);

        etimer_reset(&timer);  // Reset the timer to measure again in 5 seconds
    }

    PROCESS_END();
}
