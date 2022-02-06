//====================================================
//
// program name   : tinyUDP.cpp
// date/author    : 2022/01/25  @chrmlinux03
// update/author  : 2022/01/27  @chrmlinux03
// update/author  : 2022/02/06  @chrmlinux03 v0.0.2
// version        : 0.0.2
//
//====================================================

#include "tinyUDP.h"

tinyUDP::tinyUDP(uint16_t mode)
{
  _udpMode = mode;
}

//======================================
//
// begin
//--------------------------------------
// Create the required buffer size and start class.
// The buffer to be secured should be 80Kbyte or less.
//
//======================================
uint16_t tinyUDP::begin(uint32_t size)
{
  int16_t rtn = 0;

  WiFi.disconnect(true, true);

  if (_udpMode == TUDP_RECV) {
    WiFi.softAPConfig(_serverIp, _gwIp, _subnet);
    WiFi.softAP(_ssid, _pass);
    if (_result) {
      Serial.print("Recv Ready ! ");
      Serial.print(_serverIp); Serial.print(":");
      Serial.print(_serverPort); Serial.println();
    }
  } else {
    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid, _pass);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("WiFi Failed");
      while (1) {
        delay(1000);
      }
    }
    if (_result) {
      Serial.print("Send Ready ! ");
      Serial.print(WiFi.localIP()); Serial.print(" -> ");
      Serial.println(_serverIp);
    }
  }
  _udp.begin(_serverPort);

  _arySize = size;
  _ary = new uint8_t[_arySize];
  memset(_ary, 0x0, _arySize);

  return 0;
}

//======================================
//
// end
//--------------------------------------
// Close WiFI, return the memory and exit Class.
//
//======================================
uint16_t tinyUDP::end(void)
{
  free(_ary);
  _ary = NULL;
  WiFi.disconnect(true, true);
  return 0;
}

//======================================
//
// dump
//--------------------------------------
// Do a hexadecimal dump.
//
//======================================
void tinyUDP::dump(uint8_t* dt, uint32_t len)
{
  static int cnt = 0;
  for (int i = 0; i < len; i++) {
    Serial.printf("%02x ", dt[i]);
    cnt = (cnt + 1) % 16;
    if (cnt == 0) Serial.println();
  }
}

//======================================
//
// getArySize
//--------------------------------------
// The size of the obtained buffer will be returned.
//
//======================================
uint32_t tinyUDP::getArySize(void)
{
  return _arySize;
}

//======================================
//
// getRecvSize
//--------------------------------------
// Gets the length of the buffer actually received.
//
//======================================
uint32_t tinyUDP::getRecvSize(void)
{
  return _recvSize;
}

//======================================
//
// setResult
//--------------------------------------
// Debug print set.
//
//======================================
uint16_t tinyUDP::setResult(uint16_t result)
{
  _result = result;
  return 0;
}

//======================================
//
// isRecv
//--------------------------------------
// Did you get the data?
//
//======================================
bool tinyUDP::isRecv(void)
{
  bool rtn = _isRecv;
  _isRecv = false;
  return rtn;
}

//======================================
//
// put
//--------------------------------------
// Pass user data to Class.
//
//======================================
uint16_t tinyUDP::put(uint8_t *dt, uint16_t cnt)
{
  int16_t rtn = 0;
  memcpy(_ary, dt, cnt);
  return rtn;
}

//======================================
//
// get
//--------------------------------------
// Get the data received from Class.
//
//======================================
uint8_t* tinyUDP::get(void)
{
  return (uint8_t*)_ary;
}

//======================================
//
// recv
//--------------------------------------
// Receive data.
//
//======================================
uint16_t tinyUDP::recv(void)
{
  uint8_t buf[UDP_MAX_BYTE] = {0};
  static uint32_t len = 0;

  uint16_t sz = _udp.parsePacket();
  if (sz) {
    int16_t cnt = _udp.read(buf, sz);
    if (cnt == 5) {
      if (buf[0] == STX) {
        len = (buf[1] << 24) | (buf[2] << 16) | (buf[3] << 8) | buf[4];
        _recvSize = 0;
        _isRecv = false;
      } else {
        _isRecv = true;
        if (_result) {
          Serial.printf("%d\n", _recvSize);
        }
      }
    } else {
      memcpy(&_ary[_recvSize], buf, cnt);
			if (_recvSize < len) _recvSize += cnt;
    }
  }
  delay(1);
  return 0;
}

//======================================
//
// send
//--------------------------------------
// Send data.
//
//======================================
uint16_t tinyUDP::send(bool flag)
{
  if (_sendEnable == false) return 1;

  uint8_t head[5] = {0};

  _sendEnable = false;

  _udp.beginPacket(flag ? _broadcast : _serverIp, _serverPort);
  memset(head, 0x0, sizeof(head));
  head[0] = STX;
  head[1] = (_arySize & 0xFF000000) >> 24;
  head[2] = (_arySize & 0x00FF0000) >> 16;
  head[3] = (_arySize & 0x0000FF00) >>  8;
  head[4] = (_arySize & 0x000000FF);
  _udp.write(head, sizeof(head));
  _udp.endPacket();

  _udp.beginPacket(flag ? _broadcast : _serverIp, _serverPort);
  _udp.write(_ary, _arySize);
  _udp.endPacket();

  _udp.beginPacket(flag ? _broadcast : _serverIp, _serverPort);
  memset(head, 0x0, sizeof(head));
  head[0] = ETX;
  _udp.write(head, sizeof(head));
  _udp.endPacket();

  if (_result) {
    Serial.printf("%d\n", _arySize);
  }
  _sendEnable = true;

  delay(1);
  return 0;
}


//======================================
//
// update
//--------------------------------------
// Update the state of the Class.
//
//======================================
uint16_t tinyUDP::update(bool flag = false)
{
  if (!_udpMode) {
    recv();
  } else {
    send(flag);
  }
  return 0;
}

//======================================
//
// delay
//--------------------------------------
// Delay with through.
//
//======================================
bool tinyUDP::delay(uint16_t msec)
{
  bool rtn = false;
  static uint32_t _tm = millis();
  if ((millis() - _tm) > msec) {
    _tm = millis();
    rtn = true;
  }
  return rtn;
}
