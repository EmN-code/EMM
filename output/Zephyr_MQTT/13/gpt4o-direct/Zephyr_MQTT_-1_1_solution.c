
#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <logging/log.h>
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL

#define STACK_SIZE 1024
#define PRIORITY 7

LOG_MODULE_REGISTER(mqtt_client);

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_tx_buf tx_buf;
static struct mqtt_rx_buf rx_buf;

static int connect_mqtt(void) {
    int ret;
    struct sockaddr_in broker_addr;

    memset(&broker_addr, 0, sizeof(broker_addr));
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.sin_addr);

    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket: %d", errno);
        return -1;
    }

    ret = connect(sock, (struct sockaddr *)&broker_addr, sizeof(broker_addr));
    if (ret < 0) {
        LOG_ERR("Failed to connect to broker: %d", errno);
        close(sock);
        return -1;
    }

    mqtt_client_init(&client_ctx);

    client_ctx.broker = &broker_addr;
    client_ctx.broker_len = sizeof(broker_addr);
    client_ctx.client_id.utf8 = (uint8_t *)MQTT_CLIENT_ID;
    client_ctx.client_id.size = strlen(MQTT_CLIENT_ID);
    client_ctx.password = (uint8_t *)MQTT_PASSWORD;
    client_ctx.password_len = MQTT_PASSWORD ? strlen(MQTT_PASSWORD) : 0;
    client_ctx.user_name = (uint8_t *)MQTT_USERNAME;
    client_ctx.user_name_len = MQTT_USERNAME ? strlen(MQTT_USERNAME) : 0;
    client_ctx.protocol_version = MQTT_VERSION_3_1_1;
    client_ctx.clean_session = 1;
    client_ctx.keepalive = 60;
    client_ctx.socket = sock;
    client_ctx.evt_cb = NULL;

    ret = mqtt_connect(&client_ctx);
    if (ret < 0) {
        LOG_ERR("Failed to connect to MQTT broker: %d", ret);
        close(sock);
        return -1;
    }

    return 0;
}

static int publish_state(const char *state) {
    int ret;
    struct mqtt_publish_param param;

    param.message.topic.utf8 = (uint8_t *)MQTT_TOPIC;
    param.message.topic.size = strlen(MQTT_TOPIC);
    param.message.payload.utf8 = (uint8_t *)state;
    param.message.payload.size = strlen(state);
    param.message_id = 0;
    param.dup_flag = 0;
    param.retain_flag = 0;
    param.qos = MQTT_QOS_0_AT_MOST_ONCE;

    ret = mqtt_publish(&client_ctx, &param);
    if (ret < 0) {
        LOG_ERR("Failed to publish message: %d", ret);
        return -1;
    }

    return 0;
}

void main(void) {
    if (connect_mqtt() < 0) {
        LOG_ERR("Failed to connect to MQTT broker");
        return;
    }

    while (1) {
        if (publish_state("work") < 0) {
            LOG_ERR("Failed to publish 'work' state");
        }
        k_sleep(K_SECONDS(5));

        if (publish_state("not work") < 0) {
            LOG_ERR("Failed to publish 'not work' state");
        }
        k_sleep(K_SECONDS(5));
    }
}
