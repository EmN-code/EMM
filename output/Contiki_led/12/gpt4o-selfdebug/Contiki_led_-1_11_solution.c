#include "dev/leds.h"
#include "contiki-net.h"
#include "contiki-lib.h"
#include <stdio.h>

// Prototype for DHT11 reading function (simulated for the example)
int read_dht11(int type);

PROCESS(dht11_process, "DHT11 Reader");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data) {
    static struct etimer timer;
    static int temperature, humidity;

    PROCESS_BEGIN();

    // Set the etimer to 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read temperature and humidity from DHT11
        temperature = read_dht11(DHT11_VALUE_TEMPERATURE_INTEGER);
        humidity = read_dht11(DHT11_VALUE_HUMIDITY_INTEGER);

        printf("Temperature: %d*C, Humidity: %d%%\n", temperature, humidity);

        // Turn on the RGB Led in magenta color
        leds_on(LEDS_RED);
        leds_on(LEDS_BLUE);

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}

// Simulated function to read values from DHT11
int read_dht11(int type) {
    // Normally here would be the code interfacing with the actual sensor
    if (type == DHT11_VALUE_TEMPERATURE_INTEGER) {
        return 23; // Demo temperature value
    } else if (type == DHT11_VALUE_HUMIDITY_INTEGER) {
        return 60; // Demo humidity value
    }
    return 0;
}