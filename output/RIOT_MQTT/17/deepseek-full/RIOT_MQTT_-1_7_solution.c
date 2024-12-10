
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/sock.h>
#include <net/ipv4/addr.h>
#include <net/ipv6/addr.h>
#include <paho_mqtt.h>
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
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <stddef.h>
#include <sys/socket.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 1
#define KEEPALIVE 60

static MQTTClient client;
static Network network;
static evtimer_msg_t timer;
static msg_t msg_queue[16];

static void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    MQTTPublish(&client, TOPIC, &message);
}

static void timer_callback(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.ptr = NULL;
    msg_send(&msg, thread_getpid());
}

static void mqtt_thread(void) {
    msg_t msg;
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = KEEPALIVE;
    options.cleansession = 1;
    options.MQTTVersion = 3;
    options.clientID.cstring = "riot_esp32";

    if (MQTTConnect(&client, &options) != 0) {
        printf("MQTT connection failed\n");
        return;
    }

    printf("MQTT connected\n");

    evtimer_init_msg(&timer);
    evtimer_add_msg(&timer, timex_set(5, 0), timer_callback, NULL);

    while (1) {
        msg_receive(&msg);
        publish_state();
        evtimer_add_msg(&timer, timex_set(5, 0), timer_callback, NULL);
    }
}

int main(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    sock_udp_t sock;
    sock_udp_create(&sock, &local, &remote, 0);

    network.sock = sock;
    network.mqttread = sock_udp_recv;
    network.mqttwrite = sock_udp_send;

    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);

    thread_create(NULL, 1024, THREAD_PRIORITY_MAIN - 1, 0, mqtt_thread, NULL, "mqtt");

    return 0;
}
