#include <stdio.h>
#include <string.h>
#include "thread.h"
#include "ztimer.h"
#include "net/sock/tcp.h"
#include "paho_mqtt.h"
#include "esp_wifi.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE_WORK "work"
#define MQTT_MESSAGE_NOT_WORK "not work"
#define PUBLISH_INTERVAL (5U * MS_PER_SEC)

static mqtt_client_t mqtt_client;
static sock_tcp_t sock;
static mqtt_connect_client_info_t mqtt_client_info = {
    .client_id = MQTT_CLIENT_ID,
    .keep_alive = 60,
    .clean_session = 1,
};

static void on_message_received(mqtt_client_t *client, mqtt_message_data_t *msg)
{
    (void)client;
    printf("Received message on topic %s: %.*s\n", msg->topic->lenstring.data,
           (int)msg->message->payloadlen, (char *)msg->message->payload);
}

static void *mqtt_thread(void *arg)
{
    (void)arg;
    mqtt_client_t *client = &mqtt_client;
    mqtt_message_t message;
    int res;

    while (1) {
        message.payload = (void *)MQTT_MESSAGE_WORK;
        message.payloadlen = strlen(MQTT_MESSAGE_WORK);
        message.qos = MQTT_QOS0;
        message.retained = 0;
        message.dup = 0;

        res = mqtt_publish(client, MQTT_TOPIC, &message);
        if (res != MQTT_SUCCESS) {
            printf("Error publishing message: %d\n", res);
        } else {
            printf("Published message: %s\n", (char *)message.payload);
        }

        ztimer_sleep(ZTIMER_MSEC, PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    printf("MQTT publisher example\n");

    /* Initialize WiFi */
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_connect();

    /* Initialize MQTT client */
    mqtt_client_init(&mqtt_client, &sock, on_message_received, NULL);

    /* Connect to the MQTT broker */
    sock_tcp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.family = AF_INET;
    remote.port = MQTT_BROKER_PORT;
    ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, MQTT_BROKER_IP);

    if (sock_tcp_connect(&sock, &remote, 0, 0) < 0) {
        printf("Error connecting to MQTT broker\n");
        return 1;
    }

    if (mqtt_connect(&mqtt_client, &mqtt_client_info, NULL, 0, NULL, 0, 0, 0) != MQTT_SUCCESS) {
        printf("Error connecting to MQTT broker\n");
        return 1;
    }

    printf("Connected to MQTT broker at %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);

    /* Start MQTT thread */
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  mqtt_thread, NULL, "mqtt_thread");

    /* Main loop */
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}