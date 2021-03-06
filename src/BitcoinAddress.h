#ifndef BitcoinAddress_h
#define BitcoinAddress_h

#define MAIN_NET_VERSION 0x00
#define TEST_NET_VERSION 0x6F

#define KEY_ENCODED				0x01
#define KEY_WIF  				0x02
#define KEY_PRIVATE	 			0x03
#define KEY_COMPRESSED_PUBLIC	0x04
#define KEY_PUBLIC 				0x05
#define KEY_SCRIPT_HASH			0x06

#define SIZE_OF_RIPE_HASH 0x14

extern "C" {

  static int RNG(uint8_t *dest, unsigned size) {
    // Use the least-significant bits from the ADC for an unconnected pin (or connected to a source of
    // random noise). This can take a long time to generate random data if the result of analogRead(0)
    // doesn't change very frequently.
    while (size) {
      uint8_t val = 0;
      for (unsigned i = 0; i < 8; ++i) {
        int init = analogRead(0);
        int count = 0;
        while (analogRead(0) == init) {
          ++count;
        }

        if (count == 0) {
          val = (val << 1) | (init & 0x01);
        } else {
          val = (val << 1) | (count & 0x01);
        }
      }
      *dest = val;
      ++dest;
      --size;
    }
    // NOTE: it would be a good idea to hash the resulting random data using SHA-256 or similar.
    return 1;
  }

}  // extern "C"

class BitcoinAddress
{
public:
	BitcoinAddress(bool=false);
	BitcoinAddress(const char *,uint8_t);
	BitcoinAddress(uint8_t *,uint8_t);
	void getPrivateKey(uint8_t *);
	void getPrivateKey(const char *);
	void getPublicKey(uint8_t *);
	void getPublicKey(const char *);
	void getCompressedPublicKey(uint8_t *);
	void getCompressedPublicKey(const char *);
	void getWif(uint8_t *);
	void getWif(const char *);
	void getEncoded(uint8_t *);
	void getEncoded(char *);
	bool isTracked();
	uint64_t getBalance();
	uint64_t getConfirmedBalance();
	uint64_t getUnconfirmedBalance();
	void getScriptPubKey(uint8_t *,uint8_t);
	uint8_t getAddressType();
	bool operator==(BitcoinAddress& );

private:
	void init();
	void getStatus(char *);
	bool gotAddress();
	void setTracked();
	void setConfirmedBalance(uint32_t);
	void setUnconfirmedBalance(uint32_t);
	void setGotAddress();
	void setAddressStatus(char *);
	void resetGotAddress();
	void calculateAddress(uint8_t );
	uint8_t privateKey[32];
	uint8_t publicKey[65];
	uint8_t compPubKey[33];
	char address[36];
	char status[65];
	bool tracked;
	bool gotAddressRef;
	uint32_t confirmedBalance;
	uint32_t unconfirmedBalance;
	const struct uECC_Curve_t * curve;
	friend class KoynClass;
	friend class BitcoinTransaction;
};


#endif
