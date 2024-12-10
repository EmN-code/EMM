#include <zephyr.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <random/rand32.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENTID    "zephyr_publisher"
#define MQTT_TOPIC       "esp32/state"
#define MQTT_PAYLOAD     "work"
#define STACK_SIZE       2048
#define THREAD_PRIORITY  5

static struct mqtt_client client;
static struct sockaddr_storage broker;
static struct zsock_pollfd fds;
static struct mqtt_utf8 publish_topic = {.utf8 = MQTT_TOPIC, .size = sizeof(MQTT_TOPIC) - 1};
static struct mqtt_utf8 payload = {.utf8 = MQTT_PAYLOAD, .size = sizeof(MQTT_PAYLOAD) - 1};

void broker_init(void) {
	struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
	broker4->sin_family = AF_INET;
	broker4->sin_port = htons(MQTT_BROKER_PORT);
	zsock_inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker4->sin_addr);
}

void connect_mqtt(void) {
	struct mqtt_sec_config *tls_config = &client.transport.tls.config;
	struct mqtt_client *client_ctx = &client;

	client_ctx->transport.type = MQTT_TRANSPORT_NON_SECURE;
	client_ctx->broker = &broker;
	client_ctx->evt_cb = NULL;
	client_ctx->client_id.utf8 = MQTT_CLIENTID;
	client_ctx->client_id.size = strlen(MQTT_CLIENTID);
	client_ctx->password = NULL;
	client_ctx->user_name = NULL;
	client_ctx->protocol_version = MQTT_VERSION_3_1_1;

	mqtt_connect(client_ctx);
	fds.fd = client_ctx->transport.tcp.sock;
	fds.events = ZSOCK_POLLIN;
}

void publish_state(void) {
	struct mqtt_publish_param param;

	param.message.topic = publish_topic;
	param.message.payload.data = payload.utf8;
	param.message.payload.len = payload.size;
	param.message_id = sys_rand32_get();
	param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
	param.dup_flag = 0U;
	param.retain_flag = 0U;

	mqtt_publish(&client, &param);
}

void mqtt_thread(void) {
	broker_init();
	connect_mqtt();

	while (1) {
		publish_state();
		k_sleep(K_SECONDS(5));
	}
}

K_THREAD_DEFINE(mqtt_thread_id, STACK_SIZE, mqtt_thread, NULL, NULL, NULL, THREAD_PRIORITY, 0, 0);

int main(void) {
	printk("MQTT publisher started\n");
	return 0;
}