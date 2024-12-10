#include <zephyr.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <string.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(mqtt_simple, LOG_LEVEL_DBG);

#define MQTT_CLIENT_STACK_SIZE 1024
#define MQTT_CLIENT_THREAD_PRIORITY 7

struct mqtt_client client;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static uint8_t payload_buf[128];

static void mqtt_event_handler(struct mqtt_client *const cli, 
                               const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            LOG_INF("MQTT client connected!");
            break;

        case MQTT_EVT_PUBLISH:
            LOG_INF("MQTT publish event received!");
            break;

        case MQTT_EVT_DISCONNECT:
            LOG_INF("MQTT client disconnected!");
            break;

        default:
            break;
    }
}

void configure_mqtt_client(void)
{
    struct sockaddr_in broker;
    broker.sin_family = AF_INET;
    broker.sin_port = htons(1883);
    inet_pton(AF_INET, "47.102.103.1", &broker.sin_addr);

    mqtt_client_init(&client);

    client.broker = (struct sockaddr *)&broker;
    client.evt_cb = mqtt_event_handler;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
}

void main(void)
{
    configure_mqtt_client();
    char message[] = "work or not work";
    struct mqtt_publish_param param;

    while (1) {
        mqtt_live(&client);

        param.message.payload.data = message;
        param.message.payload.len = strlen(message);
        param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
        param.message_id = sys_rand32_get();

        int err = mqtt_publish(&client, &param);
        if (err) {
            LOG_ERR("Failed to publish: %d", err);
        }

        k_sleep(K_SECONDS(5));
    }
}