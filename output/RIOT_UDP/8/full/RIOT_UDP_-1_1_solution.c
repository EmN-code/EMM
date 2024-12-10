#include <xtimer.h>
#include <udp.h>
#include <thread.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * US_PER_SEC)  // 5 seconds

static char *device_state = "work";  // or "not work"

int main(void) {
    struct sockaddr src;
    struct sockaddr_dst;
    struct udp_conn conn;
    int rc;

    // Connect to the UDP server
    while (1) {
        rc = udp_connect(&conn, NULL, SERVER_IP, SERVER_PORT);
        if (rc != 0) {
            printf("Failed to connect to UDP server. Retrying in 1 second...\n");
            xtimer_sleep(1);
            continue;
        }
        printf("Connected to UDP server successfully.\n");
        break;
    }

    // Sending loop
    while (1) {
        ssize_t num_bytes = udp_send(&conn, device_state, sizeof(device_state) - 1);
        if(num_bytes < 0){
            printf("Failed to send state.\n");
            udp_disconnect(&conn);
            // Try connecting again
            rc = udp_connect(&conn, NULL, SERVER_IP, SERVER_PORT);
            if (rc != 0) {
                printf("Failed to reconnect to UDP server.\n");
                xtimer_sleep(1);
                continue;
            }
        } else {
            printf("State sent successfully.\n");
        }
        xtimer_sleep(5);
    }
    return 0;
}