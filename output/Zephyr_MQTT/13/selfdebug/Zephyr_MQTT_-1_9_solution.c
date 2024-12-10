
#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <logging/log.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"

LOG_MODULE_REGISTER(main);

static int mqtt_connect(struct mqtt_client *client)
{
    int rc;
    struct sockaddr_storage broker;

    broker.sa_family = AF_INET;
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_port = htons(MQTT_BROKER_PORT);

    rc = net_addr_pton(AF_INET, MQTT_BROKER_IP, &broker4->sin_addr);
    if (rc < 0) {
        LOG_ERR("Invalid IP address %s", MQTT_BROKER_IP);
        return rc;
    }

    rc = mqtt_connect_tcp(client, (struct sockaddr *)&broker);
    if (rc < 0) {
        LOG_ERR("Failed to connect to MQTT broker: %d", rc);
        return rc;
    }

    return 0;
}

static void mqtt_publish_state(struct mqtt_client *client)
{
    static bool state = false;
    const char *state_str = state ? "work" : "not work";
    state = !state;

    int rc = mqtt_publish(client, MQTT_TOPIC, state_str, strlen(state_str), MQTT_QOS_1_AT_LEAST_ONCE, false);
    if (rc < 0) {
        LOG_ERR("Failed to publish state: %d", rc);
    } else {
        LOG_INF("Published state: %s", state_str);
    }
}

void main(void)
{
    struct mqtt_client client;
    struct mqtt_client_config config;
    struct mqtt_client_buf buf;

    memset(&client, 0, sizeof(client));
    memset(&config, 0, sizeof(config));
    memset(&buf, 0, sizeof(buf));

    config.client_id = MQTT_CLIENT_ID;
    config.user_name = NULL;
    config.password = NULL;
    config.keep_alive = 60;

    mqtt_client_init(&client);
    mqtt_client_config_init(&config);
    mqtt_client_buf_init(&buf);

    int rc = mqtt_connect(&client);
    if (rc < 0) {
        LOG_ERR("MQTT connection failed: %d", rc);
        return;
    }

    while (1) {
        mqtt_publish_state(&client);
        k_sleep(K_SECONDS(5));
    }
}
