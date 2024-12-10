#include <stdio.h>
#include <pthread.h>
#include "dht11_sensor.h"
#include "contiki.h"

#define DHT11_PIN 0    // Pin where the DHT11 is connected
#define LED_PIN_R 1  // Pin for the Red LED
#define LED_PIN_G 2  // Pin for the Green LED
#define LED_PIN_B 3  // Pin for the Blue LED

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

void toggle_rgb_led(uint8_t r, uint8_t g, uint8_t b) {
    // Assume these functions are implemented to control the LEDs.
    printf("Toggle LED: R=%d, G=%d, B=%d\n", r, g, b);
    //GPIO_WRITE_PIN(LED_PIN_R, r);
    //GPIO_WRITE_PIN(LED_PIN_G, g);
    //GPIO_WRITE_PIN(LED_PIN_B, b);
}

PROCESS_THREAD(dht11_read_process, ev, data) {
    static struct etimer timer;
    PROCESS_BEGIN();

    while(1) {
        etimer_set(&timer, CLOCK_SECOND * 5);

        // Simulating reading temperature and humidity
        printf("Reading DHT11 on PIN %d\n", DHT11_PIN);
        // In a real scenario, you would call something like:
        // dht11_read(DHT11_PIN, &temperature, &humidity);
        int temperature = 25;  // Placeholders for real sensor values
        int humidity = 60;
        printf("Temperature: %d C, Humidity: %d \%\n", temperature, humidity);

        // Turn on the RGB LED
        toggle_rgb_led(1, 0, 0);  // Example to turn on the Red LED

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    }

    PROCESS_END();
}

int main(void) {
    printf("Starting DHT11 and RGB LED Control
");
    process_start(&dht11_read_process, NULL);
    while(1) {
        // Process should be polled regularly in a Contiki application
    }
    return 0;
}