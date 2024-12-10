#include "sys/process.h"
#include "contiki-default-conf.h"
#include "contiki-conf.h"

process_event_t event_data_ready;

PROCESS(dht11_process, "DHT11 Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data) {
    static struct etimer et;

    PROCESS_BEGIN();

    etimer_set(&et, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Mock reading DHT11 sensor and checking for status
        int status = 0; // Assuming 0 is a successful read
        if (status != 0) {
            // Handle error
            printf("Error reading DHT11 sensor\n");
            continue;
        }

        // Turn on RGB LED to magenta
        printf("Turning on RGB LED to magenta\n");

        etimer_reset(&et);
    }

    PROCESS_END();
}