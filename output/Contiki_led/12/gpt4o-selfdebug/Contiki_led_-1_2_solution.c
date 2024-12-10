#include "contiki.h"
#include "sys/etimer.h"
#include "dev/dht11.h"
#include "dev/rgb_led.h"

PROCESS(dht11_rgb_led_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_led_process);

PROCESS_THREAD(dht11_rgb_led_process, ev, data) {
    static struct etimer timer;
    static int temperature, humidity;
    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        
        // Read the DHT11 sensor
        if (dht11_read(&temperature, &humidity) == 0) {
            printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);

            // Based on some condition, turn on the RGB LED
            // For simplicity, we will just turn it on regardless of the values
            rgb_led_set(RGB_LED_RED);
        } else {
            printf("Failed to read from DHT11\n");
        }

        etimer_reset(&timer);
    }

    PROCESS_END();
}