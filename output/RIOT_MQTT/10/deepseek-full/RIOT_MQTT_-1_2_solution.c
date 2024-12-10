
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <time.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN false
#define MQTT_KEEPALIVE 20
#define MQTT_CLEAN_SESSION true
#define MQTT_CLIENT_ID "esp32_client"

#define PUBLISH_INTERVAL 5

int connect_to_broker(const char *broker_ip, int port) {
    int sockfd;
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, broker_ip, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sockfd);
        return -1;
    }

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sockfd);
        return -1;
    }

    return sockfd;
}

void publish_state(MQTTClient *client) {
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    pubmsg.payload = (void *)MQTT_PAYLOAD;
    pubmsg.payloadlen = strlen(MQTT_PAYLOAD);
    pubmsg.qos = MQTT_QOS;
    pubmsg.retained = MQTT_RETAIN;

    MQTTClient_deliveryToken token;
    MQTTClient_publishMessage(client, MQTT_TOPIC, &pubmsg, &token);
    MQTTClient_waitForCompletion(client, token, 1000L);
}

int main() {
    int sockfd;
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    sockfd = connect_to_broker(MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (sockfd < 0) {
        return -1;
    }

    MQTTClient_create(&client, "tcp://47.102.103.1:1883", MQTT_CLIENT_ID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = MQTT_KEEPALIVE;
    conn_opts.cleansession = MQTT_CLEAN_SESSION;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        return -1;
    }

    while (1) {
        publish_state(&client);
        sleep(PUBLISH_INTERVAL);
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    close(sockfd);

    return 0;
}
