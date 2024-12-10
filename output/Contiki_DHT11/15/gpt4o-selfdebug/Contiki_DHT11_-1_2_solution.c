#include "dev/gpio-hal.h"
#include "contiki-default-conf.h"
#include "contiki-lib.h"

PROCESS(dht11_read_process, "DHT11 Sensor Reading Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
    static struct etimer timer;
    static int temperature;
    static int humidity;

    PROCESS_BEGIN();

    // Initialize GPIO pin for DHT11
    gpio_hal_arch_pin_set_input(GPIO_HAL_PIN(0, 2));

    // Set the timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);
    
    while(1) {
        // Wait until the timer expires
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Reset the timer
        etimer_reset(&timer);

        // Read DHT11 values
        temperature = DHT11_VALUE_TEMP_INTEGER;
        humidity = DHT11_VALUE_HUMIDITY_INTEGER;

        printf("Temperature: %d C\n", temperature);
        printf("Humidity: %d %%\n", humidity);
    }

    PROCESS_END();
}
