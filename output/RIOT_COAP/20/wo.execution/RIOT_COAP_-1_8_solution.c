#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "thread_config.h"
#include "thread.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "stdio.h"
#include "sched.h"
#include "paho_mqtt.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/nanocoap/cache.h"
#include "mutex.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "ztimer.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

int main(void) {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[256];

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("Error creating socket\n");
        return 1;
    }

    // Set server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(COAP_PORT);
    inet_pton(AF_INET, COAP_SERVER_IP, &server_addr.sin_addr);

    // Initialize CoAP PDU
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, (uint8_t *)buffer, sizeof(buffer), COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, COAP_PAYLOAD, strlen(COAP_PAYLOAD));
    pdu.payload_len = strlen(COAP_PAYLOAD);

    // Send CoAP request
    ssize_t res = sendto(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (res < 0) {
        printf("Error sending request\n");
    } else {
        printf("CoAP request sent\n");
    }

    // Close socket
    close(sock);
    return 0;
}