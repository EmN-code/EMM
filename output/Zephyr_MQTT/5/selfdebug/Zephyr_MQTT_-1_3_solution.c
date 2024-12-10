
#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <logging/log.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "zephyr_client"

LOG_MODULE_REGISTER(main);

static int mqtt_connect(int sock)
{
    struct mqtt_client client;
    struct mqtt_client_config config;
    struct mqtt_client_connect_msg connect_msg;

    memset(&client, 0, sizeof(client));
    memset(&config, 0, sizeof(config));
    memset(&connect_msg, 0, sizeof(connect_msg));

    config.client_id = MQTT_CLIENT_ID;
    config.broker = MQTT_BROKER_IP;
    config.port = MQTT_BROKER_PORT;

    connect_msg.keepalive = 60;
    connect_msg.clean_session = 1;

    mqtt_client_init(&client);
    mqtt_client_set_config(&client, &config);

    return mqtt_connect(&client, &connect_msg);
}

static int mqtt_publish(int sock, const char *topic, const char *payload)
{
    struct mqtt_client client;
    struct mqtt_client_publish_msg publish_msg;

    memset(&client, 0, sizeof(client));
    memset(&publish_msg, 0, sizeof(publish_msg));

    publish_msg.topic = topic;
    publish_msg.payload = payload;
    publish_msg.payload_len = strlen(payload);
    publish_msg.qos = MQTT_QOS_0_AT_MOST_ONCE;

    return mqtt_publish(&client, &publish_msg);
}

void main(void)
{
    int sock;
    int ret;
    const char *state = "work";

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket");
        return;
    }

    struct sockaddr_in broker_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(MQTT_BROKER_PORT),
        .sin_addr.s_addr = inet_addr(MQTT_BROKER_IP),
    };

    ret = connect(sock, (struct sockaddr *)&broker_addr, sizeof(broker_addr));
    if (ret < 0) {
        LOG_ERR("Failed to connect to MQTT broker");
        close(sock);
        return;
    }

    ret = mqtt_connect(sock);
    if (ret < 0) {
        LOG_ERR("Failed to connect to MQTT broker");
        close(sock);
        return;
    }

    while (1) {
        ret = mqtt_publish(sock, MQTT_TOPIC, state);
        if (ret < 0) {
            LOG_ERR("Failed to publish message");
        } else {
            LOG_INF("Published state: %s", state);
        }

        k_sleep(K_SECONDS(5));
    }
}
