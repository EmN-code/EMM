#include <zephyr/net/mqtt.h>
#include <zephyr/kernel.h>
#include <zephyr/console/console.h>
#include <zephyr/net/socket.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "esp32/state"
#define MESSAGE "work"

struct sockaddr_storage broker;
struct mqtt_client client;
static struct mqtt_utf8 client_id = MQTT_UTF8_INITL_PARAMS("client1");
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];

static void prepare_fds(struct mqtt_client *c)
{
    FD_ZERO(&c->fds[0]);
    FD_SET(c->transport.tcp.sock, &c->fds[0]);

    c->nfds = 1;
}

static int publish_message(struct mqtt_client *c)
{
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)TOPIC;
    param.message.topic.topic.size = strlen(TOPIC);
    param.message.payload.data = MESSAGE;
    param.message.payload.len = strlen(MESSAGE);

    
    return mqtt_publish(c, &param);
}

int main(void)
{
    int rc;

    console_init();
    memset(&broker, 0, sizeof(broker));
    ((struct sockaddr_in *)&broker)->sin_family = AF_INET;
    ((struct sockaddr_in *)&broker)->sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &((struct sockaddr_in *)&broker)->sin_addr);

    struct mqtt_client client;
    mqtt_client_init(&client);

    client.broker = &broker;
    client.evt_cb = NULL;
    client.client_id  = client_id;
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    rc = mqtt_connect(&client);

    if (rc != 0) {
        printk("Failed to connect, error: %d\n", rc);
        return rc;
    }

    while (1) {
        rc = publish_message(&client);
        if (rc != 0) {
            printk("Failed to publish, error: %d\n", rc);
            return rc;
        }
        k_sleep(K_SECONDS(5));
    }
    return 0;
}