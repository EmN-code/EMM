
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_KEEPALIVE 30
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_WILL_TOPIC NULL
#define MQTT_WILL_MESSAGE NULL
#define MQTT_WILL_QOS 0
#define MQTT_WILL_RETAIN 0
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL
#define MQTT_CLIENT_ID "riot_esp32"

#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void mqtt_connect(void)
{
    struct sockaddr_in server;
    sock_tcp_ep_t remote = { .family = AF_INET };
    int sock;

    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);
    remote.port = MQTT_BROKER_PORT;

    sock = sock_tcp_create(&remote, NULL, SOCK_FLAGS_REUSE_EP);
    if (sock < 0) {
        puts("Error creating TCP socket");
        return;
    }

    if (sock_tcp_connect(sock, &remote) < 0) {
        puts("Error connecting to MQTT broker");
        sock_tcp_close(sock);
        return;
    }

    mqtt_client_t client;
    mqtt_client_config_t config = {
        .keep_alive = MQTT_KEEPALIVE,
        .client_id = MQTT_CLIENT_ID,
        .username = MQTT_USERNAME,
        .password = MQTT_PASSWORD,
        .will_topic = MQTT_WILL_TOPIC,
        .will_message = MQTT_WILL_MESSAGE,
        .will_qos = MQTT_WILL_QOS,
        .will_retain = MQTT_WILL_RETAIN
    };

    if (mqtt_client_connect(&client, sock, &config) < 0) {
        puts("Error connecting to MQTT broker");
        sock_tcp_close(sock);
        return;
    }

    while (1) {
        mqtt_publish(&client, MQTT_TOPIC, MQTT_MESSAGE, strlen(MQTT_MESSAGE), MQTT_QOS, MQTT_RETAIN);
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    mqtt_client_disconnect(&client);
    sock_tcp_close(sock);
}

int main(void)
{
    puts("MQTT client starting");
    mqtt_connect();
    return 0;
}
