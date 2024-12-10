#include <zephyr/kernel.h>  // Provides kernel functionalities, such as timers and threads.
#include <zephyr/mqtt/mqtt.h>  // Provides the MQTT client API functions.
#include <zephyr/net/socket.h>  // Provides networking socket APIs.
#include <string.h>  // Provides string manipulation functions for message handling.

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL K_SECONDS(5)  // Publish every 5 seconds.

static struct mqtt_client client;
static struct sockaddr_storage broker;

// MQTT event handler
static void mqtt_evt_handler(struct mqtt_client *client, const struct mqtt_evt *evt) {
    // Handle different MQTT events, if necessary
}

// Timer handler to publish the state
void work_handler(struct k_work *work) {
    // Define the publish message parameters and data
    struct mqtt_publish_param param;
    param.message.payload.data = "work or not work";
    param.message.payload.len = strlen("work or not work");
    param.message.topic.topic.utf8 = "state";
    param.message.topic.topic.size = strlen("state");
    param.message_id = 1;
    param.dup_flag = 0;
    param.retain_flag = 0;
    param.qos = MQTT_QOS_0_AT_MOST_ONCE;

    // Publish the message
    int rc = mqtt_publish(&client, &param);
    if (rc != 0) {
        printk("mqtt_publish failed: %d\n", rc);
    }
}

// Timer initialization
K_WORK_DEFINE(work, work_handler);
K_TIMER_DEFINE(timer, k_work_submit, NULL);

// Main function
int main(void) {
    printk("Starting MQTT publisher example...\n");

    // Prepare broker address
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &broker4->sin_addr);

    // Initialize the MQTT client
    mqtt_client_init(&client);
    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = "zephyr_pub";
    client.client_id.size = strlen("zephyr_pub");
    client.protocol_version = MQTT_VERSION_3_1_1;

    // Connect to the broker
    int rc = mqtt_connect(&client);
    if (rc != 0) {
        printk("mqtt_connect failed: %d\n", rc);
        return rc;
    }

    // Start the timer to schedule periodic publishing
    k_timer_start(&timer, PUBLISH_INTERVAL, PUBLISH_INTERVAL);

    while (1) {
        // This loop can be used to process MQTT events
        mqtt_input(&client);
        mqtt_live(&client);

        // Sleep until next processing cycle
        k_sleep(K_MSEC(100));
    }

    return 0;
}