
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work or not work"
#define MQTT_KEEPALIVE 60
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_WILL_FLAG 0
#define MQTT_CLEAN_SESSION 1
#define MQTT_CLIENT_ID "esp32_client"

typedef struct {
    int sockfd;
    struct sockaddr_in broker_addr;
} mqtt_client_t;

int mqtt_connect(mqtt_client_t *client) {
    client->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (client->sockfd < 0) {
        return -1;
    }

    client->broker_addr.sin_family = AF_INET;
    client->broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &client->broker_addr.sin_addr) <= 0) {
        return -1;
    }

    if (connect(client->sockfd, (struct sockaddr *)&client->broker_addr, sizeof(client->broker_addr)) < 0) {
        return -1;
    }

    return 0;
}

void mqtt_publish(mqtt_client_t *client) {
    char message[100];
    snprintf(message, sizeof(message), "PUBLISH %s %s", MQTT_TOPIC, MQTT_MESSAGE);
    send(client->sockfd, message, strlen(message), 0);
}

int main() {
    mqtt_client_t client;

    if (mqtt_connect(&client) < 0) {
        return -1;
    }

    while (1) {
        mqtt_publish(&client);
        sleep(5);
    }

    close(client.sockfd);
    return 0;
}
