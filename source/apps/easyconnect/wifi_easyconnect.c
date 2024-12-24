#include "wifi_base.h"
#include "wifi_events.h"
#include "wifi_hal.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "wifi_hal.h"
#include "wifi_ctrl.h"
#include "wifi_util.h"
#include "wifi_analytics.h"

/**
 * @brief Is this IE a Wi-Fi Alliance CCE IE?
 *
 * @param ie The information element in question
 * @param ie_len Length of the information element in bytes.
 * @return true if this IE is a WFA CCE IE, otherwise false
 */
static bool is_cce_ie(const uint8_t *const ie, size_t ie_len)
{
    static const uint8_t OUI_WFA[3] = { 0x50, 0x6F, 0x9A };
    static const uint8_t CCE_CONSTANT = 0x1E;
    if (ie_len < 4)
        return false;
    return memcmp(ie, OUI_WFA, sizeof(OUI_WFA)) == 0 && *(ie + 3) == CCE_CONSTANT;
}

static void handle_wifi_event_scan_results(wifi_app_t *app, void *data)
{
    int i;
    scan_results_t *scan_results = (scan_results_t *)data;
    if (!scan_results) {
        wifi_util_dbg_print(WIFI_EASYCONNECT, "%s:%d: NULL scan data!\n", __func__, __LINE__);
        return;
    }
    for (i = 0; i < scan_results->num; i++) {
        wifi_bss_info_t *bss_info = &scan_results->bss[i];
        if (!bss_info || !bss_info->ie || bss_info->ie_len == 0) {
            wifi_util_dbg_print(WIFI_EASYCONNECT, "%s:%d: Invalid BSS info! #%d\n", __func__,
                __LINE__, i);
            continue;
        }
        uint8_t *ie_pos = bss_info->ie;
        size_t ie_len_remaining = bss_info->ie_len;
        while (ie_len_remaining > 2) {
            uint8_t id = ie_pos[0];
            uint8_t ie_len = ie_pos[1];
            if (ie_len + 2 > ie_len_remaining)
                break;
            // 0xdd == Vendor IE
            if (id == 0xdd && is_cce_ie(ie_pos + 2, ie_len)) {
                wifi_util_dbg_print(WIFI_EASYCONNECT,
                    "%s:%d: BSS %s Beacon and/or Probe Response contains WFA CCE IE!\n", __func__,
                    __LINE__, bss_info->ssid);
                // TODO: publish wifi_bss_info_t to appropriate bus path
            }
            // next IE
            ie_len_remaining -= (ie_len + 2);
            ie_pos += (ie_len + 2);
        }
    }
}

static void handle_hal_event(wifi_app_t *app, wifi_event_subtype_t event_subtype, void *data)
{
    switch (event_subtype) {
    case wifi_event_scan_results:
        handle_wifi_event_scan_results(app, data);
        break;
    default:
        wifi_util_dbg_print(WIFI_EASYCONNECT, "%s:%d: unhandled event sub_type=%d\n", __func__,
            __LINE__, event_subtype);
        break;
    }
}

int easyconnect_event(wifi_app_t *app, wifi_event_t *event)
{
    switch (event->event_type) {
    case wifi_event_type_hal_ind:
        handle_hal_event(app, event->sub_type, event->u.ec_data);
        break;
    default:
        wifi_util_dbg_print(WIFI_EASYCONNECT, "%s:%d: unhandled event_type=%d\n", __func__,
            __LINE__, event->event_type);
        break;
    }
}

int easyconnect_init(wifi_app_t *app, unsigned int create_flags)
{
    wifi_util_dbg_print(WIFI_EASYCONNECT, "%s called.", __func__);
    return 0;
}

int easyconnect_deinit(wifi_app_t *app)
{
    wifi_util_dbg_print(WIFI_EASYCONNECT, "%s called.", __func__);
    return 0;
}
