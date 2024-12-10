#include <stdio.h>
#include <string.h>
#include <net/gnrc/netif.h>
#include <net/gnrc.h>
#include <net/emcute.h>
#include <thread.h>
#include <xtimer.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define MQTT_TOPIC "device/state"
#define DEVICE_STATE "work or not work"
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

char stack[THREAD_STACKSIZE_DEFAULT];

void *mqtt_pub_thread(void *arg) {
    (void)arg;
    gnrc_netif_t *netif = NULL;

    /* Get network interface */
    netif = gnrc_netif_iter(NULL);

    while (netif == NULL) {
        netif = gnrc_netif_iter(NULL);
        xtimer_sleep(1);
    }

    printf("Network interface found: %%s\n", gnrc_netif_name(netif));

    /* Initialize MQTT connection, assume network is set up */
    printf("MQTT publisher started.\n");

    while (1) {
        /* Simulate MQTT publish */
        printf("Publishing to %s:%d on topic '%s': %s\n", SERVER_ADDR, SERVER_PORT, MQTT_TOPIC, DEVICE_STATE);
        
        /* Wait for the next publish cycle */
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void) {
    printf("MQTT Publisher initializing...\n");
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, mqtt_pub_thread, NULL, "mqtt_pub");
    return 0;
}