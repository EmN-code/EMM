#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mqtt_client.h>
#include <net/gnrc.h>
#include <xtimer.h>
#include <thread.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define PUB_INTERVAL (5U * US_PER_SEC)  // 5 seconds

static char stack[THREAD_STACKSIZE_MAIN];

/* Function to publish state */
static void *publish_state(void *arg) {
    (void)arg;
    mqtt_client_t client;
    mqtt_message_t msg;
    msg.qos = MQTT_QOS_0;
    msg.topic = "/state";
    
    /* Connect to the MQTT broker */
    if (mqtt_client_connect(&client, SERVER_IP, SERVER_PORT, PUB_INTERVAL) != 0) {
        printf("Failed to connect to the broker\n");
        return NULL;
    }

    while (1) {
        /* Publish the message */
        msg.payload = "work";
        if (mqtt_client_publish(&client, &msg) != 0) {
            printf("Failed to publish\n");
        } else {
            printf("State published: %s\n", msg.payload);
        }

        /* Sleep for the defined interval */
        xtimer_sleep(PUB_INTERVAL);
    }

    /* Disconnect */
    mqtt_client_disconnect(&client);
    return NULL;
}

int main(void) {
    printf("MQTT Publisher started\n");
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  publish_state, NULL, "pub_thread");
    return 0;
}