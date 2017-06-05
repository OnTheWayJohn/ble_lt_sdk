
void kb_i2c_sim_init();
void  i2c_start(void);
void  i2c_stop(void);

bool I2CWaitAck(void);
bool I2C_ByteWrite(u8 pBuffer, u16 WAddr);
bool I2C_PageWrite(u16 pBuffer, u16 WAddr, u8 n);

bool I2C_BufferRead(u16 *pBuffer, u16 RAddr, u16 n);

