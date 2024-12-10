
#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <logging/log.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work or not work"

LOG_MODULE_REGISTER(main);

void mqtt_evt_handler(struct mqtt_client *const client,
                      const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        LOG_INF("Connected to MQTT broker");
        break;
    case MQTT_EVT_DISCONNECT:
        LOG_INF("Disconnected from MQTT broker");
        break;
    default:
        break;
    }
}

void publish_state(struct mqtt_client *client)
{
    struct mqtt_publish_param param;
    static uint8_t payload[] = MQTT_MESSAGE;

    param.message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;
    param.message.topic.topic.utf8 = MQTT_TOPIC;
    param.message.topic.topic.size = strlen(MQTT_TOPIC);
    param.message.payload.data = payload;
    param.message.payload.len = strlen(payload);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;

    int ret = mqtt_publish(client, &param);
    if (ret) {
        LOG_ERR("Failed to publish message, error: %d", ret);
    } else {
        LOG_INF("Message published");
    }
}

void main(void)
{
    struct mqtt_client client;
    struct sockaddr_in broker;
    struct mqtt_client_config config;
    struct mqtt_client_init_options init_options;

    memset(&client, 0, sizeof(client));
    memset(&broker, 0, sizeof(broker));
    memset(&config, 0, sizeof(config));
    memset(&init_options, 0, sizeof(init_options));

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    config.broker = &broker;
    config.client_id.utf8 = "zephyr_client";
    config.client_id.size = strlen(config.client_id.utf8);
    config.keepalive = 60;
    config.clean_session = 1;

    init_options.rx_buf = k_malloc(CONFIG_MQTT_MESSAGE_BUFFER_SIZE);
    init_options.rx_buf_size = CONFIG_MQTT_MESSAGE_BUFFER_SIZE;
    init_options.tx_buf = k_malloc(CONFIG_MQTT_MESSAGE_BUFFER_SIZE);
    init_options.tx_buf_size = CONFIG_MQTT_MESSAGE_BUFFER_SIZE;

    mqtt_client_init(&client, &config, &init_options);
    client.evt_cb = mqtt_evt_handler;

    int ret = mqtt_connect(&client);
    if (ret) {
        LOG_ERR("Failed to connect to MQTT broker, error: %d", ret);
        return;
    }

    while (1) {
        k_sleep(K_SECONDS(5));
        publish_state(&client);
    }
}
