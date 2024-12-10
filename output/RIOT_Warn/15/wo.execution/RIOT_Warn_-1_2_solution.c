#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT11_PIN GPIO_PIN(0, 0)  // Define the GPIO pin where DHT11 is connected
#define LED_PIN GPIO_PIN(0, 1)    // Define the GPIO pin where the LED is connected
#define TEMP_THRESHOLD 30         // Define a temperature threshold

void read_dht11_and_control_led(void) {
    int16_t temperature, humidity;
    dht_t dev;
    dht_read(&dev, &temperature, &humidity);
    
    if (temperature > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
    } else {
        gpio_clear(LED_PIN);
    }
}

int main(void) {
    dht_init(&dev, DHT11_PIN);
    gpio_init(LED_PIN, GPIO_OUT);
    
    while (1) {
        read_dht11_and_control_led();
        xtimer_sleep(5);
    }
    
    return 0;
}