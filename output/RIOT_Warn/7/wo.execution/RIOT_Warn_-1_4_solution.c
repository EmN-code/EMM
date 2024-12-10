#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "thread.h"

#define DHT11_PIN 2           // Assuming GPIO2 for DHT11
#define LED_PIN 3             // Assuming GPIO3 for LED
#define TEMP_THRESHOLD 30     // Temperature threshold in degree Celsius

int main(void) {
    int16_t temp, hum;
    dht_t dev;
    int res;
    
    // Initialize DHT11 device
    dht_params_t params;
    params.pin = DHT11_PIN;
    params.type = DHT11;
    
    if ((res = dht_init(&dev, &params)) != 0) {
        // Handle initialization error
        return 1;
    }

    // Initialize LED GPIO
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        // Read temperature and humidity
        res = dht_read(&dev, &temp, &hum);
        if (res == 0) {
            if (temp > TEMP_THRESHOLD * 10) {  // Temp is returned multiplied by 10
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            // Handle read error
        }
        
        // Sleep for 5 seconds
        xtimer_sleep(5);
    }
    
    return 0;
}