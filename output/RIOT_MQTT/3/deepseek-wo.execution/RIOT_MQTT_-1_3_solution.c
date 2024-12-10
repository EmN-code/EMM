
#include <stdio.h>
#include <arpa/inet.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <kernel_defines.h>
#include <paho_mqtt.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0

static void _mqtt_connect(void *client, const char *host, uint16_t port)
{
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, host, &server_addr.sin_addr);

    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        puts("Error creating socket");
        return;
    }

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        puts("Error connecting to server");
        close(sock);
        return;
    }

    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    int rc;
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        close(sock);
        return;
    }

    printf("Connected to MQTT broker at %s:%d\n", host, port);
}

static void _mqtt_publish(void *client, const char *topic, const char *payload)
{
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    pubmsg.payload = (void *)payload;
    pubmsg.payloadlen = strlen(payload);
    pubmsg.qos = MQTT_QOS;
    pubmsg.retained = MQTT_RETAIN;

    MQTTClient_deliveryToken token;
    MQTTClient_publishMessage(client, topic, &pubmsg, &token);
    MQTTClient_waitForCompletion(client, token, 1000L);

    printf("Message '%s' delivered to topic '%s'\n", payload, topic);
}

int main(void)
{
    puts("MQTT client starting");

    MQTTClient client;
    MQTTClient_create(&client, NULL, "RIOT_MQTT_CLIENT", MQTTCLIENT_PERSISTENCE_NONE, NULL);

    _mqtt_connect(client, EMQX_SERVER_IP, EMQX_SERVER_PORT);

    while (1) {
        _mqtt_publish(client, MQTT_TOPIC, MQTT_PAYLOAD);
        xtimer_sleep(5);
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);

    return 0;
}
