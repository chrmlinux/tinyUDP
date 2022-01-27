#include "tinyUDP.h"

#define BUF_SIZE (1024 * 80)
tinyUDP tudp(TUDP_RECV);

void setup()
{
  Serial.begin( 115200 );
  while (!Serial);
  tudp.begin(BUF_SIZE);
  tudp.setResult(0);
}

void loop()
{
  if (tudp.isRecv()) {
    uint8_t *buf = tudp.get();
    uint32_t recvSize = tudp.getRecvSize();
    Serial.printf("recv : 0x%02x, size=%d\n", buf[0], recvSize);
    tudp.dump(buf, 128);
  }
  tudp.update(false);
}
