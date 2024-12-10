
#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <logging/log.h>
#include <string.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_CLIENT_ID "esp32_client"

LOG_MODULE_REGISTER(mqtt_example);

static int mqtt_connect(int sock)
{
    struct mqtt_client client;
    struct sockaddr_in broker;
    struct mqtt_connect_client_info_t ci;
    int ret;

    memset(&client, 0, sizeof(client));
    memset(&broker, 0, sizeof(broker));
    memset(&ci, 0, sizeof(ci));

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    client.broker = &broker;
    client.broker_len = sizeof(broker);
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.transport.tcp.sock = sock;

    ci.client_id = MQTT_CLIENT_ID;
    ci.keepalive = 60;

    ret = mqtt_connect(&client, &ci);
    if (ret) {
        LOG_ERR("MQTT connect failed %d", ret);
        return ret;
    }

    return 0;
}

static int mqtt_publish(int sock, const char *topic, const char *payload)
{
    struct mqtt_client client;
    struct mqtt_publish_client_info_t pi;
    int ret;

    memset(&client, 0, sizeof(client));
    memset(&pi, 0, sizeof(pi));

    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.transport.tcp.sock = sock;

    pi.topic = topic;
    pi.payload = payload;
    pi.payload_len = strlen(payload);

    ret = mqtt_publish(&client, &pi);
    if (ret) {
        LOG_ERR("MQTT publish failed %d", ret);
        return ret;
    }

    return 0;
}

void main(void)
{
    int sock;
    int ret;

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket %d", sock);
        return;
    }

    struct sockaddr_in broker;
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    ret = connect(sock, (struct sockaddr *)&broker, sizeof(broker));
    if (ret < 0) {
        LOG_ERR("Failed to connect to broker %d", ret);
        close(sock);
        return;
    }

    ret = mqtt_connect(sock);
    if (ret) {
        close(sock);
        return;
    }

    while (1) {
        const char *state = (k_uptime_get() % 10000 < 5000) ? "work" : "not work";
        ret = mqtt_publish(sock, MQTT_TOPIC, state);
        if (ret) {
            LOG_ERR("Failed to publish state %d", ret);
            break;
        }
        k_sleep(K_SECONDS(5));
    }

    close(sock);
}
