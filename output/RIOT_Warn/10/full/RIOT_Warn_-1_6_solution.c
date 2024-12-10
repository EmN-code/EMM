#include "thread.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "paho_mqtt.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "MQTTClient.h"

#define DHT_PIN GPIO_PIN(0, 14)
#define DHT_TYPE DHT11
#define LED_PIN GPIO_PIN(0, 15)
#define TEMP_THRESHOLD 30  // Threshold temperature in Celsius
#define SLEEP_INTERVAL 5   // Time between each sensor read in seconds

static dht_t dev;

void connect_mqtt(Network* network){
    int rc;
    while (1) {
        rc = NetworkConnect(network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0){
            printf("Network connection failed. Retrying...\n");
            sleep(1);
        } else {
            printf("Network connected successfully.\n");
            break;
        }
    }
}

int main(void) {
    printf("Starting the DHT11 and LED monitor\n");

    // Initialize GPIO for LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the DHT11 sensor
    if (dht_init(&dev, DHT_PIN, DHT_TYPE) != DHT_OK) {
        printf("Error initializing DHT11\n");
        return 1;
    }

    while (1) {
        int16_t temperature, humidity;
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            printf("Error reading from DHT11\n");
            continue;
        }
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
               temperature / 10, temperature % 10, humidity / 10, humidity % 10);
        
        // Check temperature and control LED
        if (temperature > TEMP_THRESHOLD * 10) {  // Comparing against threshold
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
        
        // Delay for the next reading
        sleep(SLEEP_INTERVAL);
    }

    return 0;
}