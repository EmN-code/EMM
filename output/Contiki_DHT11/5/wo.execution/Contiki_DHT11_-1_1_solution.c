#include "contiki-default-conf.h"
#include "contiki-conf.h"
#include "dev/gpio.h"
#include "dht11.h"
#include <stdio.h>

PROCESS(read_dht11_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&read_dht11_process);

PROCESS_THREAD(read_dht11_process, ev, data){
    static struct etimer timer;
    static int temperature, humidity;

    PROCESS_BEGIN();
    
    // Initialize the DHT11 sensor
    dht11_init();

    // Set the timer for 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        // Wait for the timer event
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read from DHT11 sensor connected to GPIO pin 2
        if (dht11_read(&temperature, &humidity) != -1) {
            printf("DHT11 Temperature: %d, Humidity: %d\n", temperature, humidity);
        } else {
            printf("Failed to read from DHT11 sensor.\n");
        }

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void) {
    process_start(&read_dht11_process, NULL);
    return 0;
}