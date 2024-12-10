#include "paho_mqtt.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"

#define LED_PIN GPIO_PIN(0, 2)  // Example definition for the LED pin
#define TEMP_THRESHOLD 30       // Temperature threshold in degrees Celsius

void read_and_check_temp(void) {
    int16_t temp = 0;
    int16_t hum = 0;
    dht_t dev;
    if (dht_init(&dev, &dht_params[0]) == 0) {  // Assuming params are somehow defined and initialized
        dht_read(&dev, &temp, &hum);
        if (temp > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);  // Turn on LED
        } else {
            gpio_clear(LED_PIN);  // Turn off LED
        }
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    
    while (1) {
        read_and_check_temp();
        ztimer_sleep(ZTIMER_SEC, 5);  // Sleep for 5 seconds
    }
    return 0;
}
