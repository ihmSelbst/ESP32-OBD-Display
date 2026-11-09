#pragma once

#include <BLEClient.h>

template <typename Tag, typename Tag::type Member>
struct BleClientPrivateAccess {
    friend typename Tag::type bleClientMember(Tag) { return Member; }
};

struct BleClientLastErrTag {
    typedef int BLEClient::*type;
    friend type bleClientMember(BleClientLastErrTag);
};

struct BleClientConnectTimeoutTag {
    typedef int32_t BLEClient::*type;
    friend type bleClientMember(BleClientConnectTimeoutTag);
};

template struct BleClientPrivateAccess<BleClientLastErrTag, &BLEClient::m_lastErr>;
template struct BleClientPrivateAccess<BleClientConnectTimeoutTag, &BLEClient::m_connectTimeout>;

static inline int bleClientLastError(BLEClient* client) {
    return client ? client->*bleClientMember(BleClientLastErrTag()) : 0;
}

static inline void bleClientSetConnectTimeout(BLEClient* client, int32_t timeoutMs) {
    if (client) client->*bleClientMember(BleClientConnectTimeoutTag()) = timeoutMs;
}

struct BleClientIsConnectedTag {
    typedef bool BLEClient::*type;
    friend type bleClientMember(BleClientIsConnectedTag);
};
template struct BleClientPrivateAccess<BleClientIsConnectedTag, &BLEClient::m_isConnected>;

static inline bool bleClientLinkExists(BLEClient* client) {
    if (!client) return false;
    struct ble_gap_conn_desc desc;
    return ble_gap_conn_find(client->getConnId(), &desc) == 0;
}

static inline bool bleClientAdoptEstablishedLink(BLEClient* client) {
    if (!bleClientLinkExists(client)) return false;
    client->*bleClientMember(BleClientIsConnectedTag()) = true;
    BLEDevice::addPeerDevice(client, true, client->m_gattAppId);
    return true;
}

static inline void bleClientDropOrphanLink(BLEClient* client) {
    if (!client || client->isConnected() || !bleClientLinkExists(client)) return;
    ble_gap_terminate(client->getConnId(), BLE_ERR_REM_USER_CONN_TERM);
    for (uint8_t i = 0; i < 75 && bleClientLinkExists(client); i++) delay(20);
}
