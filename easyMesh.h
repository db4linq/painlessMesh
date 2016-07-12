#ifndef   _EASY_MESH_H_
#define   _EASY_MESH_H_

#include <Arduino.h>
#include <SimpleList.h>

extern "C" {
#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "user_config.h"
#include "user_interface.h"
#include "uart.h"

#include "c_types.h"
#include "espconn.h"
#include "mem.h"
}

#include "meshWebServer.h"
#include "meshWebSocket.h"

#define MESH_PREFIX       "mesh"
#define MESH_PASSWORD     "bootyboo"
#define MESH_PORT         4444

enum scanStatus {
  IDLE       = 0,
  SCANNING   = 1
};

struct meshConnection_t {
  espconn *esp_conn;
  uint32_t  chipId;
};


class easyMesh {
  public:
    void init( void );
    void update( void );
    bool sendMessage( uint32_t finalDestId, String &msg );  //must be accessable from callback
    bool sendHandshake( meshConnection_t *connection );  //must be accessable from callback
    void tcpConnect( void );  //must be accessable from callback
    void manageStation( void );
        
    // callbacks
    static void wifiEventCb(System_Event_t *event);
    static void meshConnectedCb(void *arg);
    static void meshSentCb(void *arg);
    static void meshRecvCb(void *arg, char *packageData, unsigned short length);
    static void meshDisconCb(void *arg);
    static void meshReconCb(void *arg, sint8 err);
    static void stationScanCb(void *arg, STATUS status);

    static bool findBestAP( char *buffer );

    uint8_t     scanStatus = IDLE;
    SimpleList<bss_info>            _meshAPs;

  protected:
    void apInit( void );
    void tcpServerInit(espconn &serverConn, esp_tcp &serverTcp, espconn_connect_callback connectCb, uint32 port);
    bool stationConnect( void );
    bool stationScan( void );


    meshConnection_t* findConnection( uint32_t chipId );
    String buildMeshPackage( uint32_t localDestId, uint32_t finalDestId, String &msg );
    bool sendPackage( meshConnection_t *connection, String &package );

    uint32_t    _chipId;
    String      _mySSID;

    os_timer_t  _scanTimer;

    espconn     _meshServerConn;
    esp_tcp     _meshServerTcp;

    espconn     _webServerConn;
    esp_tcp     _webServerTcp;

    espconn     _webSocketConn;
    esp_tcp     _webSocketTcp;

    SimpleList<meshConnection_t>    _connections;
};

#endif //   _EASY_MESH_H_
