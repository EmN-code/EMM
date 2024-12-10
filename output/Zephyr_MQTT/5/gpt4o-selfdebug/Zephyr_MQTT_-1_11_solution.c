#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/logging/log.h>
#include <string.h>

LOG_MODULE_REGISTER(mqtt_publisher, LOG_LEVEL_DBG);

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_PAYLOAD_MSG "work or not work"
#define MQTT_TOPIC "state"

static struct mqtt_client client;
static struct sockaddr_storage broker;
static struct zsock_pollfd fds[1];
static bool connected = false;

static void prepare_fds(void)
{
    fds[0].fd = client.transport.tcp.sock;
    fds[0].events = ZSOCK_POLLIN;
}

static void broker_init(void)
{
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(BROKER_PORT);
    zsock_inet_pton(AF_INET, BROKER_ADDR, &broker4->sin_addr);
}

static void connect_to_broker(void)
{
    struct mqtt_sec_config *tls_config = NULL;
    struct mqtt_client_init_param init_params = {
        .broker = &broker,
        .evt_cb = NULL,
        .client_id.utf8 = "zephyr_client",
        .client_id.size = strlen("zephyr_client"),
        .password = NULL,
        .user_name = NULL,
        .protocol_version = MQTT_VERSION_3_1_1,
        .transport.type = MQTT_TRANSPORT_NON_SECURE,
    };
    mqtt_client_init(&client, &init_params);
    int ret = mqtt_connect(&client, &broker, tls_config);
    if (ret) {
        LOG_ERR("Failed to connect to broker: %d", ret);
        return;
    }
    prepare_fds();
    connected = true;
}

static void publish_state(void)
{
    struct mqtt_publish_param param;
    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = MQTT_TOPIC;
    param.message.topic.topic.size = strlen(MQTT_TOPIC);
    param.message.payload.data = MQTT_PAYLOAD_MSG;
    param.message.payload.len = strlen(MQTT_PAYLOAD_MSG);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;

    int ret = mqtt_publish(&client, &param);
    if (ret) {
        LOG_ERR("Failed to publish message: %d", ret);
    } else {
        LOG_INF("Message published successfully");
    }
}

void main(void)
{
    broker_init();
    connect_to_broker();
    while (1) {
        if (connected) {
            publish_state();
        }
        k_sleep(K_SECONDS(5));
    }
}