#include <stdio.h>
#include <string.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <kernel.h>
#include <device.h>
#include <zephyr.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883

static struct mqtt_client client;
static struct sockaddr_storage broker;
static struct mqtt_utf8 client_id = { .utf8 = "zephyr_client", .size = sizeof("zephyr_client") - 1};
static struct mqtt_utf8 topic = { .utf8 = "zephyr/state", .size = sizeof("zephyr/state") - 1};
static char payload[32];
static struct k_work_delayable mqtt_work;

static void mqtt_publish_state(struct k_work *work) {
	static bool state = false;

	state = !state;
	if (state) {
		snprintk(payload, sizeof(payload), "work");
	} else {
		snprintk(payload, sizeof(payload), "not work");
	}

	struct mqtt_publish_param param;
	param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
	param.message.topic.topic.utf8 = topic.utf8;
	param.message.topic.topic.size = topic.size;
	param.message.payload.data = payload;
	param.message.payload.len = strlen(payload);
	param.message.id = sys_rand32_get();
	param.dup_flag = 0;
	param.retain_flag = 0;

	mqtt_publish(&client, &param);
	k_work_reschedule(&mqtt_work, K_SECONDS(5));
}

void main(void) {
	struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

	broker4->sin_family = AF_INET;
	broker4->sin_port = htons(MQTT_BROKER_PORT);
	zsock_inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker4->sin_addr);

	struct mqtt_sec_config tls_config = { 0 };
	struct mqtt_transport transport = {
		.type = MQTT_TRANSPORT_NON_SECURE,
	};

	mqtt_client_init(&client);
	client.broker = &broker;
	client.evt_cb = NULL;
	client.client_id.utf8 = client_id.utf8;
	client.client_id.size = client_id.size;
	client.protocol_version = MQTT_VERSION_3_1_1;
	client.transport = transport;
	client.rx_buf = NULL;
	client.rx_buf_size = 0;
	client.tx_buf = NULL;
	client.tx_buf_size = 0;

	mqtt_connect(&client);
	k_work_init_delayable(&mqtt_work, mqtt_publish_state);
	k_work_reschedule(&mqtt_work, K_NO_WAIT);

	while (true) {
		mqtt_input(&client);
		mqtt_live(&client);
		k_sleep(K_MSEC(500));
	}
}