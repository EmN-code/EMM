#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define DHT_PIN      GPIO_PIN(PORT_A, 5)      // Assuming GPIO8 as PORT_A, pin 5
#define LED_PIN      GPIO_PIN(PORT_B, 0)      // Assuming the LED is connected to GPIO0 of PORT_B
#define THRESHOLD    30                       // Temperature threshold
#define INTERVAL     (5U * US_PER_SEC)        // 5 seconds interval

static gpio_t dht_pin = DHT_PIN;
static gpio_t led_pin = LED_PIN;

int main(void) {
    gpio_init(led_pin, GPIO_OUT);
    gpio_clear(led_pin);
    
    while (1) {
        int16_t temp, hum;
        
        if (dht_measure(dht_pin, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum);
            
            if (temp > THRESHOLD) {
                gpio_set(led_pin);  // turn on LED
            } else {
                gpio_clear(led_pin);  // turn off LED
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        
        xtimer_sleep(INTERVAL);
    }
    return 0;
}