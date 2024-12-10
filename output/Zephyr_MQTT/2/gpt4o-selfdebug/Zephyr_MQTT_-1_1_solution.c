#include <zephyr/net/mqtt.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr.h>

#define MQTT_BROKER_ADDR   "47.102.103.1"
#define MQTT_BROKER_PORT   1883
#define PUBLISH_INTERVAL   K_SECONDS(5)
#define MQTT_CLIENTID      "esp32_client"

static struct mqtt_client client;
static struct sockaddr_storage broker;
static struct mqtt_pubmsg pub_msg;
static uint8_t mqtt_payload[] = "work or not work";

static struct k_work_delayable pub_work;

/* Forward declarations */
void mqtt_event_handler(struct mqtt_client *const client, const struct mqtt_evt *evt);
void schedule_publish(struct k_work *work);

int init_mqtt_client(void) {
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker4->sin_addr);

    mqtt_client_init(&client);

    client.broker = &broker;
    client.evt_cb = mqtt_event_handler;
    client.client_id.utf8 = (uint8_t *)MQTT_CLIENTID;
    client.client_id.size = strlen(MQTT_CLIENTID);
    client.protocol_version = MQTT_VERSION_3_1_1;

    client.rx_buf = NULL;
    client.rx_buf_size = 0;
    client.tx_buf = NULL;
    client.tx_buf_size = 0;
    client.user_data = NULL;

    return mqtt_connect(&client);
}

void mqtt_event_handler(struct mqtt_client *const client, const struct mqtt_evt *evt) {
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        if (evt->result != 0) {
            printk("MQTT connect failed %d\n", evt->result);
            return;
        }

        printk("MQTT client connected!\n");
        k_work_schedule(&pub_work, PUBLISH_INTERVAL);
        break;

    case MQTT_EVT_DISCONNECT:
        printk("MQTT client disconnected %d\n", evt->result);
        break;

    default:
        break;
    }
}

void schedule_publish(struct k_work *work) {
    pub_msg.message.payload.data = mqtt_payload;
    pub_msg.message.payload.len = sizeof(mqtt_payload) - 1;
    pub_msg.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    pub_msg.message.topic.topic.utf8 = (uint8_t *)"esp32/status";
    pub_msg.message.topic.topic.size = strlen("esp32/status");
    pub_msg.message_id = sys_rand32_get();
    pub_msg.dup_flag = 0;
    pub_msg.retain_flag = 0;

    mqtt_publish(&client, &pub_msg);

    k_work_reschedule(&pub_work, PUBLISH_INTERVAL);
}

void main(void) {
    printk("Starting MQTT sample\n");

    if (init_mqtt_client() != 0) {
        printk("MQTT client initialization failed\n");
        return;
    }

    k_work_init_delayable(&pub_work, schedule_publish);

    while (1) {
        mqtt_input(&client);
        mqtt_live(&client);
        k_sleep(K_MSEC(500));
    }
}