#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki-default-conf.h"
#include <stdio.h>
#include <stdint.h>

#define CLOCK_SECOND 128
#define RGB_LED_RED 1
#define RGB_LED_GREEN 2
#define RGB_LED_BLUE 4

#define RGB_LED_WHITE   (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)
#define DHT11_STATUS_CHECKSUM_FAILED    (2)

int main(void) {
    static struct etimer timer;
    etimer_set(&timer, 5 * CLOCK_SECOND);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        printf("Reading DHT11 sensor\n");
        // Simulating reading from DHT11 sensor
        uint8_t status = 0; // Simulate reading status
        if (status != DHT11_STATUS_CHECKSUM_FAILED) {
            printf("DHT11 read success, turning on RGB LED to WHITE\n");
            // Turning the LED on to white
            int rgb_led = RGB_LED_WHITE;
        }
        else {
            printf("DHT11 read failed\n");
        }

        // Reset the timer
        etimer_reset(&timer);
    }
    return 0;
}