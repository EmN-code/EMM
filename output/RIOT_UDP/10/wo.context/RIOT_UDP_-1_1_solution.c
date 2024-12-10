#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/af.h>
#include <net/protnet/protnet_udp.h>

// IP address and port of the server
#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001

// Send interval in microseconds (5 seconds)
#define SEND_INTERVAL (5U * US_PER_SEC)

static net_udp_t udp_socket;

typedef struct {
    char *msg;
    uint16_t msg_len;
} device_state_t;

void *send_state_thread(void *arg) {
    (void) arg;
    sock_udp_ep_t server = {
        .family = AF_INET,
        .port = SERVER_PORT
    };

    if (inet_pton(AF_INET, SERVER_ADDR, &server.addr) != 1) {
        printf("Error: unable to parse remote address\n");
        return NULL;
    }

    char state_not_work[] = "not work";
    char state_work[] = "work";
    device_state_t states[] = {{state_work, sizeof(state_work)}, {state_not_work, sizeof(state_not_work)}};

    size_t state_index = 0;

    while (1) {
        device_state_t *state = &states[state_index];

        ssize_t res = sock_udp_send(&udp_socket, state->msg, state->msg_len, &server);
        if (res < 0) {
            printf("Error sending UDP packet\n");
        } else {
            printf("UDP packet sent: %s\n", state->msg);
        }

        state_index = (state_index + 1) % 2;

        xtimer_sleep(SEND_INTERVAL / US_PER_SEC);
    }

    return NULL;
}

int main(void) {
    if (sock_udp_create(&udp_socket, NULL, NULL, 0) < 0) {
        printf("Error initializing UDP\n");
        return 1;
    }

    thread_create(send_state_thread, NULL);

    return 0;
}

/* Note: In practice, you might want to adjust the interval or the device state management according to your specific needs. */