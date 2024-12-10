#include <zephyr/types.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <net/socket.h>

#define STACK_SIZE 1024
#define PRIORITY 7

static struct mqtt_client client;
static struct sockaddr_in broker;
char payload[] = "work or not work";

void prepare_fds(struct mqtt_client *c, struct pollfd *fds)
{
    fds->fd = c->transport.tcp.sock;
    fds->events = POLLIN;
    fds->revents = 0;
}

void client_init(void)
{
    static uint8_t rx_buffer[128];
    static uint8_t tx_buffer[128];
    static struct mqtt_utf8 client_id = { .utf8 = (uint8_t *)"zephyr_publisher", .size = 16 };

    mqtt_client_init(&client);

    broker.sin_family = AF_INET;
    broker.sin_port = htons(1883);
    inet_pton(AF_INET, "47.102.103.1", &broker.sin_addr);

    client.broker = &broker;
    client.evt_cb = NULL;
    client.client_id = client_id;
    client.password = NULL;
    client.user_name = NULL;
    client.protocol_version = MQTT_VERSION_3_1_1;

    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
}

void mqtt_publish_message(struct mqtt_client *c)
{
    struct mqtt_publish_param param;
    struct mqtt_utf8 topic = {
        .utf8 = (uint8_t *)"zephyr/state",
        .size = strlen("zephyr/state")
    };

    param.message.topic = topic;
    param.message.payload.data = payload;
    param.message.payload.len = strlen(payload);
    param.message_id = k_uptime_get_32();
    param.dup_flag = 0;
    param.retain_flag = 0;
    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;

    mqtt_publish(c, &param);
}

void main(void)
{
    struct pollfd fds;
    int rc;

    client_init();

    rc = mqtt_connect(&client);
    if (rc != 0) {
        printk("mqtt_connect failed: %d\n", rc);
        return;
    }

    while (1) {
        mqtt_input(&client);
        prepare_fds(&client, &fds);

        rc = poll(&fds, 1, K_SECONDS(5));

        if (rc > 0 && (fds.revents & POLLIN)) {
            mqtt_input(&client);
        }

        mqtt_publish_message(&client);
        k_sleep(K_SECONDS(5));
    }

    mqtt_disconnect(&client);
}