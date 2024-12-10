#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <mqtt_snip.h>  // Include MQTT related RIOT header
#include <net/emcute.h> // Include the emcute MQTT-SN implementation header

#define BROKER_IP "47.102.103.1"  // IP address of the broker
#define BROKER_PORT 1883            // Port of the broker (default MQTT port)
#define MQTT_TOPIC "device/state"  // MQTT topic for publishing the state
#define MESSAGE_BUFFER_SIZE 64      // Size of the message buffer

static char message_buffer[MESSAGE_BUFFER_SIZE];

// This function publishes a message to the MQTT broker
static int publish_message(void)
{
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;  // Quality of service level 0

    // Setup topic
    t.name = MQTT_TOPIC;

    // Construct message
    time_t current_time = time(NULL);
    struct tm *tm_info = localtime(&current_time);
    const char *state = (tm_info->tm_sec % 2 == 0) ? "work" : "not work";
    snprintf(message_buffer, MESSAGE_BUFFER_SIZE, "Device state: %s", state);

    // Publish the message
    if (emcute_pub(&t, message_buffer, strlen(message_buffer), flags) != EMCUTE_OK) {
        puts("Error: unable to publish message");
        return 1;
    }

    printf("Published: %s\n", message_buffer);
    return 0;
}

int main(void)
{
    emcute_start(); // Start the emcute network thread

    // Connect to MQTT broker
    emcute_con_t con = {
        .port = BROKER_PORT,
        .name = NULL,
        .peer = BROKER_IP
    };

    if (emcute_con(&con, true) != EMCUTE_OK) {
        puts("Error: unable to connect to the broker\n");
        return 1;
    }

    puts("Connected to broker\n");

    while (1) {
        publish_message();
        xtimer_sleep(5); // Sleep for 5 seconds before publishing again
    }

    // Disconnect from the broker (never reached in this sample)
    emcute_discon();
    return 0;
}