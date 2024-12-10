#include <thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/mqtt.h>
#include <net/conn.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUBLISH_INTERVAL (5U)

static mqtt_client_t client;

void publish_data(mqtt_client_t *client) {
    static char *topic = "device/state";
    static bool work_state = 0;
    char *state_msg;
    
    work_state = !work_state;  // Toggle state
    
    if(work_state) {
        state_msg = "work";
    } else {
        state_msg = "not work";
    }
    
    mqtt_publish(client, topic, state_msg, strlen(state_msg), MQTT_QOS_0, false);
}

void *mqtt_thread(void *arg) {
    (void)arg;
    sockaddr_t addr;
    mqtt_opt_t options = MQTT_OPT_NONE;

    addr.family = AF_INET;
    addr.netmask = 0;
    addr.port = MQTT_BROKER_PORT;
    ipv4_addr_from_str((ipv4_addr_t *)&addr.addr.ipv4, MQTT_BROKER_IP);

    mqtt_client_init(&client, NULL, NULL, mqtt_options_init(), NULL);
    mqtt_connect(&client, &addr, NULL, sizeof(mqtt_opt_t));

    while (1) {
        publish_data(&client);
        xtimer_sleep(MQTT_PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void) {
    printf("Starting MQTT example with custom implementation\n");

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN, 0, mqtt_thread, NULL, "mqtt_thread");

    // Keeping the main thread active
    while(1) {
        xtimer_sleep(60);
    }

    return 0;
}