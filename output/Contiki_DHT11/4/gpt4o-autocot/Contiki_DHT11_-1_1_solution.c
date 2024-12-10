#include "contiki-default-conf.h"
#include <stdio.h>
#include "contiki.h"

PROCESS(dht11_reader_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&dht11_reader_process);

#define DHT11_READ_INTERVAL (CLOCK_SECOND * 5)
#define DHT11_PIN 2

static struct etimer et;

PROCESS_THREAD(dht11_reader_process, ev, data) {
    PROCESS_BEGIN();

    printf("Starting DHT11 reader process\n");
    etimer_set(&et, DHT11_READ_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        // Simulated reading from DHT11 sensor on GPIO2
        printf("Reading data from DHT11 sensor on GPIO%d\n", DHT11_PIN);
        // Here you would add the code to actually read the sensor

        etimer_reset(&et);
    }

    PROCESS_END();
}