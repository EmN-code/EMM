#include <zephyr/net/socket.h>
#include <zephyr/mgmt/mqtt.h>
#include <zephyr/kernel.h>

#define BROKER_HOSTNAME "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_CLIENTID "zephyr_client"
#define TOPIC "state"
#define PAYLOAD_WORK "work"
#define PAYLOAD_NOT_WORK "not work"
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_storage broker;
static struct k_work_delayable mqtt_work;

void mqtt_event_handler(struct mqtt_client *const c, const struct mqtt_evt *evt)
{
    /* Handle different MQTT events here as needed. */
}

void mqtt_work_fn(struct k_work *work)
{
    struct mqtt_publish_param param;
    int ret;

    /* Publish "work" message to the topic. */
    param.message.payload.data = PAYLOAD_WORK;
    param.message.payload.len = strlen(PAYLOAD_WORK);
    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = TOPIC;
    param.message.topic.topic.size = strlen(TOPIC);

    ret = mqtt_publish(&client, &param);
    if (ret) {
        printk("mqtt_publish failed: %d\n", ret);
    }

    /* Schedule next publish. */
    k_work_schedule(&mqtt_work, PUBLISH_INTERVAL);
}

void mqtt_setup(void)
{
    struct mqtt_client *client = &client;

    mqtt_client_init(client);

    broker.ss_family = AF_INET;
    net_sin(&broker)->sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_HOSTNAME, &net_sin(&broker)->sin_addr);

    client->broker = &broker;
    client->evt_cb = mqtt_event_handler;
    client->client_id.utf8 = (uint8_t *)MQTT_CLIENTID;
    client->client_id.size = strlen(MQTT_CLIENTID);
    client->transport.type = MQTT_TRANSPORT_NON_SECURE;
}

int main(void)
{
    printk("Starting MQTT sample...\n");

    mqtt_setup();

    /* Connect to the MQTT broker. */
    int ret = mqtt_connect(&client);
    if (ret) {
        printk("Unable to connect to MQTT broker: %d\n", ret);
        return ret;
    }

    /* Initialize and schedule work for publishing. */
    k_work_init_delayable(&mqtt_work, mqtt_work_fn);
    k_work_schedule(&mqtt_work, PUBLISH_INTERVAL);

    while (1) {
        mqtt_input(&client);
        mqtt_live(&client);
        k_sleep(K_SECONDS(1));
    }

    return 0;
}