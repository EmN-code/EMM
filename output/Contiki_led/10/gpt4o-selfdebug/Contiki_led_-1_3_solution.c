#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki.h"
#include <stdio.h>
#include <stdint.h>

#define DHT11_READ_INTERVAL (CLOCK_SECOND * 5)

PROCESS(dht11_reader_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&dht11_reader_process);

static struct etimer timer;

/* Mock function to simulate DHT11 readings */
static int16_t read_dht11(void) {
    // Simulate reading temperature and humidity
    return 25; // Example temperature value
}

/* Mock function to control RGB LED based on the temperature */
static void control_rgb_led(int16_t temperature) {
    if (temperature > 24) {
        printf("Turning on RGB LED\n");
        // Code to turn on RGB LED
    } else {
        printf("Turning off RGB LED\n");
        // Code to turn off RGB LED
    }
}

PROCESS_THREAD(dht11_reader_process, ev, data) {
    PROCESS_BEGIN();

    etimer_set(&timer, DHT11_READ_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        int16_t temperature = read_dht11();
        printf("Temperature: %d\n", temperature);

        control_rgb_led(temperature);

        etimer_reset(&timer);
    }

    PROCESS_END();
}
