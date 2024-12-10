
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/ipv4/addr.h>
#include <net/af.h>
#include <net/sock/util.h>
#include <MQTTClient.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static Network network;
static MQTTClient client;

static int mqtt_read(Network *network, unsigned char *buffer, int len, int timeout) {
    (void)timeout;
    return sock_tcp_recv(network->sock, buffer, len, SOCK_NO_TIMEOUT);
}

static int mqtt_write(Network *network, unsigned char *buffer, int len, int timeout) {
    (void)timeout;
    return sock_tcp_send(network->sock, buffer, len, 0);
}

static void connect_to_broker(void) {
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote;
    remote.family = AF_INET;
    remote.port = BROKER_PORT;
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    sock_tcp_create(&network.sock, &local, &remote, 0);
    network.mqttread = mqtt_read;
    network.mqttwrite = mqtt_write;

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "RIOT_ESP32";
    data.keepAliveInterval = 60;
    data.cleansession = 1;

    int rc = MQTTConnect(&client, &data);
    if (rc != 0) {
        printf("MQTT connect failed\n");
        return;
    }
    printf("MQTT connected\n");
}

static void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = PAYLOAD_LEN;

    int rc = MQTTPublish(&client, TOPIC, &message);
    if (rc != 0) {
        printf("MQTT publish failed\n");
        return;
    }
    printf("MQTT published\n");
}

int main(void) {
    connect_to_broker();

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
