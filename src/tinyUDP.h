//====================================================
//
// program name   : tinyUDP.h
// date/author    : 2022/01/25  @chrmlinux03
// update/author  : 2022/01/27  @chrmlinux03
// version        : 0.0.2
//
//====================================================

#ifndef _tinyUDP_H_
#define _tinyUDP_H_

#include "arduino.h"
#include "WiFi.h"
#include "WiFiUdp.h"

#define UDP_MAX_BYTE (1460)

enum {TUDP_RECV = 0, TUDP_SEND};
enum {TUDP_UNICAST = 0, TUDP_BLOADCAST};
enum {STX = 0x02, ETX};

class tinyUDP {

  public:

    tinyUDP(uint16_t);
    uint16_t begin(uint32_t);         // library begin & send/recv arrey size
    uint16_t end(void);               // library end
    uint16_t recv(void);              // udp data recv
    uint16_t send(bool);              // udp data send
    uint16_t update(bool);            // udp data update
    uint8_t* get(void);               // user data get
    uint16_t put(uint8_t*, uint16_t); // user data set
    uint16_t setResult(uint16_t);     // debug display on/off set
    bool delay(uint16_t);             // library delay
    bool isRecv(void);                // is data recv ?
    uint32_t getRecvSize(void);       // get now recv data size
    uint32_t getArySize(void);				// get real array size
    void dump(uint8_t*, uint32_t);		// dump of data

  private:

    WiFiUDP _udp;
    uint16_t _udpMode = TUDP_RECV;
    uint16_t _castMode = TUDP_UNICAST;

    char *_ssid = "tinyUDP";
    char *_pass = "#tinyUDP#";
    IPAddress     _gwIp  = IPAddress(192, 168,   4,   1);
    IPAddress _serverIp  = IPAddress(192, 168,   4,   2);
    IPAddress   _subnet  = IPAddress(255, 255, 255,   0);
    IPAddress _broadcast = IPAddress(192, 168,   4, 255);
    uint16_t _serverPort = 50002;
    uint8_t *_ary = NULL;
    uint32_t _arySize = 0;
    bool _sendEnable = true;
    bool _isRecv = false;
    uint16_t _result = 0;
    uint32_t _recvSize = 0;

};

#endif
