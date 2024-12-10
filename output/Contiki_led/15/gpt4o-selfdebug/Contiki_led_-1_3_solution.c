#include "net/netstack.h"
#include "dev/leds.h"
#include "contiki.h"

PROCESS(example_process, "Example Process");
AUTOSTART_PROCESSES(&example_process);

PROCESS_THREAD(example_process, ev, data)
{
    static struct etimer timer;

    PROCESS_BEGIN();

    // Initialize the timer to fire every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        // Wait for the timer to expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor (assuming a function dht11_read is defined elsewhere)
        int humidity = dht11_read(DHT11_VALUE_HUMIDITY_INTEGER);
        if(humidity != DHT11_STATUS_CHECKSUM_FAILED) {
            // If read is successful, turn on an RGB LED with a predefined colour
            rgb_led_set(RGB_LED_CYAN);
        } else {
            // Handle reading failure
        }

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main() {
    process_start(&example_process, NULL);
    return 0;
}