
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "sys/etimer.h"
#include "sys/timer.h"
#include "sys/ctimer.h"
#include "sys/rtimer.h"
#include "contiki-net.h"

#define DHT11_PIN 2

PROCESS(dht11_process, "DHT11 Reading Process");
AUTOSTART_PROCESSES(&dht11_process);

static void read_dht11(void) {
    // Simulate reading DHT11 data
    printf("Reading DHT11 data from GPIO %d\n", DHT11_PIN);
}

PROCESS_THREAD(dht11_process, ev, data) {
    static struct etimer et;

    PROCESS_BEGIN();

    etimer_set(&et, CLOCK_SECOND * 5); // Set timer to 5 seconds

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        read_dht11();

        etimer_reset(&et); // Reset the timer
    }

    PROCESS_END();
}

int main(void) {
    // Initialize Contiki
    contiki_init();

    // Start the DHT11 reading process
    process_start(&dht11_process, NULL);

    // Enter the Contiki main loop
    while(1) {
        process_run();
    }

    return 0;
}
