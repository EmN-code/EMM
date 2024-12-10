#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "time_units.h"
#include "timex.h"
#include "thread.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stdio.h"
#include "stddef.h"
#include "stdbool.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "msg.h"
#include "mbox.h"
#include "mutex.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "kernel_defines.h"
#include "net/emcute.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "dht.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"

#define TEMP_THRESHOLD 30
#define DHT_PIN GPIO_NUM_4
#define LED_PIN GPIO_NUM_2

void read_sensor(void) {
    int16_t temperature, humidity;
    dht_t device;
    
    while (1) {
        if (dht_read(&device, &temperature, &humidity) == ESP_OK) {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
            
            if(temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from sensor\n");
        }
        
        ztimer_sleep(ZTIMER_SEC, 5);
    }
}

int main(void) {
    // Initialize GPIO for LED
    esp_idf_gpio_set_intr_type(LED_PIN, GPIO_INTR_DISABLE);
    esp_idf_gpio_set_drive_capability(LED_PIN, GPIO_DRIVE_CAP_3);
    
    // Create a thread for reading the DHT sensor periodically
    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, read_sensor, NULL, "dht_reader");
    
    // Main thread will just go to sleep
    while(1) {
        ztimer_sleep(ZTIMER_SEC, 1);
    }
    return 0;
}