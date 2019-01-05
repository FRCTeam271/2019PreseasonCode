#ifndef TEAM271_I2C_H_
#define TEAM271_I2C_H_

#if defined( T271_I2C_ENABLE )

typedef enum
{
   TEAM271_I2C_PORT_ONBOARD,
   TEAM271_I2C_PORT_MXP,

   TEAM271_I2C_PORT_COUNT
}TEAM271_I2C_PORT;

class Team271I2C
{
protected:
   /*
    *
    * Variables
    *
    */
   TEAM271_I2C_PORT Port_ = TEAM271_I2C_PORT_ONBOARD;

   uint8_t DeviceAddress_ = 0;

   int32_t OnBoardHandle_ = -1;
   int32_t MXPHandle_ = -1;

public:

   /*
    *
    * Constructor/Destructor
    *
    */
   Team271I2C( const TEAM271_I2C_PORT argPort,
               const uint32_t argDeviceAddress );

   virtual ~Team271I2C( void );

   /*
    *
    * Output Functions
    *
    */
   void Transaction( const uint8_t* const argTXBuf,
                     const uint32_t argTXCount,
                     uint8_t* const argRXBuf,
                     const uint32_t argRXCount ) const;

   void Read( const uint8_t argRegister,
              uint8_t* const argBuf,
              const uint32_t argCount ) const;

   void Read( uint8_t* const argBuf,
              const uint32_t argCount ) const;

   void Write( uint8_t* const argBuf,
               const uint32_t argCount ) const;

   void Write( const uint8_t argRegister,
               const uint8_t argData ) const;
};

#endif

#endif /* TEAM271_MOTOR_H_ */
