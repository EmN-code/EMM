#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki-default-conf.h"
#include "contiki-conf.h"
#include "contiki.h"

#define DHT11_GPIO_PIN 2
#define DHT11_READ_INTERVAL (CLOCK_SECOND * 5)

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

static struct etimer timer;

PROCESS_THREAD(dht11_read_process, ev, data) {
    PROCESS_BEGIN();
    
    // Configure the DHT11 sensor GPIO pin
    DHT11_CONFIGURE_GPIO_PORT = 0; // Assume port 0 for this example
    DHT11_CONFIGURE_GPIO_PIN = DHT11_GPIO_PIN;

    while(1) {
        // Set the timer for the reading interval
        etimer_set(&timer, DHT11_READ_INTERVAL);
        
        // Wait for the timer event
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read from the DHT11 sensor
        int temperature = DHT11_VALUE_TEMPERATURE_INTEGER;
        int humidity = DHT11_VALUE_HUMIDITY_INTEGER;

        // Simulate reading sensor data
        printf("DHT11 sensor reading: Temperature = %d, Humidity = %d\n", temperature, humidity);
    }

    PROCESS_END();
}

int main(void) {
    // Initialize the Contiki system
    process_init();
    
    // Start the processes
    process_start(&dht11_read_process, NULL);

    // Main loop
    while(1) {
        process_run();
        etimer_request_poll();
    }
}