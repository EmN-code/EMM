#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <net/mqtt.h>
#include <string.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_CLIENTID "ESPClient"
#define TOPIC_NAME "state"
#define MSG_WORK "work"
#define MSG_NOT_WORK "not work"
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_in broker;

void mqtt_evt_handler(struct mqtt_client *const client, const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            if (evt->result != 0) {
                printk("MQTT connect failed %d\n", evt->result);
                return;
            }
            printk("MQTT client connected!");
            break;
        case MQTT_EVT_DISCONNECT:
            printk("MQTT client disconnected %d\n", evt->result);
            break;
        case MQTT_EVT_PUBLISH:
            printk("MQTT publish event: packet id %u\n", evt->param.publish.message_id);
            break;
        default:
            break;
    }
}

void configure_mqtt(void)
{
    struct mqtt_sec_config *tls_config = NULL;

    mqtt_client_init(&client);
    broker.sin_family = AF_INET;
    broker.sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &broker.sin_addr);
    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = (uint8_t *)MQTT_CLIENTID;
    client.client_id.size = strlen(MQTT_CLIENTID);
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.rx_buf = (uint8_t[256]){};
    client.rx_buf_size = 256;
    client.tx_buf = (uint8_t[256]){};
    client.tx_buf_size = 256;
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;

    mqtt_connect(&client);
}

void publish_state(struct k_work *work)
{
    static bool work_state = true;
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)TOPIC_NAME;
    param.message.topic.topic.size = strlen(TOPIC_NAME);
    param.message.payload.data = (uint8_t *)(work_state ? MSG_WORK : MSG_NOT_WORK);
    param.message.payload.len = strlen(work_state ? MSG_WORK : MSG_NOT_WORK);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;

    mqtt_publish(&client, &param);
    work_state = !work_state;
}

K_WORK_DELAYABLE_DEFINE(publish_work, publish_state);

void main(void)
{
    configure_mqtt();
    while (1) {
        mqtt_input(&client);
        mqtt_live(&client);
        k_work_reschedule(&publish_work, PUBLISH_INTERVAL);
        k_sleep(K_SECONDS(1));
    }
}