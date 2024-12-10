#include <zephyr/net/socket.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>

LOG_MODULE_REGISTER(mqtt_simple, LOG_LEVEL_INF);

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state/topic"
#define PUBLISH_INTERVAL K_SECONDS(5)
#define MESSAGE "work"

static const struct sockaddr_in broker = {
    .sin_family = AF_INET,
    .sin_port = htons(MQTT_BROKER_PORT),
    .sin_addr.s_addr = inet_addr(MQTT_BROKER_ADDR)
};

static int sock;

void connect_to_broker(void) {
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket");
        return;
    }

    if (connect(sock, (struct sockaddr *)&broker, sizeof(broker)) < 0) {
        LOG_ERR("Failed to connect to broker");
        close(sock);
        return;
    }
    LOG_INF("Connected to broker");
}

void publish_state(const char *state) {
    int ret = send(sock, state, strlen(state), 0);
    if (ret < 0) {
        LOG_ERR("Failed to send message");
    } else {
        LOG_INF("Message sent: %s", log_strdup(state));
    }
}

void main(void) {
    LOG_INF("Starting MQTT Client");

    connect_to_broker();

    if (sock < 0) {
        return;
    }

    while (1) {
        publish_state(MESSAGE);
        k_sleep(PUBLISH_INTERVAL);
    }
}