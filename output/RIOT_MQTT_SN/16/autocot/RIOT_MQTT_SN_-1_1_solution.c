#include "ztimer.h"
#include "net/emcute.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "esp32client"
#define TOPIC_NAME "device/state"
#define PUBLISH_INTERVAL (5 * ZTIMER_SECURE_MILLISECONDS_PER_SEC)

static emcute_topic_t state_topic;
static char *state = "work";
static ztimer_t timer;

static void on_publish_timer(event_t *event) {
    (void) event;

    emcute_publish(&state_topic, 
                   (uint8_t *)state, 
                   strlen(state), 
                   EMCUTE_QOS_0);
                   
    emcute_disconnect();
    emcute_connect(BROKER_IP, BROKER_PORT, CLIENT_ID);
}

int main(void) {
    emcute_con_t client;
    emcute_topic_t topic;

    emcute_connect(BROKER_IP, BROKER_PORT, CLIENT_ID);

    topic.name = TOPIC_NAME;
    emcute_reg_t reg;
    emcute_reg_t *regptr = &reg;

    emcute_sub_t subscription;

    emcute_reg(&topic, regptr);

    state_topic.name = topic.name;
    state_topic.id = reg.id;
    
    ztimer_init(&timer, ZTIMER_SECURE, on_publish_timer, NULL);
    
    ztimer_set_oneshot(&timer, PUBLISH_INTERVAL, NULL);

    while (1) {
        ztimer_periodic_get_deadline(&timer, 5000);
        // Allow time for other tasks - Sleep for interval duration
        ztimer_sleep(ZTIMER_SECURE, PUBLISH_INTERVAL);
    }

    return 0;
}