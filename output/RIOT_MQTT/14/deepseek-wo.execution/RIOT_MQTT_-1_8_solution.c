
#include "net/ipv6/addr.h"
#include "net/emcute.h"
#include "paho_mqtt.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "time_units.h"
#include "esp_system.h"
#include "xtimer.h"
#include "thread.h"
#include "mutex.h"
#include "mma8x5x_params.h"
#include "stdio.h"
#include "dht_params.h"
#include "stdatomic.h"
#include "ztimer.h"
#include "dht.h"
#include "esp_wifi.h"
#include "stdbool.h"
#include "kernel_defines.h"
#include "net/sock/util.h"
#include "evtimer_mbox.h"
#include "netinet/in.h"
#include "timex.h"
#include "evtimer_msg.h"
#include "net/sock.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "architecture.h"
#include "stddef.h"
#include "mbox.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"

static void publish_state(void) {
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        puts("Error creating socket");
        return;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        puts("Error connecting to server");
        close(sock);
        return;
    }

    const char *message = "PUBLISH " TOPIC " " PAYLOAD "\n";
    send(sock, message, strlen(message), 0);

    close(sock);
}

int main(void) {
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
