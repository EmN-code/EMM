#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "timex.h"
#include "thread_flags.h"
#include "thread.h"
#include "sys/socket.h"
#include "time_units.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "netinet/in.h"
#include "netinet/in.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdint.h"
#include "stdint.h"
#include "stdint.h"
#include "stdatomic.h"
#include "net/sock/util.h"
#include "net/emcute.h"
#include "paho_mqtt.h"
#include "paho_mqtt.h"
#include "nvs_flash.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "mutex.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "kernel_defines.h"
#include "kernel_defines.h"
#include "kernel_defines.h"
#include "ztimer.h"
#include "ztimer.h"
#include "ztimer.h"
#include "ztimer.h"
#include "ztimer.h"
#include "ztimer.h"
#include "ztimer.h"
#include "ztimer.h"
#include "ztimer.h"
#include "ztimer.h"
#include "ztimer.h"
#include "ztimer.h"
#include "ztimer.h"
#include "ztimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "event/thread.h"
#include "event/thread.h"
#include "unistd.h"
#include "unistd.h"
#include "unistd.h"
#include "unistd.h"
#include "unistd.h"
#include "unistd.h"
#include "unistd.h"
#include "unistd.h"
#include "unistd.h"
#include "unistd.h"
#include "unistd.h"
#include "unistd.h"
#include "unistd.h"
#include "unistd.h"
#include "unistd.h"
#include "unistd.h"
#include "unistd.h"
#include "unistd.h"
#include "unistd.h"
#include "unistd.h"
#include "unistd.h"
#include "unistd.h"
#include "unistd.h"
#include "unistd.h"
#include "unistd.h"
#include "unistd.h"
#include "unistd.h"
#include "unistd.h"
#include "stdatomic.h"
#include "stdatomic.h"
#include "stdatomic.h"
#include "stdatomic.h"
#include "stdatomic.h"
#include "stdatomic.h"
#include "stdatomic.h"
#include "stdatomic.h"
#include "stdatomic.h"
#include "stdatomic.h"
#include "stdatomic.h"
#include "stdatomic.h"
#include "stdatomic.h"
#include "stdatomic.h"
#include "stdatomic.h"
#include "stdatomic.h"
#include "atto.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "arpa/inet.h"

#define DHT_TYPE_DHT11 1

static dht_t dev;

int main(void) {
    int16_t temp, hum;
    int result = -1;

    // Initialize the flash memory
    while (nvs_flash_init() != 0) {
        printf("Flash initialization failed, retrying...\n");
        usleep(1000000); // 1 second
    }

    printf("Flash initialized\n");
    
    // Initialize the DHT11 sensor
    while (dht_init(&dev, GPIO_NUM_2, DHT_TYPE_DHT11) != 0) {
        printf("DHT sensor initialization failed, retrying...\n");
        usleep(1000000); // 1 second
    }

    printf("DHT sensor initialized\n");
    
    while (1) {
        // Read values from the DHT sensor
        result = dht_read(&dev, &temp, &hum);
        
        if (result == 0) {
            // Convert temp and hum from integer values to human-readable format
            float temperature = temp / 10.0;
            float humidity = hum / 10.0;
            printf("Temperature: %.1f°C, Humidity: %.1f%%\n", temperature, humidity);
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        ztimer_sleep(ZTIMER_MSEC, 5000); // Sleep for 5 seconds
    }

    return 0;
}