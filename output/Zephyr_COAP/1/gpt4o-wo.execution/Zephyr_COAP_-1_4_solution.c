#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/types.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/socket.h>
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <coap3/coap.h>

#define SERVER_EP "coap://47.102.103.1"
#define SERVER_PORT "5683"
#define URI_PATH "/coap/test"

void main(void) {
    int sock;
    struct sockaddr_in server_addr;
    coap_context_t *ctx;
    coap_session_t *session;
    coap_pdu_t *pdu;
    coap_mid_t mid;

    printf("Setting up socket...\n");
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("Failed to create socket\n");
        return;
    }

    printf("Initializing CoAP library...\n");
    coap_startup();

    ctx = coap_new_context(NULL);
    if (!ctx) {
        printf("Failed to create CoAP context\n");
        close(sock);
        return;
    }

    printf("Setting server address...\n");
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(SERVER_PORT));
    inet_pton(AF_INET, "47.102.103.1", &server_addr.sin_addr);

    session = coap_new_client_session(ctx, NULL, (const struct sockaddr*)&server_addr, COAP_PROTO_UDP);
    if (!session) {
        printf("Failed to create CoAP session\n");
        coap_free_context(ctx);
        close(sock);
        return;
    }

    pdu = coap_pdu_init(COAP_MESSAGE_CON, COAP_REQUEST_POST, coap_new_message_id(session), coap_session_max_pdu_size(session));
    if (!pdu) {
        printf("Failed to create CoAP PDU\n");
        coap_session_release(session);
        coap_free_context(ctx);
        close(sock);
        return;
    }

    coap_add_option(pdu, COAP_OPTION_URI_PATH, strlen(URI_PATH), (const uint8_t *)URI_PATH);
    coap_add_data(pdu, 16, (const uint8_t *)"Hello, COAP Cloud");

    printf("Sending CoAP request...\n");
    mid = coap_send(session, pdu);
    if (mid == COAP_INVALID_MID) {
        printf("Failed to send CoAP request\n");
    }

    coap_session_release(session);
    coap_free_context(ctx);
    close(sock);

    printf("CoAP request sent successfully\n");
}