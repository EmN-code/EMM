#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(mqtt_simple, LOG_LEVEL_DBG);

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_TOPIC "state"
#define MSG_WORK "work"
#define MSG_NOT_WORK "not work"
#define STACK_SIZE 4096
#define THREAD_PRIORITY 5

static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static struct mqtt_client client;
static struct sockaddr_in broker;
static struct mqtt_utf8 client_id = {.utf8 = (const uint8_t *)"mqtt_client", .size = 11};

static void prepare_fds(struct mqtt_client *c)
{
  if (c->transport.type == MQTT_TRANSPORT_NON_SECURE) {
    c->transport.tcp.sock = zsock_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  }
}

static int start_client(void)
{
  int err;

  prepare_fds(&client);

  broker.sin_family = AF_INET;
  broker.sin_port = htons(BROKER_PORT);
  inet_pton(AF_INET, BROKER_ADDR, &broker.sin_addr);

  err = mqtt_connect(&client);
  if (err) {
    LOG_ERR("mqtt_connect failed: %d", err);
    return err;
  }

  return 0;
}

static void publish_message(struct mqtt_client *client, const char *topic, const char *msg)
{
  struct mqtt_publish_param param;

  param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
  param.message.topic.topic.utf8 = (const uint8_t *)topic;
  param.message.topic.topic.size = strlen(topic);
  param.message.payload.data = (const uint8_t *)msg;
  param.message.payload.len = strlen(msg);
  param.message_id = 0;
  param.dup_flag = 0;
  param.retain_flag = 0;

  mqtt_publish(client, &param);
}

void main(void)
{
  int err;

  memset(&client, 0x00, sizeof(client));
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

  err = start_client();
  if (err) {
    LOG_ERR("Failed to start MQTT client: %d", err);
    return;
  }

  while (1) {
    publish_message(&client, PUB_TOPIC, MSG_WORK);
    k_sleep(K_SECONDS(5));
    publish_message(&client, PUB_TOPIC, MSG_NOT_WORK);
    k_sleep(K_SECONDS(5));
  }
}