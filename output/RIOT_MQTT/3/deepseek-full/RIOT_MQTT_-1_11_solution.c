
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <arpa/inet.h>
#include <net/sock.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <mutex.h>
#include <timex.h>
#include <kernel_defines.h>
#include <msg.h>
#include <net/emcute.h>
#include <ztimer.h>
#include <xtimer.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <paho_mqtt.h>
#include <time_units.h>
#include <stddef.h>
#include <thread.h>
#include <mma8x5x_regs.h>
#include <dht_params.h>
#include <dht.h>
#include <mma8x5x.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "riot_esp32"
#define TOPIC "state"
#define PAYLOAD "work"

static sock_udp_t sock;
static sock_udp_ep_t server_ep = SOCK_IPV4_EP_ANY;

void network_init(void) {
    server_ep.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &server_ep.addr.ipv4);
    sock_udp_create(&sock, NULL, &server_ep, 0);
}

void mqtt_publish(void) {
    static char payload[] = PAYLOAD;
    MQTTMessage message;
    message.qos = 0;
    message.retained = 0;
    message.dup = 0;
    message.payload = payload;
    message.payloadlen = sizeof(payload) - 1;

    MQTTPublish(&client, TOPIC, &message);
}

void mqtt_connect(void) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = 20;
    options.cleansession = 1;
    options.clientID.cstring = CLIENT_ID;

    if (MQTTConnect(&client, &options) != 0) {
        printf("MQTT connection failed\n");
        return;
    }
    printf("MQTT connected\n");
}

void mqtt_client_init(void) {
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
}

int main(void) {
    network_init();
    mqtt_client_init();
    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
