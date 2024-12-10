#include <zephyr/net/mqtt.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>

LOG_MODULE_REGISTER(mqtt_demo, LOG_LEVEL_INF);

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883

#define STATE_WORK "work"
#define STATE_NOT_WORK "not work"

#define STACK_SIZE 1024
#define PRIORITY 5

static struct mqtt_client client;
static struct sockaddr_in broker;
static struct mqtt_pubmsg pub_msg;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[256];

void main(void) {
    int ret;

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    struct mqtt_utf8 broker_username = {.utf8 = NULL, .size = 0 };
    struct mqtt_utf8 broker_password = {.utf8 = NULL, .size = 0 };
    struct mqtt_utf8 client_id = {.utf8 = (uint8_t*)"zephyr_client", .size = strlen("zephyr_client")};

    struct mqtt_connect_data conn_data = {
        .broker = (struct sockaddr*)&broker,
        .broker_id = client_id,
        .user_name = &broker_username,
        .password = &broker_password,
        .client_id = &client_id,
        .protocol_version = MQTT_VERSION_3_1_1,
        .keep_alive = 60,
        .rx_buf = rx_buffer,
        .rx_buf_size = sizeof(rx_buffer),
        .tx_buf = tx_buffer,
        .tx_buf_size = sizeof(tx_buffer),
    };

    mqtt_client_init(&client, &conn_data);

    while (1) {
        pub_msg.topic = "state";
        pub_msg.payload.data = (uint8_t*)STATE_WORK;
        pub_msg.payload.len = strlen(STATE_WORK);

        ret = mqtt_publish(&client, &pub_msg);
        if (ret) {
            LOG_ERR("Failed to publish message, error: %d", ret);
        }

        k_sleep(K_SECONDS(5));
    }
}
