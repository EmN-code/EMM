#include <ztimer.h>
#include <unistd.h>
#include <timex.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <kernel_defines.h>

#define TEMP_THRESHOLD 25
#define READ_INTERVAL 5
#define GPIO_PIN 2

void* dht_read_thread(void *arg) {
    (void)arg;
    int16_t temperature, humidity;
    dht_t dev;
    // Assuming DHT11 sensor setup is completed earlier

    while (1) {
        int rc = dht_read(&dev, &temperature, &humidity);
        if (rc == 0) {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(GPIO_PIN); // Turn on an LED connected to GPIO_PIN
            }
        }
        xtimer_sleep(READ_INTERVAL);
    }
    return NULL;
}

int main(void) {
    puts("RIOT DHT11 example with LED warning\n");
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, dht_read_thread, NULL, "dht_read_thread");
    
    return 0;
}