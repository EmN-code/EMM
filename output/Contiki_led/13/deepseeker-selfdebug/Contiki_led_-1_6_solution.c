
#include "xtimer.h"
#include "dht11_sensor.h"

#define LED_PIN GPIO_PIN(PORT_A, 5)
#define DHT11_PIN GPIO_PIN(PORT_A, 6)

void rgb_led_set(uint8_t colour) {
    // Simulate setting the RGB LED color
    (void)colour; // Suppress unused variable warning
}

int main(void) {
    // Initialize the DHT11 sensor
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_PIN);

    while (1) {
        // Read the DHT11 sensor
        int16_t humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
        int16_t temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

        // Turn on the RGB LED
        rgb_led_set(RGB_LED_WHITE);

        // Print the sensor values
        printf("Humidity: %d, Temperature: %d\n", humidity, temperature);

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}
