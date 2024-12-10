#include "os.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "dht.h"
#include "xtimer.h"

#define GPIO_PIN_DHT11 GPIO_PIN(0, 2)

void read_dht11(void) {
    int16_t temperature = 0;
    int16_t humidity = 0;
    
    if (dht_read(GPIO_PIN_DHT11, &temperature, &humidity) == DHT_OK) {
        printf("Temperature: %d.%d°C\n", temperature / 10, abs(temperature % 10));
        printf("Humidity: %d.%d%%\n", humidity / 10, abs(humidity % 10));
    } else {
        printf("Failed to read from DHT11 sensor.\n");
    }
}

int main(void) {
    gpio_init(GPIO_PIN_DHT11, GPIO_IN);
    
    while (1) {
        read_dht11();
        xtimer_sleep(5);
    }
    return 0;
}