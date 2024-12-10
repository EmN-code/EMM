#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <msg.h>
#include <mqtt.h>
#include <mqtt_client.h>
#include <xtimer.h>
#include <timex.h>
#include <ztimer.h>
#include <sys/types.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <arpa/inet.h>
#include <kernel_defines.h>
#include <stdbool.h>
#include <pthread.h>

#define INTERVAL (1U * US_PER_SEC)
#define LED_PIN 22

void *led_toggle_thread(void *arg) {
    (void)arg;
    while (1) {
        // Simulated LED toggle
        printf("LED Toggled\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    pthread_t thread;

    // Create a separate thread for handling LED toggle
    if (pthread_create(&thread, NULL, led_toggle_thread, NULL) != 0) {
        printf("Failed to create thread\n");
        return -1;
    }

    // Here should come the MQTT connection logic or any other network setup
    // Mocking network connection
    int rc = -1;
    while (rc != 0) {
        // Mock Network connect function
        rc = 0;  // For successful connection mock
        if(rc!=0){
            printf("Network connection failed, retrying in 1 second...\n");
            xtimer_sleep(1);
        }
    }
    printf("Network connected successfully\n");

    // Joining thread (in this simple example, but usually, it would run separately)
    pthread_join(thread, NULL);

    return 0;
}
