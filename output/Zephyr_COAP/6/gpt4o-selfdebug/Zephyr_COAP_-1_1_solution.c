#include <zephyr/net/tls_credentials.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/tracing/tracing.h>

#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <stdlib.h>
#include <string.h>

#define COAP_HOST "47.102.103.1"
#define COAP_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"

void main(void) {
    int sock;
    struct sockaddr_in server_addr;
    char coap_payload[] = COAP_MESSAGE;
    struct zsock_pollfd fds[1];
    int ret;

    k_sleep(K_SECONDS(1));  // Allow network interface to initialize

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(COAP_PORT);
    inet_pton(AF_INET, COAP_HOST, &server_addr.sin_addr);

    /* Send CoAP POST Request */
    ret = sendto(sock, coap_payload, strlen(coap_payload), 0,
                 (struct sockaddr *)&server_addr, sizeof(server_addr));

    if (ret < 0) {
        printk("Failed to send CoAP request\n");
        close(sock);
        return;
    }

    printk("CoAP request sent\n");

    close(sock);
}