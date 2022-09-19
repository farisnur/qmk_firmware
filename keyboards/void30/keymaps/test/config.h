/* i2c config for oleds */
#define I2C_DRIVER        I2CD1
#define I2C1_SCL_PIN      B8
#define I2C1_SDA_PIN      B9
#define I2C1_SCL_PAL_MODE 4
#define I2C1_SDA_PAL_MODE 4
// #define I2C1_CLOCK_SPEED  400000
/* For Legacy Compatibility: */
#define I2C1_SCL 8
#define I2C1_SDA 9
#define I2C1_CLOCK_SPEED  400000
#define I2C1_DUTY_CYCLE FAST_DUTY_CYCLE_2
#define OLED_DISPLAY_ADDRESS 0x3C