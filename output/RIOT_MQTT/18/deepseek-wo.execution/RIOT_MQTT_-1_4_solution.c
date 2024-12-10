
#include <net/ipv4/addr.h>
#include <net/sock/tcp.h>
#include <net/sock/util.h>
#include <timex.h>
#include <thread.h>
#include <msg.h>
#include <ztimer.h>

#define MQTT_SERVER_IP "47.102.103.1"
#define MQTT_SERVER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_COMMAND_TIMEOUT 30000

typedef struct {
    sock_tcp_t socket;
} Network;

typedef struct {
    Network* network;
    unsigned char* sendbuf;
    unsigned char* readbuf;
    size_t sendbuf_size;
    size_t readbuf_size;
} MQTTClient;

int NetworkConnect(Network *n, char* address_ip, int port_number) {
    ipv4_addr_t addr;
    if (inet_pton(AF_INET, address_ip, &addr) <= 0) {
        return -1;
    }
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = port_number, .addr = { .ipv4 = addr } };
    return sock_tcp_connect(&n->socket, &remote, &local, SOCK_FLAGS_REUSE_EP);
}

void MQTTClientInit(MQTTClient* client, Network* network, unsigned int command_timeout_ms,
                    unsigned char* sendbuf, size_t sendbuf_size, unsigned char* readbuf, size_t readbuf_size) {
    client->network = network;
    client->sendbuf = sendbuf;
    client->readbuf = readbuf;
    client->sendbuf_size = sendbuf_size;
    client->readbuf_size = readbuf_size;
}

int MQTTPublish(MQTTClient* client, const char* topic, const char* payload) {
    // Implement MQTT publish logic here
    return 0;
}

void mqtt_thread(void *arg) {
    MQTTClient client;
    Network network;
    unsigned char sendbuf[100];
    unsigned char readbuf[100];

    MQTTClientInit(&client, &network, MQTT_COMMAND_TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    NetworkConnect(&network, MQTT_SERVER_IP, MQTT_SERVER_PORT);

    while (1) {
        MQTTPublish(&client, MQTT_TOPIC, MQTT_PAYLOAD);
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
}

int main(void) {
    thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1, 0, mqtt_thread, NULL, "mqtt_thread");
    return 0;
}
