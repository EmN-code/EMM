#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/sock.h>
#include <periph/wdt.h>
#include <periph/rtc.h>
#include <mqtt.h>

// Define MQTT server settings
#define MQTT_ADDR      "47.102.103.1"
#define MQTT_PORT      1883
#define MQTT_TOPIC     "device/state"
#define INTERVAL       (5LU * US_PER_SEC)  // 5 seconds interval

static mqtt_client_t mqtt_client;
static mqtt_connect_client_info_t mqtt_params = {
    .client_id = "esp32_device",
};

static void _on_pub_request_sent(void *arg)
{
    (void)arg;
    printf("** Awaiting next publish **\n");
}

static void _on_connect_success(mqtt_client_t *client)
{
    printf("Connected to the MQTT broker!\n");
    (void)client;
}

static void _on_connect_fail(mqtt_client_t *client, int result_code)
{
    printf("Connection to the MQTT broker failed with code: %d\n", result_code);
    (void)client;
}

static void _publish_state(void)
{
    static int state = 0;
    mqtt_publish(&mqtt_client, MQTT_TOPIC, state ? "work" : "not work", state ? 4 : 8,
                 MQTT_QOS_0, false, _on_pub_request_sent, NULL);
    state = !state;
}

void *publisher_thread(void *arg)
{
    (void)arg;
    while (1) {
        _publish_state();
        xtimer_sleep(INTERVAL);
    }
    return NULL;
}

int main(void)
{
    printf("Starting MQTT client\n");

    // Set up the client's connection parameters
    mqtt_client.client_info = &mqtt_params;

    // Connect to the MQTT broker
    if (mqtt_connect(&mqtt_client, MQTT_ADDR, MQTT_PORT, _on_connect_success, _on_connect_fail, 0) != 0) {
        printf("Connection to MQTT broker failed\n");
        return 1;
    }

    // Launch publisher thread
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, publisher_thread, NULL, "pub_thread");

    return 0;
}