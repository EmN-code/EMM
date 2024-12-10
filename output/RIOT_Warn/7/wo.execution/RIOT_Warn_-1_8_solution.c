#include "ztimer.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "esp_system.h"
#include "dht.h"
#include "dht_params.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "arpa/inet.h"
#include "msg.h"
#include "kernel_defines.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "MBQTTClient.h"
#include "mutex.h"
#include "paho_mqtt.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "mbox.h"
#include "timex.h"
#include "time_units.h"
#include "unistd.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "ztimer.h"

#define DHT_PIN GPIO_NUM_4
#define LED_PIN GPIO_NUM_2
#define TEMP_THRESHOLD 30  // 30 degrees Celsius

dht_t dev;

void read_dht(void)
{
    int16_t temperature, humidity;
    if (dht_read(&dev, &temperature, &humidity) == 0) {
        printf("Temperature: %d C, Humidity: %d%%\n", temperature, humidity);
        if (temperature > TEMP_THRESHOLD) {
            // Turn LED on
            esp_idf_gpio_set_drive_capability(LED_PIN, GPIO_DRIVE_CAP_STRONG);
            gpio_set(GPIO_NUM_2);
        }
        else {
            // Turn LED off using equivalent clear if available
        }
    }
    else {
        printf("Error reading DHT sensor\n");
    }
}

int main(void)
{
    // Initialize GPIO
    esp_idf_gpio_set_drive_capability(DHT_PIN, GPIO_DRIVE_CAP_STRONG);

    while (1) {
        read_dht();
        ztimer_sleep(ZTIMER_MSEC, 5000);  // sleep for 5 seconds
    }
    return 0;
}
