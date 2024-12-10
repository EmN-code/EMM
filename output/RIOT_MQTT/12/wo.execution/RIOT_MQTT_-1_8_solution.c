#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mutex.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <unistd.h>
#include <time_units.h>
#include <timex.h>
#include <thread.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <paho_mqtt.h>
#include <kernel_defines.h>
#include <ztimer.h>
#include <dht_params.h>
#include <dht.h>
#include <architecture.h>
#include <arpa/inet.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <MQTTClient.h>

#define SERVER_IP          "47.102.103.1"
#define SERVER_PORT        1883
#define MQTT_CLIENT_ID     "RIOT_ESP32"
#define MQTT_TOPIC         "device/status"
#define MQTT_MSG_WORKING   "work"
#define MQTT_MSG_NOT_WORK  "not work"
#define PUB_INTERVAL       (5U)        // 5 seconds

static int network_init(Network* net) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    return sock_udp_create(&net->sock, &local, &remote, 0);
}

int main(void) {
    static msg_t queue[8];
    msg_init_queue(queue, 8);

    Network n;
    MQTTClient client;
    unsigned char sendbuf[100];
    unsigned char readbuf[100];
    MQTTClientInit(&client, &n, PUB_INTERVAL * 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    if (network_init(&n) < 0) {
        puts("Failed to initialize network");
        return -1;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 4;
    connectData.clientID.cstring = MQTT_CLIENT_ID;

    if (MQTTConnect(&client, &connectData) < 0) {
        puts("Failed to connect to MQTT broker");
        return -1;
    }

    while (1) {
        MQTTMessage message;
        message.qos = 0;
        message.retained = 0;
        message.payload = (void*)MQTT_MSG_WORKING;
        message.payloadlen = strlen(MQTT_MSG_WORKING);
        MQTTPublish(&client, MQTT_TOPIC, &message);

        ztimer_sleep(ZTIMER_SEC, PUB_INTERVAL);
    }

    return 0;
}