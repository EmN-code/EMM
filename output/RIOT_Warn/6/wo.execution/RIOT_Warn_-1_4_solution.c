#include "thread.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "paho_mqtt.h"
#include "periph/gpio.h"

// Define constants for GPIO pins and temperature threshold
#define DHT11_PIN  GPIO_PIN(0, 1) // Example pin
#define LED_PIN    GPIO_PIN(0, 2) // Example pin
#define TEMP_THRESHOLD 30 // Temperature threshold in Celsius

// Function prototypes
void configure_pins(void);
void read_dht11(void);

int main(void) {
    configure_pins();
    
    while (1) {
        read_dht11();
        ztimer_sleep(ZTIMER_MSEC, 5000); // Sleep for 5 seconds
    }
    return 0;
}

void configure_pins(void) {
    // Setup GPIO for DHT11 and LED
    gpio_init(DHT11_PIN, GPIO_IN);
    gpio_init(LED_PIN, GPIO_OUT);
}

void read_dht11(void) {
    int16_t temperature = 0;
    int16_t humidity = 0;
    
    if (dht_read(&temperature, &humidity, DHT11_PIN) == DHT_OK) {
        printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
        if (temperature > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
    } else {
        printf("Failed to read from DHT11 sensor.\n");
    }
}