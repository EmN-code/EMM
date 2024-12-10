#include <zephyr/kernel.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <stdio.h>

#define MQTT_BROKER_ADDRESS "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_storage broker;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
char payload[] = "Not work";

static void broker_init(void) {
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDRESS, &broker4->sin_addr);
}

static void mqtt_evt_handler(struct mqtt_client *client, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            if (evt->result != 0) {
                printf("MQTT connect failed: %d
", evt->result);
            } else {
                printf("MQTT connected!
");
            }
            break;
        case MQTT_EVT_DISCONNECT:
            printf("MQTT disconnected: %d
", evt->result);
            break;
        default:
            printf("MQTT event: %d
", evt->type);
            break;
    }
}

static void mqtt_publish(struct mqtt_client *client) {
    struct mqtt_publish_param param;
    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)"state";
    param.message.topic.topic.size = strlen("state");
    param.message.payload.data = (uint8_t *)payload;
    param.message.payload.len = strlen(payload);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;
    int rc = mqtt_publish(client, &param);
    if (rc) {
        printf("Failed to publish message: %d
", rc);
    } else {
        printf("Message published successfully
");
    }
}

void main(void) {
    mqtt_client_init(&client);
    broker_init();

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = (uint8_t *)"zephyr-mqtt-client";
    client.client_id.size = strlen("zephyr-mqtt-client");
    client.password = NULL;
    client.user_name = NULL;
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    int rc = mqtt_connect(&client);
    if (rc) {
        printf("MQTT connect failed: %d
", rc);
        return;
    }

    while (1) {
        mqtt_input(&client);
        mqtt_live(&client);
        mqtt_publish(&client);
        k_sleep(PUBLISH_INTERVAL);

        /* Toggle the payload for demonstration purposes */
        if (strcmp(payload, "Work") == 0) {
            strcpy(payload, "Not work");
        } else {
            strcpy(payload, "Work");
        }
    }
}