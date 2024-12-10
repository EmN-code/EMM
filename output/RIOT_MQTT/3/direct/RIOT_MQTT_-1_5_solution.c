#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>
#include "net/mqtt.h"
#include "net/gnrc.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "esp32client"
#define USERNAME ""
#define PASSWORD ""
#define TOPIC "device/state"
#define MESSAGE_INTERVAL (5U * US_PER_SEC)

static mqtt_client_t mqtt_client;
static mqtt_qos_t qos = MQTT_QOS_0;

static void on_pub(void *arg, const char *topic, const void *message, size_t len)
{
    (void) arg;
    printf("Published to topic '%s': %.*s\n", topic, len, (const char *)message);
}

static void *mqtt_thread(void *arg)
{
    (void) arg;
    mqtt_connect(&mqtt_client, "esp32", BROKER_IP, BROKER_PORT, CLIENT_ID, USERNAME, PASSWORD, MQTT_CONNECT_KEEPALIVE(60), false);

    while (1) {
        const char *message = (((xtimer_now_usec() / US_PER_SEC) % 2) == 0) ? "work" : "not work";

        mqtt_publish(&mqtt_client, NULL, TOPIC, message, strlen(message), qos, false, on_pub);

        xtimer_sleep(MESSAGE_INTERVAL / US_PER_SEC);
    }

    mqtt_disconnect(&mqtt_client);
    return NULL;
}

int main(void)
{
    puts("MQTT example with ESP32 starting");

    /* Initialize the MQTT client */
    mqtt_client_init(&mqtt_client, &gnrc_mqtt_transport, NULL, NULL, NULL);

    /* Start the MQTT thread */
    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");
    return 0;
}