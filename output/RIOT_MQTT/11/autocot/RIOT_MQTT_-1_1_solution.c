#include "stdio.h"
#include "string.h"
#include "mqtt.h"
#include "mutex.h"
#include "thread.h"

#define MQTT_BROKER_ADDRESS "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * US_PER_SEC) /* 5 seconds interval */
#define BASE_STACK_SIZE 1024

static mqtt_client_t mqtt_client;
static char stack[BASE_STACK_SIZE];
static char sendbuf[256];
static char recvbuf[256];
static mutex_t mqtt_lock = MUTEX_INIT;

static void *publisher(void *arg) {
    (void)arg;
    mqtt_pub_t pub;
    int i = 0;

    while (1) {
        mutex_lock(&mqtt_lock);

        pub.topic = "state/pub";
        pub.payload = (i % 2 == 0) ? "work" : "not work";
        pub.retain = 0;
        pub.qos = MQTT_QOS_0;

        if (mqtt_publish(&mqtt_client, &pub) != MQTT_OK) {
            printf("Error: unable to publish\n");
        }

        mutex_unlock(&mqtt_lock);

        xtimer_usleep(PUBLISH_INTERVAL);
        i++;
    }

    return NULL;
}

int main(void) {
    mqtt_client_t client;
    mqtt_connect_client_t connect_info;

    connect_info.client_id = "esp32_mqtt_client";
    connect_info.user = NULL;
    connect_info.password = NULL;
    connect_info.keep_alive = 60;

    /* Initialize the MQTT client */
    mqtt_init(&client, MQTT_BROKER_ADDRESS, MQTT_BROKER_PORT, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf), xtimer_now_usec);

    /* Connect to the MQTT broker */
    if (mqtt_connect(&client, &connect_info) != MQTT_OK) {
        printf("Error: unable to connect to the broker\n");
        return 1;
    }

    /* Initialize the mutex */
    mutex_init(&mqtt_lock);

    /* Start the publisher thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, publisher, NULL, "publisher thread");

    return 0;
}