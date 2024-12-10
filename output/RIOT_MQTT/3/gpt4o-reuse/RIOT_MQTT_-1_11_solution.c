
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "net/ipv4/addr.h"
#include "net/sock/tcp.h"
#include "xtimer.h"
#include "MQTTClient.h"

#define ADDRESS     "47.102.103.1"
#define PORT        1883
#define CLIENTID    "mqtt_esp32"
#define TOPIC       "state"
#define PAYLOAD     "work"
#define QOS         1
#define TIMEOUT     10000L
#define INTERVAL    5000000L  // 5 seconds in microseconds

#define USERNAME    "mqtt_user"
#define PASSWORD    "123456" 

static MQTTClient client;
static MQTTClient_deliveryToken deliveredtoken;
static MQTTClient_message pubmsg = MQTTClient_message_initializer;
static MQTTClient_deliveryToken token;
static MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: %.*s\n", message->payloadlen, (char*)message->payload);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    
    return 1;
}

void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

void public_mqttMsg()
{
    int rc;
 
    pubmsg.payload = PAYLOAD;
    pubmsg.payloadlen = (int)strlen(PAYLOAD);
    pubmsg.qos = QOS;
    pubmsg.retained = 1;
    deliveredtoken = 0;
    if ((rc = MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to publish message, return code %d\n", rc);
        rc = EXIT_FAILURE;
    }
    else
    {
        printf("Waiting for publication of %s\n"
            "on topic %s for client with ClientID: %s\n",
            PAYLOAD, TOPIC, CLIENTID);
        while (deliveredtoken != token)
        {
            usleep(10000000L);
        }
    }
}

int main(void)
{
    int rc;
    sock_tcp_ep_t remote;
    sock_tcp_t sock;

    remote.family = AF_INET;
    remote.port = PORT;
    inet_pton(AF_INET, ADDRESS, &remote.addr.ipv4);

    if ((rc = sock_tcp_create(&sock, NULL, &remote, 0)) < 0) {
        printf("Failed to create socket, return code %d\n", rc);
        return EXIT_FAILURE;
    }

    if ((rc = MQTTClient_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to create client, return code %d\n", rc);
        rc = EXIT_FAILURE;
        goto close_socket;
    }

    if ((rc = MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to set callbacks, return code %d\n", rc);
        rc = EXIT_FAILURE;
        goto destroy_client;
    }

    conn_opts.keepAliveInterval = 60;
    conn_opts.cleansession = 0;
    conn_opts.username = USERNAME;   //用户名
    conn_opts.password = PASSWORD;   //密码
    
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        rc = EXIT_FAILURE;
        goto destroy_client;
    }

    while (1) {
        public_mqttMsg();
        xtimer_usleep(INTERVAL);
    }

    if ((rc = MQTTClient_disconnect(client, 10000)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to disconnect, return code %d\n", rc);
        rc = EXIT_FAILURE;
    }

destroy_client:
    MQTTClient_destroy(&client);

close_socket:
    sock_tcp_close(&sock);

    return rc;
}
