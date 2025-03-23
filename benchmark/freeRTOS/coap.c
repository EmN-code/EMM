#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_netif.h"
#include<sys/socket.h>
#include<netdb.h>

#include "coap3/coap.h"


// COAP
#define COAP_URI "coap://10.214.131.229/basic"
#define COAP_PORT 14023
#define COAP_PAYLOAD "Hello CoAP!"
static int have_response = 0;

// WiFi credentials
#define WIFI_SSID "EmNets-301"
#define WIFI_PASS "eagle402"
int connected = 0;

// tag for log
static const char *TAG = "COAP_CLIENT";

// Event handler for WiFi
static TaskHandle_t wifi_task_handle = NULL;

// Event handler for WiFi
static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data)
{
    if(event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START){
        esp_wifi_connect();
        ESP_LOGI(TAG, "WiFi Connecting...");
    }else if(event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED){
        ESP_LOGI(TAG, "WiFi Connected");
    }else if(event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED){
        ESP_LOGI(TAG, "WiFi Disconnected. Trying to reconnect...");
        esp_wifi_connect();
    }else if(event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP){
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
        connected = 1;
    }
}


//init wifi
void wifi_connect_task(void *pvParameters){
    // init nvs
    ESP_ERROR_CHECK(nvs_flash_init());
    // init tcp/ip adapter
    ESP_ERROR_CHECK(esp_netif_init());
    // create default event loop
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    // init wifi config
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    // register wifi event handler
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, NULL));
    // start to connect
    ESP_ERROR_CHECK(esp_wifi_start());
    
    ESP_LOGI(TAG, "WiFi init finished.");
    
    // keep running
    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

coap_response_t message_handler(coap_session_t *session,
                                const coap_pdu_t *sent,
                                const coap_pdu_t *received,
                                const coap_mid_t mid)
{ 
    size_t len;
    const uint8_t *data_buf;
    size_t offset;
    size_t total;
    have_response = 1;
    coap_show_pdu(COAP_LOG_WARN,received);
    if(coap_get_data_large(received, &len, &data_buf, &offset, &total)){
        fwrite(data_buf, 1, len, stdout);
        fwrite("\n", 1, 1, stdout);
    }
    return COAP_RESPONSE_OK;
    
}

static void coap_client(void *pvParameters){
    // Wait for WiFi connection
    while(connected == 0){
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    
    coap_context_t *ctx = NULL;
    coap_session_t *session = NULL;
    coap_optlist_t *optlist = NULL;
    coap_address_t dst;
    coap_pdu_t *pdu = NULL;
    int result = EXIT_FAILURE;;
    int ret;
    // int res;
    unsigned int wait_ms;
    coap_uri_t uri;
    const char *coap_uri = COAP_URI;
    int is_mcast;
  #define BUFSIZE 100
    unsigned char scratch[BUFSIZE];

    // Initialize libcoap library
    coap_startup();
    // Set logging level
    coap_set_log_level(COAP_LOG_WARN);
    // Parse URI
    ret = coap_split_uri((const uint8_t *)coap_uri, strlen(coap_uri), &uri);
    if (ret != 0) {
        ESP_LOGE(TAG, "URI parsing failed");
        goto clean_up;
    }
    // Reslove address
    coap_addr_info_t *addr_info;
    addr_info = coap_resolve_address_info(&uri.host, COAP_PORT, COAP_PORT, COAP_PORT, COAP_PORT, AF_UNSPEC, 1 << uri.scheme, COAP_RESOLVE_TYPE_REMOTE);
    if (addr_info == NULL) {
        ESP_LOGE(TAG, "failed to resolve address");
        goto clean_up;
    }
    dst = addr_info->addr;
    coap_free_address_info(addr_info);
    is_mcast = coap_is_mcast(&dst);
    // Create context
    ctx = coap_new_context(NULL);
    if (!ctx) {
        ESP_LOGE(TAG, "coap_new_context() failed");
        goto clean_up;
    }
    // Set block mode
    coap_context_set_block_mode(ctx, COAP_BLOCK_USE_LIBCOAP | COAP_BLOCK_SINGLE_BODY);
    if(uri.scheme == COAP_URI_SCHEME_COAP){
        session = coap_new_client_session(ctx, NULL, &dst, COAP_PROTO_UDP);
    }else if(uri.scheme == COAP_URI_SCHEME_COAP_TCP){
        session = coap_new_client_session(ctx, NULL, &dst, COAP_PROTO_TCP);
    }
    if(!session){
        ESP_LOGE(TAG, "coap_new_client_session() failed");
        goto clean_up;
    }
    // CoAP register response handler
    coap_register_response_handler(ctx,message_handler);
    // Create PDU
    pdu = coap_pdu_init(is_mcast ? COAP_MESSAGE_NON : COAP_MESSAGE_CON, COAP_REQUEST_CODE_GET,coap_new_message_id(session),coap_session_max_pdu_size(session));
    if (!pdu) {
        ESP_LOGE(TAG, "coap_pdu_init() failed");
        goto clean_up;
    }
    // Add option to PDU
    ret = coap_uri_into_options(&uri, &dst, &optlist, 1, scratch, BUFSIZE);
    if(ret){
        ESP_LOGE(TAG, "coap_uri_into_options() failed");
        goto clean_up;
    }
    if(optlist){
        ret = coap_add_optlist_pdu(pdu, &optlist);
        if(ret != 1){
            ESP_LOGE(TAG, "coap_add_optlist_pdu() failed");
            goto clean_up;
        }
    }

    coap_show_pdu(COAP_LOG_WARN, pdu);
    
    // Send PDU
    ret = coap_send(session, pdu);
    if(ret == COAP_INVALID_MID){
        ESP_LOGE(TAG, "coap_send() failed");
        goto clean_up;
    }
    // Wait for response
    wait_ms = (coap_session_get_default_leisure(session).integer_part + 1) * 1000;
    while(have_response == 0 || is_mcast){
        ret = coap_io_process(ctx, 1000);
        if(ret >= 0){
            if(wait_ms > 0){
                if((unsigned)ret >= wait_ms){
                    fprintf(stdout, "timeout\n");
                    break;
                }else{
                    wait_ms -= ret;
                }
            }
        }
    }

    result = EXIT_SUCCESS;



clean_up:
    if (optlist) {
        coap_delete_optlist(optlist);
    }
    if (session) {
        coap_session_release(session);
    }
    if (ctx) {
        coap_free_context(ctx);
    }
    coap_cleanup();
    ESP_LOGI(TAG, "Finished");
    vTaskDelete(NULL);

}

void app_main(void)
{
    xTaskCreate(
        wifi_connect_task,   // function to implement the task
        "wifi_connect_task", // name of the task
        4096,                // stack size in words
        NULL,                // task input parameter
        configMAX_PRIORITIES-1, // priority of the task
        &wifi_task_handle   // task handle
    );
    xTaskCreate(
        coap_client,   // function to implement the task
        "coap_client", // name of the task
        4096,                // stack size in words
        NULL,                // task input parameter
        configMAX_PRIORITIES-2, // priority of the task
        NULL   // task handle
    );
}