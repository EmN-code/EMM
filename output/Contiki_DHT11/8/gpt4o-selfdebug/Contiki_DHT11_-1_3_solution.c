#include "net/contiki.h"  // For contiki.lib and other networking functions
#include "dev/leds.h"  // To manage the LEDs on the board
#include "dev/dht11-sensor.h"  // Hypothetical header file for DHT11 sensor functions
#include "sys/etimer.h"  // For event timer system

#include <stdio.h>  // For standard IO operations

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

static struct etimer timer;

PROCESS_THREAD(dht11_read_process, ev, data)
{
    PROCESS_BEGIN();

    // Initialize the DHT11 sensor
    dht11_init(GPIO_PIN_2); // Placeholder: actual initialization may differ

    // Set timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read data
        int temp_integer = dht11_read(DHT11_VALUE_TEMPERATURE_INTEGER);
        int temp_decimal = dht11_read(DHT11_VALUE_TEMPERATURE_DECIMAL);
        int humidity_integer = dht11_read(DHT11_VALUE_HUMIDITY_INTEGER);
        int humidity_decimal = dht11_read(DHT11_VALUE_HUMIDITY_DECIMAL);

        printf("Temperature: %d.%d C
", temp_integer, temp_decimal);
        printf("Humidity: %d.%d %%
",  humidity_integer, humidity_decimal);

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void) {
    // Initialize Contiki
    process_start(&dht11_read_process, NULL);
    
    return 0;
}