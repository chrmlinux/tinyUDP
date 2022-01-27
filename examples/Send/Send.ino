#include "tinyUDP.h"

#define BUF_SIZE (1024 * 80)
tinyUDP tudp(TUDP_SEND);

void setup()
{
  Serial.begin( 115200 );
  while (!Serial);
  tudp.begin(BUF_SIZE);
  tudp.setResult(0);
}

void loop()
{
  static uint8_t c = 0x0;

  if (tudp.delay(10)) {

    uint8_t *buf = tudp.get();
    uint32_t bufSize = tudp.getArySize();

    memset(buf, c, bufSize);
    tudp.put(buf, bufSize);
    Serial.printf("send : 0x%02x, size=%d\n", c, bufSize);
    c = (c + 1) % 256;

    tudp.update(false);

  }
}
