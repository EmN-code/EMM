#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <random/rand32.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENTID "zephyr_esp32_client"

#define STACK_SIZE 1024
#define THREAD_PRIORITY 7

#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_storage broker;
static struct mqtt_publish_param param;
static struct mqtt_utf8 publish_topic = { .utf8 = "device/state", .size = sizeof("device/state") - 1 };

static void mqtt_evt_handler(struct mqtt_client *const c, const struct mqtt_evt *evt) {
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        printk("MQTT client connected!\n");
        break;
    case MQTT_EVT_DISCONNECT:
        printk("MQTT client disconnected!\n");
        break;
    case MQTT_EVT_PUBLISH:
        printk("MQTT message published\n");
        break;
    default:
        break;
    }
}

static int connect_to_mqtt_broker(void) {
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker4->sin_addr);

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = (uint8_t *)MQTT_CLIENTID;
    client.client_id.size = strlen(MQTT_CLIENTID);
    client.protocol_version = MQTT_VERSION_3_1_1;

    struct mqtt_sec_config tls_config = {0};
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.transport.tls.config = &tls_config;

    return mqtt_connect(&client);
}

static void publish_state_work(bool work) {
    const char *state = work ? "work" : "not work";
    param.message.topic = publish_topic;
    param.message.payload.data = state;
    param.message.payload.len = strlen(state);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;

    mqtt_publish(&client, &param);
}

static void publish_state_thread(void) {
    while (1) {
        publish_state_work(true);
        k_sleep(PUBLISH_INTERVAL);
        publish_state_work(false);
        k_sleep(PUBLISH_INTERVAL);
    }
}

K_THREAD_DEFINE(publish_state_thread_id, STACK_SIZE, publish_state_thread, NULL, NULL, NULL, THREAD_PRIORITY, 0, 0);

void main(void) {
    int ret;

    ret = connect_to_mqtt_broker();
    if (ret) {
        printk("Failed to connect to the broker, error %d\n", ret);
        return;
    }
}