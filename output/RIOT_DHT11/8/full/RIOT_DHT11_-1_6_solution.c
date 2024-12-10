#include "paho_mqtt.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mqtt/MQTTClient.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "thread.h"
#include "dht_params.h"
#include "dht.h"

#define GPIO_PIN 2
#define MQTT_BROKER_IP "<your_mqtt_broker_ip>"
#define MQTT_BROKER_PORT 1883

// Function to initialize the MQTT connection with retries
void initialize_mqtt_connection(Network* network) {
    int rc;
    while (1) {
        rc = NetworkConnect(network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if(rc != 0){
            printf("Connect failed, retrying in 1 second...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Connect successfully\n");
            break;
        }
    }
}

// Function to read DHT11 sensor data
void read_dht11_sensor(dht_t *dev, int16_t *temp, int16_t *hum) {
    int rc = dht_read(dev, temp, hum);
    if (rc != 0) {
        printf("Error reading DHT11 sensor\n");
    } else {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", *temp / 10, *temp % 10, *hum / 10, *hum % 10);
    }
}

int main(void) {
    dht_t dev;
    Network network;
    int16_t temperature, humidity;
    
    // Initialize the GPIO pin for DHT sensor
    if (gpio_init(GPIO_PIN, GPIO_IN) < 0) {
        printf("Failed to initialize GPIO\n");
        return 1;
    }

    // Initialize MQTT connection
    initialize_mqtt_connection(&network);

    while (1) {
        // Read data from DHT11 sensor
        read_dht11_sensor(&dev, &temperature, &humidity);
        
        // Sleep for 5 seconds
        xtimer_sleep(5);
    }
    return 0;
}