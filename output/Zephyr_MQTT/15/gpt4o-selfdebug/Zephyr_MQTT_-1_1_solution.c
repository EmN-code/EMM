#include <zephyr.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <stdio.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_CLIENTID "zephyr_client"

static struct mqtt_client client;
static struct sockaddr_storage broker;
static bool connected = false;

static void publish_state(struct mqtt_client *client)
{
    struct mqtt_publish_param param;
    
    param.message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;
 
    param.message.topic.topic.utf8 = (uint8_t *)"state";
    param.message.topic.topic.size = strlen("state");

    param.message.payload.data = (uint8_t *)(connected ? "work" : "not work");
    param.message.payload.len = strlen(connected ? "work" : "not work");

    param.message_id = sys_rand32_get();
    param.dup_flag = 0U;
    param.retain_flag = 0U;

    mqtt_publish(client, &param);
}

static void mqtt_event_handler(struct mqtt_client *const client, const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        if (evt->result) {
            printk("MQTT connect failed %d
", evt->result);
            return;
        }

        connected = true;
        printk("MQTT client connected!
");
        break;

    case MQTT_EVT_DISCONNECT:
        printk("MQTT client disconnected %d
", evt->result);
        connected = false;
        break;

    default:
        break;
    }
}

static int init_mqtt(void)
{
    mqtt_client_init(&client);

    struct sockaddr_in *broker4 = ((struct sockaddr_in *)&broker);
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &broker4->sin_addr);

    client.broker = &broker;
    client.evt_cb = mqtt_event_handler;
    client.client_id.utf8 = (uint8_t *)MQTT_CLIENTID;
    client.client_id.size = strlen(MQTT_CLIENTID);
    client.protocol_version = MQTT_VERSION_3_1_1;

    return mqtt_connect(&client);
}

void main(void)
{
    int ret;

    printk("Connecting to MQTT broker...
");

    ret = init_mqtt();
    if (ret) {
        printk("Failed to connect to MQTT broker (%d)
", ret);
        return;
    }

    while (1) {
        mqtt_input(&client);
        mqtt_live(&client);
        if (connected) {
            publish_state(&client);
        }
        k_sleep(K_SECONDS(5));
    }
}