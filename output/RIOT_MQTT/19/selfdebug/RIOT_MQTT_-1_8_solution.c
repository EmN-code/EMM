#include <xtimer.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include "net/sock/udp.h"
#include "net/af.h"
#include "log.h"

int main(void) {
    struct sockaddr_in broker_addr;
    int sock;
    int ret;
    const char *broker_ip = "47.102.103.1";
    const char *state_msg = "work";
    uint16_t broker_port = 1883;
    socklen_t addr_len = sizeof(broker_addr);

    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(broker_port);
    ret = inet_pton(AF_INET, broker_ip, &broker_addr.sin_addr);
    if (ret <= 0) {
        LOG_ERROR("Invalid broker IP address\n");
        return -1;
    }

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        LOG_ERROR("Failed to create socket\n");
        return -1;
    }

    ret = connect(sock, (struct sockaddr *) &broker_addr, addr_len);
    if (ret < 0) {
        LOG_ERROR("Failed to connect to broker\n");
        return -1;
    }

    while (1) {
        ret = send(sock, state_msg, strlen(state_msg), 0);
        if (ret < 0) {
            LOG_ERROR("Failed to send message\n");
        } else {
            LOG_INFO("Message sent: %s\n", state_msg);
        }
        xtimer_sleep(5);
    }

    close(sock);
    return 0;
}