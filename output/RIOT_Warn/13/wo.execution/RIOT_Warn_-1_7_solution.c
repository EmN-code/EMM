#include "periph/rtc.h"
#include "mqpa8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "paho_mqtt.h"
#include "periph/config.h"
#include "event/thread.h"
#include "periph/i2c.h"
#include "MQTTClient.h"
#include "thread.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN     GPIO_NUM_4
#define LED_PIN     GPIO_NUM_2
#define TEMP_THRESH 30  // temperature threshold in degree Celsius

void *read_dht11(void *arg) {
    dht_t dev;
    int16_t temperature = 0;
    int16_t humidity = 0;

    // Initialize DHT
    dev.pin = DHT_PIN; // assuming that correct pin member or function is available for the device
    dht_params_t params = dht_params[DHT11].dht_config;

    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == 0) {
            if (temperature > TEMP_THRESH) {
                gpio_set(LED_PIN);// turn on LED
            } else {
                gpio_clear(LED_PIN); // turn off LED
            }
        }
        vTaskDelay(pdMS_TO_TICKS(5000)); // assume some function for delay
    }
    return NULL;
}

int main(void) {
    char thread_stack[THREAD_STACKSIZE];
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN,
                  THREAD_FLAGS_CREATE_WOUT_YIELD, read_dht11, NULL, "dht_reader");
    return 0;
}