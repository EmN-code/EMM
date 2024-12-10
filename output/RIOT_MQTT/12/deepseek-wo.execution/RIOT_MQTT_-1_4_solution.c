
#include <net/sock/tcp.h>
#include <arpa/inet.h>
#include <xtimer.h>
#include <esp_system.h>
#include <MQTTClient.h>
#include <ztimer.h>
#include <net/sock/util.h>
#include <net/sock.h>
#include <timex.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 1000L

static Network network;
static MQTTClient client;

int mqtt_read(Network* n, unsigned char* buffer, int len, int timeout) {
    (void)timeout;
    return sock_tcp_read(n->sock, buffer, len);
}

int mqtt_write(Network* n, unsigned char* buffer, int len, int timeout) {
    (void)timeout;
    return sock_tcp_write(n->sock, buffer, len);
}

void network_init(void) {
    network.mqttread = mqtt_read;
    network.mqttwrite = mqtt_write;
}

void connect_to_broker(void) {
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote;
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    sock_tcp_create(&network.sock, &local, &remote, 0);
}

void publish_message(void) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    MQTTPublish(&client, TOPIC, &message);
}

int main(void) {
    network_init();
    connect_to_broker();

    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    MQTTClient_create(&client, &network, 1000, NULL, MQTTCLIENT_PERSISTENCE_NONE);
    MQTTConnect(&client, &conn_opts);

    while (1) {
        publish_message();
        xtimer_sleep(5);
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return 0;
}
