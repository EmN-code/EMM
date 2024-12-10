#include <net/mqtt.h>
#include <net/socket.h>
#include <zephyr.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883

#define STACK_SIZE 2048
#define THREAD_PRIORITY 5

#define MQTT_CLIENTID "zephyr_publisher"

static struct mqtt_client client;
static struct sockaddr_storage broker;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static char payload[] = "work or not work";

void mqtt_evt_handler(struct mqtt_client *const client, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printf("MQTT client connected!\n");
            break;
        case MQTT_EVT_DISCONNECT:
            printf("MQTT client disconnected!\n");
            break;
        default:
            break;
    }
}

void mqtt_connect_work(struct k_work *work) {
    struct mqtt_sec_config tls_cfg = {0};
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &broker4->sin_addr);

    struct mqtt_client_init_param init_param = {
        .broker = &broker,
        .evt_cb = mqtt_evt_handler,
        .client_id.utf8 = MQTT_CLIENTID,
        .client_id.size = strlen(MQTT_CLIENTID),
        .password = NULL,
        .user_name = NULL,
        .rx_buf = rx_buffer,
        .rx_buf_size = sizeof(rx_buffer),
        .tx_buf = tx_buffer,
        .tx_buf_size = sizeof(tx_buffer),
    };

    mqtt_init(&client, &init_param);

    int rc = mqtt_connect(&client);
    if (rc != 0) {
        printf("Failed to connect to broker: %d\n", rc);
    }
}

void main(void) {
    struct k_work_delayable mqtt_work;
    k_work_init_delayable(&mqtt_work, mqtt_connect_work);

    while (1) {
        mqtt_publish(&client, &(struct mqtt_publish_param){
            .message.topic.topic.utf8 = "state",
            .message.topic.topic.size = strlen("state"),
            .message.payload.data = payload,
            .message.payload.len = strlen(payload),
            .message_id = sys_rand32_get(),
            .dup_flag = 0,
            .retain_flag = 0,
            .message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE,
        });

        k_sleep(K_SECONDS(5));
    }
}
