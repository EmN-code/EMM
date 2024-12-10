
#include <stdlib.h>
#include <stdio.h>
#include <net/ipv4/addr.h>
#include <event/thread.h>
#include <esp_system.h>
#include <thread.h>
#include <mutex.h>
#include <mma8x5x_params.h>
#include <dht_params.h>
#include <esp_wifi.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include <net/sock.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stddef.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <paho_mqtt.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 1
#define KEEP_ALIVE 20

static void publish_state(void) {
    static int state = 0;
    const char *payload = state ? "work" : "not work";
    state = !state;

    MQTTClient client;
    Network network;
    int rc;

    NetworkInit(&network);
    MQTTClient_create(&client, &network, "MQTTClient", MQTTCLIENT_PERSISTENCE_NONE, NULL);

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = KEEP_ALIVE;
    options.cleansession = 1;

    if ((rc = MQTTConnect(&client, &options)) != 0) {
        printf("Failed to connect, return code %d\n", rc);
        return;
    }

    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    pubmsg.payload = (void *)payload;
    pubmsg.payloadlen = strlen(payload);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;

    MQTTClient_deliveryToken token;
    MQTTClient_publishMessage(&client, TOPIC, &pubmsg, &token);
    MQTTClient_waitForCompletion(&client, token, 1000L);

    printf("Message '%s' delivered\n", payload);

    MQTTClient_disconnect(&client, 10000);
    MQTTClient_destroy(&client);
}

int main(void) {
    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return 0;
}
