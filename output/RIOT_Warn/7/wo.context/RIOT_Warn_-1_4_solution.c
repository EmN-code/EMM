#include <xtimer.h>
#include <timex.h>
#include <stdio.h>
#include <dht.h>

#define DHT11_PIN GPIO_PIN(PORT_A, 2)  // Change PORT_A and PORT_B to actual port numbers
#define LED_PIN GPIO_PIN(PORT_B, 5)
#define TEMP_THRESHOLD 30  // Temperature threshold to turn on the LED

static dht_t dht11_sensor;
static int16_t temp, hum;

void init_gpio(void) {
    // Initialize GPIO pins for DHT11 and LED
    gpio_init(DHT11_PIN, GPIO_IN);
    gpio_init(LED_PIN, GPIO_OUT);
}

void read_dht11(void) {
    if (dht_read(&dht11_sensor, &temp, &hum) != DHT_OK) {
        printf("Error reading DHT11 sensor.\n");
        return;
    }
    printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum);

    // Check if temperature exceeds threshold
    if (temp > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
    } else {
        gpio_clear(LED_PIN);
    }
}

int main(void) {
    // Initialize DHT11 sensor and GPIO
    init_gpio();
    dht_init(&dht11_sensor, DHT11_PIN, DHT11_DHT11);

    while (1) {
        read_dht11();
        xtimer_sleep(5);  // Wait for 5 seconds
    }
    return 0;
}