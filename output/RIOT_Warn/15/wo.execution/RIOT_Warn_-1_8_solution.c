#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "paho_mqtt.h"
#include "event/thread.h"
#include "thread.h"
#include "dht_params.h"
#include "dht.h"
#include "MQTTClient.h"

#define GPIO_PIN_LED    (GPIO_NUM_2) // Assume LED is connected to GPIO 2
#define DHT_PIN         (GPIO_NUM_4) // Assume DHT is connected to GPIO 4
#define TEMP_THRESHOLD  30           // Temperature threshold in degree Celsius

int main(void) {
    dht_t sensor;
    dht_params_t params;
    
    // Initialize DHT parameters
    params.pin = DHT_PIN;
    params.type = DHT11;
    
    // Initialize DHT sensor
    if (dht_init(&sensor, &params) != 0) {
        printf("Error: Unable to initialize DHT sensor\n");
        return -1;
    }
    
    // Configure the LED pin
    gpio_set(GPIO_PIN_LED);
    
    printf("System initialized. Monitoring temperature...\n");

    while(1) {
        int16_t temperature, humidity;

        // Read temperature and humidity
        if (dht_read(&sensor, &temperature, &humidity) == 0) {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);

            // Check if the temperature exceeds the threshold
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(GPIO_PIN_LED); // Turn on LED
            } else {
                gpio_clear(GPIO_PIN_LED); // Turn off LED
            }
        } else {
            printf("Error: Unable to read data from DHT sensor\n");
        }

        ztimer_sleep(ZTIMER_MSEC, 5000); // Sleep for 5 secs using ZTimer instead of XTimer
    }

    return 0;
}