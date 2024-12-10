#include <zephyr/net/mqtt.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/socket.h>

LOG_MODULE_REGISTER(mqtt_pub_example, LOG_LEVEL_INF);

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "esp32_client"

static struct mqtt_client client;
static struct sockaddr_storage broker;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static uint8_t payload[] = "work";

void broker_init(void) {
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(BROKER_PORT);
    zsock_inet_pton(AF_INET, BROKER_ADDR, &broker4->sin_addr);
}

void mqtt_evt_handler(struct mqtt_client *const c, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            if (evt->result != 0) {
                LOG_ERR("MQTT connect failed (%d)", evt->result);
                return;
            }
            LOG_INF("MQTT client connected!");
            break;
        case MQTT_EVT_PUBACK:
            if (evt->result != 0) {
                LOG_ERR("MQTT PUBACK error (%d)", evt->result);
                return;
            }
            LOG_INF("PUBACK packet id: %u", evt->param.puback.message_id);
            break;
        case MQTT_EVT_DISCONNECT:
            LOG_INF("MQTT client disconnected (%d)", evt->result);
            break;
        default:
            LOG_INF("MQTT event received: %d", evt->type);
            break;
    }
}

void mqtt_setup(void) {
    mqtt_client_init(&client);

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = (uint8_t *)CLIENT_ID;
    client.client_id.size = strlen(CLIENT_ID);
    client.protocol_version = MQTT_VERSION_3_1_1;

    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    struct mqtt_utf8 connect_username = {.utf8 = NULL, .size = 0};
    struct mqtt_utf8 connect_password = {.utf8 = NULL, .size = 0};

    struct mqtt_sec_config tls_config = {
        .peer_verify = 0,
        .cipher_list = NULL,
        .sec_tag_list = NULL,
        .sec_tag_count = 0,
        .hostname = NULL,
    };

    struct mqtt_connect_client_id client_id = {
        .utf8 = (uint8_t *)CLIENT_ID,
        .size = strlen(CLIENT_ID)
    };

    struct mqtt_connect_msg conn_msg = {
        .client_id = client_id,
        .clean_session = 1,
        .will_flag = 0,
        .will_topic = NULL,
        .will_message = NULL,
        .user_name = connect_username,
        .password = connect_password,
        .keep_alive = 60,
        .will_retain = 0,
        .will_qos = 0,
    };

    int rc = mqtt_connect(&client, &conn_msg);
    if (rc != 0) {
        LOG_ERR("mqtt_connect error: %d", rc);
        return;
    }

    LOG_INF("Connecting to MQTT broker...");
}

void publish_state(void) {
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;
    param.message.topic.topic.utf8 = (const uint8_t *)"state";
    param.message.topic.topic.size = strlen("state");
    param.message.payload.data = payload;
    param.message.payload.len = sizeof(payload) - 1;
    param.message_id = 1;
    param.dup_flag = 0;
    param.retain_flag = 0;

    int rc = mqtt_publish(&client, &param);
    if (rc != 0) {
        LOG_ERR("mqtt_publish error: %d", rc);
    }
}

void main(void) {
    LOG_INF("MQTT simple publisher example");

    broker_init();
    mqtt_setup();

    while (1) {
        publish_state();
        k_sleep(K_SECONDS(5));
    }
}