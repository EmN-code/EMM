
#include <thread.h>
#include <net/sock.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN false
#define MQTT_KEEP_ALIVE 20

static void mqtt_publish(MQTTClient client, const char *topic, const char *payload)
{
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

    pubmsg.payload = (void *)payload;
    pubmsg.payloadlen = strlen(payload);
    pubmsg.qos = MQTT_QOS;
    pubmsg.retained = MQTT_RETAIN;

    MQTTClient_publishMessage(client, topic, &pubmsg, &token);
    MQTTClient_waitForCompletion(client, token, 1000L);
}

int main(void)
{
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;
    sock_udp_t sock;
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    // Initialize the remote endpoint for the MQTT broker
    remote.family = AF_INET;
    remote.port = HTONS(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    // Create a UDP socket
    sock_udp_create(&sock, &local, &remote, 0);

    // Initialize the MQTT client
    MQTTClient_create(&client, NULL, NULL, MQTTCLIENT_PERSISTENCE_NONE);
    conn_opts.keepAliveInterval = MQTT_KEEP_ALIVE;
    conn_opts.cleansession = 1;

    // Connect to the MQTT broker
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
        return -1;
    }

    // Publish the state every 5 seconds
    while (1) {
        mqtt_publish(client, MQTT_TOPIC, MQTT_PAYLOAD);
        xtimer_sleep(5);
    }

    // Disconnect and destroy the MQTT client
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);

    return 0;
}
