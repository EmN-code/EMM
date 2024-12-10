#include <xtimer.h>  // Timer functionality
#include <string.h>   // String manipulation and memory functions
#include <stdio.h>    // Basic I/O functions
#include <emcute.h>   // For emcute MQTT-SN client functionality

#define EMCUTE_ID           "mqtt-sn-client"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_PORT         (1883U)
#define EMCUTE_ADDRESS      "47.102.103.1"
#define PUBLISH_INTERVAL    (5U)

static emcute_topic_t state_topic;
static char *state_topic_name = "device/state";
static char *device_state_work = "work";
static char *device_state_not_work = "not work";

static void publish_state(bool work)
{
    const char *state = work ? device_state_work : device_state_not_work;
    emcute_pub(&state_topic, state, strlen(state), EMCUTE_QOS_0);
    printf("Published state: %s\n", state);
}

int main(void)
{
    emcute_topic_t t;

    t.name = state_topic_name;
    state_topic = t;

    emcute_connect(EMCUTE_PORT, EMCUTE_ADDRESS);
    emcute_sub(&state_topic, EMCUTE_QOS_0);
    
    bool work_state = true; // Example state variable

    while (1) {
        publish_state(work_state);
        work_state = !work_state; // Toggle state for demo purposes
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}