#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devices/gpio.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/reboot.h>

LOG_MODULE_REGISTER(mqtt_simple, LOG_LEVEL_INF);

#define MQTT_BROKER_ADDR  "47.102.103.1"
#define MQTT_BROKER_PORT  1883
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_storage broker;
static struct mqtt_utf8 publish_topic = {.utf8 = "state", .size = 5};
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static uint8_t payload[] = "work or not work";

static void connect_to_broker(void) {
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker4->sin_addr);

    client.broker = &broker;

    mqtt_connect(&client);
}

static void publish_state(struct k_work *work) {
    struct mqtt_publish_param param;

    param.message.topic = publish_topic;
    param.message.payload.data = payload;
    param.message.payload.len = sizeof(payload) - 1;
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;

    mqtt_publish(&client, &param);
}

static struct k_work_delayable publish_work;

void main(void) {
    struct mqtt_client_init_params init_params = {
        .client_id = (struct mqtt_utf8){.utf8 = "esp32_client", .size = 11},
        .user_name = NULL,
        .password = NULL,
        .protocol_version = MQTT_VERSION_3_1_1,
        .rx_buffer = rx_buffer,
        .rx_buffer_size = sizeof(rx_buffer),
        .tx_buffer = tx_buffer,
        .tx_buffer_size = sizeof(tx_buffer),
    };

    mqtt_client_init(&client, &init_params);
    connect_to_broker();

    k_work_init_delayable(&publish_work, publish_state);
    k_work_schedule(&publish_work, PUBLISH_INTERVAL);

    while (1) {
        mqtt_input(&client);
        mqtt_live(&client);
        k_sleep(K_SECONDS(1));
    }
}