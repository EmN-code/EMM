
#include <net/sock.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <net/gcoap.h>
#include <MQTTClient.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work or not work"
#define PAYLOAD_LEN strlen(PAYLOAD)

static void _mqtt_publish(void)
{
    int sock;
    struct sockaddr_in server_addr;
    char buf[1024];
    int res;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        puts("Error creating socket");
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    res = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (res < 0) {
        puts("Error connecting to server");
        close(sock);
        return;
    }

    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

    MQTTClient_create(&client, "tcp://47.102.103.1:1883", "RIOT_MQTT_CLIENT", MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((res = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", res);
        return;
    }

    pubmsg.payload = (void *)PAYLOAD;
    pubmsg.payloadlen = PAYLOAD_LEN;
    pubmsg.qos = 0;
    pubmsg.retained = 0;
    MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
    printf("Waiting for up to %d seconds for publication of %s\n"
           "on topic %s for client with ClientID: %s\n",
           (int)(1000 / 1000), PAYLOAD, TOPIC, "RIOT_MQTT_CLIENT");
    res = MQTTClient_waitForCompletion(client, token, 1000);
    printf("Message with delivery token %d delivered\n", token);

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);

    close(sock);
}

int main(void)
{
    while (1) {
        _mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
