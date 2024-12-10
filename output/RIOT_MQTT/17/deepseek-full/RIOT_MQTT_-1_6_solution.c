
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <net/ipv6/addr.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <timex.h>
#include <stdatomic.h>
#include <unistd.h>
#include <time_units.h>
#include <stdbool.h>
#include <MQTTClient.h>
#include <msg.h>
#include <thread.h>
#include <ztimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 10000L

static Network network;
static MQTTClient client;
static sock_udp_t sock;
static sock_udp_ep_t local_ep = SOCK_IPV4_EP_ANY;
static sock_udp_ep_t remote_ep;

void connect_to_broker(void) {
    int rc;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = 20;
    options.cleansession = 1;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";

    rc = NetworkConnect(&network, SERVER_IP, SERVER_PORT);
    if (rc != 0) {
        printf("Failed to connect to network\n");
        return;
    }

    rc = MQTTConnect(&client, &options);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }

    printf("Connected to MQTT broker\n");
}

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    int rc = MQTTPublish(&client, TOPIC, &message);
    if (rc != 0) {
        printf("Failed to publish message\n");
    } else {
        printf("Message published\n");
    }
}

void periodic_publish(void) {
    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
}

int main(void) {
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    local_ep.port = 12345;
    remote_ep.port = SERVER_PORT;
    remote_ep.family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &remote_ep.addr.ipv4);

    sock_udp_create(&sock, &local_ep, &remote_ep, 0);

    NetworkInit(&network);
    MQTTClientInit(&client, &network, TIMEOUT, NULL, 0, NULL, 0);

    connect_to_broker();

    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  (thread_task_func_t)periodic_publish, NULL, "periodic_publish");

    return 0;
}
