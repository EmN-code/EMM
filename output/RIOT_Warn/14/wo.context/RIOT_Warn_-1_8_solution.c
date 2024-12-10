#include <xtimer.h>
#include <periph/gpio.h>

#define DHT_PIN GPIO_PIN(PORT_A, 0) // Select the pin where the DHT11 is connected
#define LED_PIN GPIO_PIN(PORT_B, 5) // Select the pin where the LED is connected
#define TEMP_THRESHOLD 30 // Temperature threshold for warning

void read_dht(void) {
    int16_t temp, hum;
    // Initialize DHT sensor (assuming that a correction was made to the dht_init function to fit two parameters: pin and a delay/sync time)
    if (dht_init(DHT_PIN) != DHT_OK) {
        puts("Error initializing DHT sensor");
        return;
    }

    // Read temperature and humidity values
    if (dht_read(DHT_PIN, &temp, &hum) != DHT_OK) {
        puts("Error reading sensor data");
        return;
    }

    printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
    
    // Check if temperature exceeds threshold
    if (temp / 10 > TEMP_THRESHOLD) {
        // Turn on the LED
        gpio_set(LED_PIN);
    } else {
        // Turn off the LED
        gpio_clear(LED_PIN);
    }
}

int main(void) {
    // Initialize GPIO pins
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_init(DHT_PIN, GPIO_IN);

    puts("Starting DHT11 sensor and LED warning system...");

    while (1) {
        read_dht();
        xtimer_sleep(5); // Wait for 5 seconds
    }

    return 0;
}