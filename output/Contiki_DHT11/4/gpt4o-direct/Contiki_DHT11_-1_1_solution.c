#include "dht.h"
#include "contiki.h"
#include "nrf_gpio.h"

PROCESS(dht11_process, "DHT11 Process");
AUTOSTART_PROCESSES(&dht11_process);

static struct etimer timer;
#define DHT11_PIN 2

PROCESS_THREAD(dht11_process, ev, data) {
    PROCESS_BEGIN();

    // Initialize DHT11
    dht_init(DHT11_PIN);

    // Create a timer to fire every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        // Wait for the timer event
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        int16_t temperature, humidity;
        if (dht_read(&temperature, &humidity) == DHT_SUCCESS) {
            printf("Temperature: %d.%d C, Humidity: %d.%d %%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}
