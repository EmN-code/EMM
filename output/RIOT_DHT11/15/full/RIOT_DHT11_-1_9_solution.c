#include "ztimer.h"
#include "msg.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "stdio.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "timex.h"
#include "unistd.h"
#include "time_units.h"
#include "paho_mqtt.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "MQTTClient.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "architecture.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "thread.h"
#include "xtimer.h"

#define DHT_TYPE DHT11         //Specify the DHT type
#define DHT_PIN GPIO_PIN(0,2)  

static dht_t dev;

int main(void) {
    printf("Initializing DHT11 sensor\n");
    
    if (dht_init(&dev, &(dht_params_t){.pin = DHT_PIN, .type = DHT_TYPE}) != DHT_OK) {
        printf("DHT initialization failed\n");
        return 1;
    }

    int16_t temp, hum;
    while (1) {
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read DHT sensor\n");
        }
        else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",temp / 10, temp % 10, hum / 10, hum % 10);
        }
        xtimer_sleep(5);
    }

    return 0;
}