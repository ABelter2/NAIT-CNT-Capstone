/////////////////////////////////////////////
// LSM303DLHC - eCompass (3D Accelerometer + 3D Magnetometer)
// 7-bit device address 0x19 for accelerometer
// 7-bit device address 0x1E for magnetometer
// requires I2C bus to be initialized (400KHz OK)
/////////////////////////////////////////////

// I2C library uses 8-bit addresses
#define LSM303_ADDR_ACC 0x32
#define LSM303_ADDR_MAG 0x3C

// init simple
int LSM303_Init (void);

// pull status register from device
unsigned char LSM303_GetAccStatus (void);

// pull status register from device
unsigned char LSM303_GetMagStatus (void);

// pull X/Y/X data for accelerometer (requires target length of 6 bytes)
// XH XL YH YL ZH ZL
int LSM303_GetXYZ (unsigned char * pTarget);

// appears to be an uncalibrated (relative) temp sensor
// yank temperature from device (return -300 if error)
int LSM303_GetTemp (void);