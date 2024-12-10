#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "msg.h"
#include "thread.h"
#include "net/sock/udp.h"
#include "net/mqtt.h"
#include "net/ipv4/addr.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_USERNAME "your_username"
#define MQTT_PASSWORD "your_password"
#define MQTT_KEEPALIVE 60
#define MQTT_PUBLISH_INTERVAL (5 * 1000 * 1000) // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static char sendbuf[128];
static char readbuf[128];
static mqtt_client_t client;

static void _on_puback(mqtt_client_t *client, void *param, const mqtt_packet_t *pk) {
    (void)client;
    (void)param;
    (void)pk;
    printf("PUBLISH acknowledged\n");
}

static void _on_connect(mqtt_client_t *client, void *param, mqtt_return_code_t return_code) {
    (void)client;
    (void)param;
    if (return_code == MQTT_CONNECT_ACCEPTED) {
        printf("MQTT connected successfully\n");
    } else {
        printf("MQTT connection failed with code %d\n", return_code);
    }
}

static void _on_disconnect(mqtt_client_t *client, void *param, mqtt_return_code_t return_code) {
    (void)client;
    (void)param;
    (void)return_code;
    printf("MQTT disconnected\n");
}

static void _on_subscribe(mqtt_client_t *client, void *param, const char *topic, uint8_t qos, mqtt_return_code_t return_code) {
    (void)client;
    (void)param;
    (void)topic;
    (void)qos;
    (void)return_code;
    printf("SUBSCRIBE acknowledged\n");
}

static void _on_unsubscribe(mqtt_client_t *client, void *param, const char *topic, mqtt_return_code_t return_code) {
    (void)client;
    (void)param;
    (void)topic;
    (void)return_code;
    printf("UNSUBSCRIBE acknowledged\n");
}

static void _on_message(mqtt_client_t *client, void *param, const char *topic, const char *msg, uint16_t len) {
    (void)client;
    (void)param;
    (void)topic;
    (void)msg;
    (void)len;
    printf("Received message on topic '%s': %.*s\n", topic, len, msg);
}

static void _publish_state(void) {
    static bool state = true;
    char state_str[10];
    snprintf(state_str, sizeof(state_str), "%s", state ? "work" : "not work");

    mqtt_topic_t topic = MQTT_TOPIC;
    mqtt_message_t msg = {
        .qos = MQTT_QOS_0,
        .retain = false,
        .payload = state_str,
        .payload_len = strlen(state_str),
    };

    mqtt_publish(&client, &topic, &msg, _on_puback, NULL);

    state = !state; // Toggle state
}

static void *mqtt_task(void *arg) {
    (void)arg;

    struct sockaddr_in broker_addr;
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.sin_addr);

    mqtt_client_init(&client, NULL, MQTT_BROKER_PORT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    mqtt_set_connect_handler(&client, _on_connect, NULL);
    mqtt_set_disconnect_handler(&client, _on_disconnect, NULL);
    mqtt_set_publish_handler(&client, _on_message, NULL);

    mqtt_connect(&client, (struct sockaddr *)&broker_addr, MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD, MQTT_KEEPALIVE, NULL);

    while (1) {
        mqtt_yield(&client, MQTT_PUBLISH_INTERVAL);
        _publish_state();
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, mqtt_task, NULL, "mqtt_task");
    return 0;
}