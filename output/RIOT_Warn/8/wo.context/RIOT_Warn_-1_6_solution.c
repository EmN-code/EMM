#include <xtimer.h> // for xtimer_sleep
#include <stdio.h> // for printf
#include <periph/gpio.h> // for GPIO functions
#include <net/emcute.h> // Mesh and network functionality in RIOT
#include <random.h> // for random number generation

#define DHT11_PIN GPIO_PIN(PORT_A, 0) // Use appropriate pin number and correct port
#define LED_PIN GPIO_PIN(PORT_B, 0) // LED pin for status indication
#define TEMP_THRESHOLD 30 // Temperature threshold for LED warning

void init_gpio() {
    // Initialize GPIO for DHT11 and LED
    if (gpio_init(DHT11_PIN, GPIO_IN_PD) < 0) {
        printf("Error: Unable to initialize DHT11 GPIO\n");
    }
    if (gpio_init(LED_PIN, GPIO_OUT) < 0) {
        printf("Error: Unable to initialize LED GPIO\n");
    } else {
        gpio_clear(LED_PIN);  // Ensure LED is off at start
    }
}

void read_dht11() {
    // Assume dht_init() and dht_measure() are available functions
    int16_t temperature, humidity;
    if (dht_init(DHT11_PIN, DHT11_) < 0) {
        printf("Error: Unable to initialize DHT11 sensor\n");
        return;
    }
    while (1) {
        // Measure temperature and humidity
        if (dht_measure(DHT11_PIN, &temperature, &humidity) == 0) {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
            if (temperature >= TEMP_THRESHOLD) {
                gpio_set(LED_PIN); // Turn on LED
            } else {
                gpio_clear(LED_PIN); // Turn off LED
            }
        } else {
            printf("Error: Unable to read from DHT11 sensor\n");
        }
        xtimer_sleep(5); // Sleep for 5 seconds
    }
}

int main(void) {
    init_gpio();
    read_dht11();
    return 0;
}