
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/netif.h>
#include <net/ipv4/addr.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_t sock;
static mqtt_client_t mqtt_client;
static char sendbuf[128];
static char readbuf[128];

void mqtt_connect(void) {
    mqtt_client_init(&mqtt_client);
    mqtt_client.sendbuf = sendbuf;
    mqtt_client.sendbuf_size = sizeof(sendbuf);
    mqtt_client.readbuf = readbuf;
    mqtt_client.readbuf_size = sizeof(readbuf);

    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    sock_udp_create(&sock, &local, &remote, 0);

    MQTTClientInit(&mqtt_client, (Network *)&sock, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPublish(&mqtt_client, MQTT_TOPIC, &(MQTTMessage) {
        .qos = MQTT_QOS,
        .retained = MQTT_RETAIN,
        .dup = 0,
        .payload = (void *)MQTT_MESSAGE,
        .payloadlen = strlen(MQTT_MESSAGE)
    });
}

void mqtt_publish(void) {
    static char state[] = "work";
    static int toggle = 0;

    if (toggle) {
        state[0] = 'n';
        state[1] = 'o';
        state[2] = 't';
        state[3] = ' ';
        state[4] = 'w';
        state[5] = 'o';
        state[6] = 'r';
        state[7] = 'k';
        state[8] = '\0';
    } else {
        state[0] = 'w';
        state[1] = 'o';
        state[2] = 'r';
        state[3] = 'k';
        state[4] = '\0';
    }

    toggle = !toggle;

    MQTTPublish(&mqtt_client, MQTT_TOPIC, &(MQTTMessage) {
        .qos = MQTT_QOS,
        .retained = MQTT_RETAIN,
        .dup = 0,
        .payload = (void *)state,
        .payloadlen = strlen(state)
    });
}

int main(void) {
    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
