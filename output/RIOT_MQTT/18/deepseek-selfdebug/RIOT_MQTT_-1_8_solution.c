
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_CLIENT_ID "riot_esp32"
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL
#define MQTT_KEEPALIVE 60
#define MQTT_WILL_TOPIC NULL
#define MQTT_WILL_MESSAGE NULL
#define MQTT_WILL_QOS 0
#define MQTT_WILL_RETAIN 0
#define MQTT_CLEAN_SESSION 1
#define MQTT_TIMEOUT 10000
#define MQTT_BUFFER_SIZE 1024
#define MQTT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static void mqtt_publish(void)
{
    sock_tcp_t sock;
    mqtt_client_t client;
    mqtt_connect_info_t conn_info = {
        .client_id = MQTT_CLIENT_ID,
        .username = MQTT_USERNAME,
        .password = MQTT_PASSWORD,
        .keep_alive = MQTT_KEEPALIVE,
        .will_topic = MQTT_WILL_TOPIC,
        .will_message = MQTT_WILL_MESSAGE,
        .will_qos = MQTT_WILL_QOS,
        .will_retain = MQTT_WILL_RETAIN,
        .clean_session = MQTT_CLEAN_SESSION
    };

    if (sock_tcp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating TCP socket");
        return;
    }

    if (sock_tcp_connect(&sock, MQTT_BROKER_IP, MQTT_BROKER_PORT, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error connecting to MQTT broker");
        sock_tcp_close(&sock);
        return;
    }

    mqtt_client_init(&client, &sock, MQTT_BUFFER_SIZE, MQTT_BUFFER_SIZE);

    if (mqtt_connect(&client, &conn_info, MQTT_TIMEOUT) != MQTT_OK) {
        puts("Error connecting to MQTT broker");
        sock_tcp_close(&sock);
        return;
    }

    mqtt_publish_simple(&client, MQTT_TOPIC, MQTT_MESSAGE, strlen(MQTT_MESSAGE), MQTT_QOS, MQTT_RETAIN);

    mqtt_disconnect(&client);
    sock_tcp_close(&sock);
}

int main(void)
{
    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
