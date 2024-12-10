#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"
#include "msg.h"

#define DHT_PIN     GPIO_PIN(0, 4)  // Define the pin where the DHT11 is connected
#define LED_PIN     GPIO_PIN(0, 5)  // Define the pin where the LED is connected
#define TEMP_THRESHOLD 25           // Temperature threshold in Celsius

static dht_t dev;

void read_dht11_then_check_temp(void) {
    while (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        printf("Failed to initialize DHT11 sensor! Retrying...\n");
        xtimer_sleep(1);
    }
    
    int16_t temperature = 0;
    int16_t humidity = 0;
    
    if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
        printf("Failed to read from DHT11 sensor\n");
        return;
    }
    
    printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
    
    if (temperature / 10 > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
    } else {
        gpio_clear(LED_PIN);
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    
    while (1) {
        read_dht11_then_check_temp();
        xtimer_sleep(5);
    }
    
    return 0;
}