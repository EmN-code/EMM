#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/mqtt/client.h"
#include "net/ipv4/addr.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_USERNAME "username"
#define MQTT_PASSWORD "password"
#define MQTT_KEEPALIVE 60
#define MQTT_PUBLISH_INTERVAL (5 * 1000 * 1000) // 5 seconds in microseconds

static char sendbuf[128];
static char readbuf[128];

static void on_puback(mqtt_client_t *client, void *param, uint16_t msg_id) {
    (void)client;
    (void)param;
    (void)msg_id;
    printf("PUBACK received\n");
}

static void on_suback(mqtt_client_t *client, void *param, uint16_t msg_id, uint8_t qos) {
    (void)client;
    (void)param;
    (void)msg_id;
    (void)qos;
    printf("SUBACK received\n");
}

static void on_disconnect(mqtt_client_t *client, void *param) {
    (void)client;
    (void)param;
    printf("Disconnected from MQTT broker\n");
}

static void on_connect(mqtt_client_t *client, void *param, mqtt_connack_t connack) {
    (void)client;
    (void)param;
    if (connack == MQTT_CONNACK_ACCEPTED) {
        printf("Connected to MQTT broker\n");
    } else {
        printf("Connection failed with code %u\n", connack);
    }
}

static void on_publish(mqtt_client_t *client, void *param, const char *topic, const void *data, size_t len, int qos, bool retain) {
    (void)client;
    (void)param;
    (void)topic;
    (void)data;
    (void)len;
    (void)qos;
    (void)retain;
    printf("PUBLISH received\n");
}

static void publish_state(mqtt_client_t *client) {
    static bool working = true;
    char state[10];
    if (working) {
        strcpy(state, "work");
    } else {
        strcpy(state, "not work");
    }
    working = !working;

    mqtt_message_t message = {
        .qos = MQTT_QOS_0,
        .retain = false,
        .topic = MQTT_TOPIC,
        .data = state,
        .len = strlen(state),
    };

    mqtt_publish(client, &message, on_puback, NULL);
}

static void mqtt_task(void *arg) {
    (void)arg;
    mqtt_client_t client;
    mqtt_client_init(&client, NULL, MQTT_BROKER_IP, MQTT_BROKER_PORT, MQTT_KEEPALIVE,
                     sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    mqtt_connect_data_t connect_data = {
        .client_id = MQTT_CLIENT_ID,
        .will_retain = false,
        .will_qos = MQTT_QOS_0,
        .clean_session = true,
        .keep_alive = MQTT_KEEPALIVE,
        .username = MQTT_USERNAME,
        .password = MQTT_PASSWORD,
    };

    mqtt_client_set_connect_cb(&client, on_connect, NULL);
    mqtt_client_set_disconnect_cb(&client, on_disconnect, NULL);
    mqtt_client_set_publish_cb(&client, on_publish, NULL);

    mqtt_connect(&client, &connect_data);

    while (1) {
        mqtt_yield(&client, MQTT_PUBLISH_INTERVAL);
        publish_state(&client);
    }
}

int main(void) {
    puts("Starting MQTT client");

    kernel_pid_t mqtt_pid = thread_create(
        "mqtt_task",
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_WOUT_YIELD,
        mqtt_task,
        NULL,
        "MQTT Task"
    );

    if (mqtt_pid < 0) {
        puts("Failed to create MQTT task");
        return 1;
    }

    puts("MQTT task created successfully");

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}