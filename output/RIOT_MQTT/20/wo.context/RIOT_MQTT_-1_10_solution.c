#include <thread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mqtt_client.h>  // Ensure this is the LAST include

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL_SEC (5U)
#define MSG_WORK "work"
#define MSG_NOT_WORK "not work"

static char mqtt_thread_stack[THREAD_STACKSIZE_DEFAULT];
static mqtt_client_t mqtt_client;
static Network network;

static void mqtt_pub_handler(void) {
    mqtt_client_config_t conf;
    memset(&conf, 0, sizeof(conf));
    conf.broker_addr = MQTT_BROKER_IP;
    conf.broker_port = MQTT_BROKER_PORT;
    conf.message = (rand() % 2) ? MSG_WORK : MSG_NOT_WORK;

    mqtt_client_connect(&mqtt_client, &network, &conf);

    while (1) {
        mqtt_client_publish(&mqtt_client, conf.message, strlen(conf.message));
        xtimer_sleep(PUBLISH_INTERVAL_SEC);
    }
}

int main(void) {
    NetworkInit(&network);
    mqtt_client_init(&mqtt_client, &network);

    thread_create(mqtt_thread_stack, sizeof(mqtt_thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_pub_handler, NULL, "MQTT Pub Thread");

    return 0;
}