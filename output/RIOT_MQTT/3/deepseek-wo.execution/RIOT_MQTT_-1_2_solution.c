
#include <string.h>
#include <unistd.h>
#include "net/sock.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "MQTTClient.h"  // Ensure this is the last header file included

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 10000L

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;
    char buf[128];
    int res;

    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = EMQX_SERVER_PORT;
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error: invalid address");
        return 1;
    }

    // Create a UDP socket
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error: failed to create UDP socket");
        return 1;
    }

    // Initialize MQTT client
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;

    MQTTClient_create(&client, "tcp://47.102.103.1:1883", "RIOT_MQTT_Client", MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        return 1;
    }

    pubmsg.payload = (void*)PAYLOAD;
    pubmsg.payloadlen = strlen(PAYLOAD);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;

    while (1) {
        MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
        printf("Waiting for up to %d seconds for publication of %s\n"
               "on topic %s for client with ClientID: %s\n",
               (int)(TIMEOUT/1000), PAYLOAD, TOPIC, "RIOT_MQTT_Client");
        rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
        printf("Message with delivery token %d delivered\n", token);

        // Sleep for 5 seconds
        sleep(5);
    }

    // Disconnect MQTT client
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);

    // Close the UDP socket
    sock_udp_close(&sock);

    return 0;
}
