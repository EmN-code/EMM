#include "thread.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "paho_mqtt.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"
#include "MQTTClient.h"
#include "event/thread.h"

#define DHT_PIN GPIO_NUM_4 // Assuming GPIO 4 is where DHT is connected
#define LED_PIN GPIO_NUM_2 // Assuming GPIO 2 is where the LED is connected
#define TEMP_THRESHOLD 30 // Temperature threshold in degree Celsius
#define MQTT_BROKER_IP "192.168.1.100" // Placeholder IP
#define MQTT_BROKER_PORT 1883

static gpio_t led_pin = LED_PIN; // Define the LED pin
static dht_t dht_sensor;

// Function to setup GPIO pins
void setup_gpio()
{
    // Initialize DHT sensor
    dht_params_t params;
    params.pin = DHT_PIN;
    dht_init(&dht_sensor, &params);

    // Set LED pin to output
    gpio_init(led_pin, GPIO_OUT);
    gpio_clear(led_pin);
}

// Function to read temperature from DHT sensor
int read_temperature(int16_t *temperature, int16_t *humidity)
{
    while (1) {
        if (dht_read(&dht_sensor, temperature, humidity) == 0) {
            return 0; // Reading successful
        }
        // Retry reading after 1 second
        ztimer_sleep(ZTIMER_SEC, 1);
    }
}

// MQTT connection setup
void setup_mqtt(void)
{
    // Initialize networking and MQTT here (omitted for brevity)
    // Example: NetworkConnect, MQTTClientInit, etc.
}

// Main function
int main(void)
{
    printf("Starting DHT11 monitoring example\n");

    setup_gpio();
    setup_mqtt();

    int16_t temperature, humidity;

    while (1) {
        if (read_temperature(&temperature, &humidity) == 0) {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(led_pin);
            } else {
                gpio_clear(led_pin);
            }
        }
        
        // Delay of 5 seconds before the next read
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}
