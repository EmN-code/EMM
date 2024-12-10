#include "xtimer.h"
#include "net/sock/util.h"
#include "net/ipv6/addr.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include <unistd.h>
#include <string.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "work or not work"
#define INTERVAL (5U * 1000000U) // 5 seconds in microseconds

typedef struct {
    sock_udp_ep_t server;
    sock_udp_t sock;
} udp_conn_t;

void udp_reconnect(udp_conn_t *conn) {
    while (1) {
        if (sock_udp_create(&conn->sock, NULL, NULL, 0) == 0) {
            printf("Socket created successfully\n");
            break;
        } else {
            printf("Failed to create socket, retrying...\n");
            xtimer_usleep(1000000); // wait for 1 second
        }
    }
}

void *send_thread(void *arg) {
    udp_conn_t *conn = (udp_conn_t *)arg;
    char data[] = MESSAGE;
    size_t len = strlen(data);

    while (1) {
        int res = sock_udp_send(&conn->sock, data, len, &conn->server);
        if (res < 0) {
            printf("Failed to send message, error code: %d\n", res);
            udp_reconnect(conn);
        } else {
            printf("Message sent: %s\n", MESSAGE);
        }
        xtimer_usleep(INTERVAL);
    }
    return NULL;
}

int main(void) {
    udp_conn_t conn;
    memset(&conn, 0, sizeof(conn));

    ipv6_addr_set_all_nodes_multicast(&conn.server.addr.ipv6);
    conn.server.family = AF_INET6;
    conn.server.netif = SOCK_ADDR_ANY_NETIF;
    conn.server.port = SERVER_PORT;

    udp_reconnect(&conn);

    thread_create(send_thread, &conn);

    return 0;
}
