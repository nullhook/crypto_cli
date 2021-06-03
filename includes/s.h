#include <cstdint>

#define NUM_FANS "FNum"

// enum {
//   kSMCSuccess = 0,
//   kSMCError   = 1
// };

typedef enum {
  kSMCSuccess     = 0,
  kSMCError       = 1,
  kSMCKeyNotFound = 0x84
} kSMC_t;

enum {
 kSMCUserClientOpen = 0,
 kSMCUserClientClose = 1,
 kSMCHandleYPCEvent = 2,
 kSMCReadKey = 5,
 kSMCWriteKey = 6,
 kSMCGetKeyCount = 7,
 kSMCGetKeyFromIndex = 8,
 kSMCGetKeyInfo = 9
 };

typedef struct SMCVersion
 {
 unsigned char major;
 unsigned char minor;
 unsigned char build;
 unsigned char reserved;
 unsigned short release;

} SMCVersion;

typedef struct SMCPLimitData
 {
 uint16_t version;
 uint16_t length;
 uint32_t cpuPLimit;
 uint32_t gpuPLimit;
 uint32_t memPLimit;

} SMCPLimitData;

typedef struct SMCKeyInfoData
 {
 IOByteCount dataSize;
 uint32_t dataType;
 uint8_t dataAttributes;
 uint8_t  data[32];
} SMCKeyInfoData;

typedef struct {
  uint8_t  data[32];
  uint32_t dataType;
  uint32_t dataSize;
  kSMC_t   kSMC;
} smc_return_t;

typedef struct {
 uint32_t key;
 SMCVersion vers;
 SMCPLimitData pLimitData;
 SMCKeyInfoData keyInfo;
 uint8_t result;
 uint8_t status;
 uint8_t data8;
 uint32_t data32;
 uint8_t bytes[32];
 } SMCParamStruct;