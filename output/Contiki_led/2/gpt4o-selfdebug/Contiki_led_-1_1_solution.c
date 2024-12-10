#include <contiki.h>
#include <dev/dht11.h>
#include <dev/leds.h>

PROCESS(dht11_read_process, "DHT11 Sensor Reading Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
    static struct etimer timer;
    PROCESS_BEGIN();

    while(1) {
        etimer_set(&timer, CLOCK_SECOND * 5);

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        int temperature, humidity;
        if(dht11_read(&temperature, &humidity) == DHT11_SUCCESS) {
            printf("Temperature: %d C, Humidity: %d%%\n", temperature, humidity);

            // Turn on RGB LED based on temperature value
            if (temperature > 25) {
                leds_on(LEDS_RED);
            } else if (temperature > 15) {
                leds_on(LEDS_GREEN);
            } else {
                leds_on(LEDS_BLUE);
            }
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }
    }

    PROCESS_END();
}
