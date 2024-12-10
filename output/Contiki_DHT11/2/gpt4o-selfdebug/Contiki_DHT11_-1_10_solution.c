#include <stdio.h>
#include "contiki.h"
#include "sys/etimer.h"
#include "dev/gpio.h"

#define DHT11_PIN 2  // Define the GPIO pin for DHT11, assuming pin number 2

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

static void read_dht11(void) {
    // Placeholder function to simulate DHT11 reading
    printf("Reading DHT11 on GPIO %d\n", DHT11_PIN);
}

PROCESS_THREAD(dht11_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    while(1) {
        etimer_set(&timer, CLOCK_SECOND * 5);

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        read_dht11();
    }

    PROCESS_END();
}