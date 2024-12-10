
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt_client.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_CLIENT_ID "esp32_client"
#define PUBLISH_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static MQTTClient mqtt_client;
static Network network;
static unsigned char sendbuf[100], readbuf[100];

void mqtt_connect(void) {
    int rc;
    IPStack ipstack = {&network};

    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    MQTTClient_create(&mqtt_client, &ipstack, MQTT_CLIENT_ID, MQTTCLIENT_PERSISTENCE_NONE, NULL);

    if ((rc = MQTTClient_connect(&mqtt_client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        return;
    }

    printf("MQTT connected\n");
}

void mqtt_publish(const char *message) {
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    pubmsg.payload = (void *)message;
    pubmsg.payloadlen = strlen(message);
    pubmsg.qos = 0;
    pubmsg.retained = 0;

    MQTTClient_deliveryToken token;
    MQTTClient_publishMessage(&mqtt_client, MQTT_TOPIC, &pubmsg, &token);
    MQTTClient_waitForCompletion(&mqtt_client, token, 1000L);

    printf("Message '%s' published to topic '%s'\n", message, MQTT_TOPIC);
}

int main(void) {
    printf("Starting MQTT example\n");

    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    sock_udp_t sock;
    sock_udp_create(&sock, &local, &remote, 0);

    network.mqttread = sock_udp_recv;
    network.mqttwrite = sock_udp_send;

    mqtt_connect();

    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        mqtt_publish(MQTT_MESSAGE);
        xtimer_periodic_wakeup(&last_wakeup, PUBLISH_INTERVAL);
    }

    return 0;
}
