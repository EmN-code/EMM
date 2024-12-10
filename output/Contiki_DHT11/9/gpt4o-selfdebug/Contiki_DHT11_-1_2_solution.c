#include "contiki.h"
#include "sys/etimer.h"
#include "dev/dht11.h"

PROCESS(dht11_reader_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&dht11_reader_process);

PROCESS_THREAD(dht11_reader_process, ev, data)
{
    static struct etimer timer;
    static int temperature;
    static int humidity;

    PROCESS_BEGIN();

    // Configure DHT11 sensor
    dht11_init(GPIO_PIN);

    // Loop to read data every 5 seconds
    while (1) {
        // Set timer for 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);

        // Wait for timer to expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read temperature and humidity
        if (dht11_read(&temperature, &humidity) == DHT11_STATUS_OK) {
            printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }
    }

    PROCESS_END();
}
