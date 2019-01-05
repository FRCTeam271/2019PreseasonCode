/*
 *    FRC Team 271's 2019 Pre Season Code
 *    Copyright (C) 2019  FRC Team 271
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef TEAM271_EM7180_H_
#define TEAM271_EM7180_H_

#if defined(T271_EM7180_ENABLE)

typedef enum : uint8_t
{
   TEAM271_IMU_STATUS_EEPROM_NONE = 0,

   TEAM271_IMU_STATUS_EEPROM_DETECTED = ( 1 << 0 ),

   TEAM271_IMU_STATUS_EEPROM_UPLOAD_DONE = ( 1 << 1 ),

   TEAM271_IMU_STATUS_EEPROM_UPLOAD_ERROR = ( 1 << 2 ),

   TEAM271_IMU_STATUS_EEPROM_IDLE = ( 1 << 3 ),

   TEAM271_IMU_STATUS_EEPROM_NO_EEPROM = ( 1 << 4 ),

   TEAM271_IMU_STATUS_EEPROM_COUNT
}TEAM271_IMU_STATUS_EEPROM;

typedef enum : uint8_t
{
   TEAM271_IMU_EM7180_STATE_RESET = 0,

   TEAM271_IMU_EM7180_STATE_DETECT_EEPROM,

   TEAM271_IMU_EM7180_STATE_CANCEL_PASSTHROUGH,
   TEAM271_IMU_EM7180_STATE_CANCEL_PASSTHROUGH_WAIT,

   TEAM271_IMU_EM7180_STATE_RESTART_ALGORITHM,
   TEAM271_IMU_EM7180_STATE_RESTART_ALGORITHM_WAIT,

   TEAM271_IMU_EM7180_STATE_INITIALIZE_REGISTERS,

   TEAM271_IMU_EM7180_STATE_DISABLE_STILLNESS,

   TEAM271_IMU_EM7180_STATE_NORMAL,

   TEAM271_IMU_EM7180_STATE_COUNT
}TEAM271_IMU_EM7180_STATE;

typedef enum : uint8_t
{
   TEAM271_IMU_EM7180_SETTINGS_NONE = 0,

   TEAM271_IMU_EM7180_SETTINGS_6DOF = ( 1 << 0 ),

   TEAM271_IMU_EM7180_SETTINGS_9DOF = ( 1 << 1 ),

   TEAM271_IMU_EM7180_SETTINGS_DISABLE_GYRO_WHEN_STILL = ( 1 << 2 ),

   TEAM271_IMU_EM7180_SETTINGS_COUNT
}TEAM271_IMU_EM7180_SETTINGS;

/*
 *
 * Shared Network Data
 *
 */
class t271_sensor_em7180_data_index : public t271_sensor_imu_data_index
{
public:
   typedef enum : uint8_t
   {
      T271_SENSOR_EM7180_DATA_STATE = t271_sensor_imu_data_index::T271_DATA_COUNT,

      T271_DATA_COUNT,
   } t271_sensor_em7180_data_index_t;
};

class Team271EM7180 : public Team271IMU
{
protected:
   /*
    *
    * Constants
    *
    */
   // EM7180 SENtral register map
   // see http://www.emdeveloper.com/downloads/7180/EMSentral_EM7180_Register_Map_v1_3.pdf
   static const uint8_t QX = 0x00;// this is a 32-bit normalized floating point number read from registers = 0x00-03
   static const uint8_t QY = 0x04;// this is a 32-bit normalized floating point number read from registers = 0x04-07
   static const uint8_t QZ = 0x08;// this is a 32-bit normalized floating point number read from registers = 0x08-0B
   static const uint8_t QW = 0x0C;// this is a 32-bit normalized floating point number read from registers = 0x0C-0F
   static const uint8_t QTIME = 0x10;// this is a 16-bit unsigned integer read from registers = 0x10-11
   static const uint8_t MX = 0x12;// int16_t from registers = 0x12-13
   static const uint8_t MY = 0x14;// int16_t from registers = 0x14-15
   static const uint8_t MZ = 0x16;// int16_t from registers = 0x16-17
   static const uint8_t MTIME = 0x18;// uint16_t from registers = 0x18-19
   static const uint8_t AX = 0x1A;// int16_t from registers = 0x1A-1B
   static const uint8_t AY = 0x1C;// int16_t from registers = 0x1C-1D
   static const uint8_t AZ = 0x1E;// int16_t from registers = 0x1E-1F
   static const uint8_t ATIME = 0x20;// uint16_t from registers = 0x20-21
   static const uint8_t GX = 0x22;// int16_t from registers = 0x22-23
   static const uint8_t GY = 0x24;// int16_t from registers = 0x24-25
   static const uint8_t GZ = 0x26;// int16_t from registers = 0x26-27
   static const uint8_t GTIME = 0x28;// uint16_t from registers = 0x28-29
   static const uint8_t Baro = 0x2A;// start of two-byte MS5637 pressure data, 16-bit signed interger
   static const uint8_t BaroTIME = 0x2C;// start of two-byte MS5637 pressure timestamp, 16-bit unsigned
   static const uint8_t Temp = 0x2E;// start of two-byte MS5637 temperature data, 16-bit signed interger
   static const uint8_t TempTIME = 0x30;// start of two-byte MS5637 temperature timestamp, 16-bit unsigned
   static const uint8_t QRateDivisor = 0x32;// uint8_t
   static const uint8_t EnableEvents = 0x33;
   static const uint8_t HostControl = 0x34;
   static const uint8_t EventStatus = 0x35;
   static const uint8_t SensorStatus = 0x36;
   static const uint8_t SentralStatus = 0x37;
   static const uint8_t AlgorithmStatus = 0x38;
   static const uint8_t FeatureFlags = 0x39;
   static const uint8_t ParamAcknowledge = 0x3A;
   static const uint8_t SavedParamByte0 = 0x3B;
   static const uint8_t SavedParamByte1 = 0x3C;
   static const uint8_t SavedParamByte2 = 0x3D;
   static const uint8_t SavedParamByte3 = 0x3E;
   static const uint8_t ActualMagRate = 0x45;
   static const uint8_t ActualAccelRate = 0x46;
   static const uint8_t ActualGyroRate = 0x47;
   static const uint8_t ActualBaroRate = 0x48;
   static const uint8_t ActualTempRate = 0x49;
   static const uint8_t ErrorRegister = 0x50;
   static const uint8_t AlgorithmControl = 0x54;
   static const uint8_t MagRate = 0x55;
   static const uint8_t AccelRate = 0x56;
   static const uint8_t GyroRate = 0x57;
   static const uint8_t BaroRate = 0x58;
   static const uint8_t TempRate = 0x59;
   static const uint8_t LoadParamByte0 = 0x60;
   static const uint8_t LoadParamByte1 = 0x61;
   static const uint8_t LoadParamByte2 = 0x62;
   static const uint8_t LoadParamByte3 = 0x63;
   static const uint8_t ParamRequest = 0x64;
   static const uint8_t ROMVersion1 = 0x70;
   static const uint8_t ROMVersion2 = 0x71;
   static const uint8_t RAMVersion1 = 0x72;
   static const uint8_t RAMVersion2 = 0x73;
   static const uint8_t ProductID = 0x90;
   static const uint8_t RevisionID = 0x91;
   static const uint8_t RunStatus = 0x92;
   static const uint8_t UploadAddress = 0x94;// uint16_t registers = 0x94 (MSB)-5(LSB)
   static const uint8_t UploadData = 0x96;
   static const uint8_t CRCHost = 0x97;// uint32_t from registers = 0x97-9A
   static const uint8_t ResetRequest = 0x9B;
   static const uint8_t PassThruStatus = 0x9E;
   static const uint8_t PassThruControl = 0xA0;
   static const uint8_t ACC_LPF_BW = 0x5B;//Register GP36
   static const uint8_t GYRO_LPF_BW = 0x5C;//Register GP37
   static const uint8_t BARO_LPF_BW = 0x5D;//Register GP38
   static const uint8_t GP36 = 0x5B;
   static const uint8_t GP37 = 0x5C;
   static const uint8_t GP38 = 0x5D;
   static const uint8_t GP39 = 0x5E;
   static const uint8_t GP40 = 0x5F;
   static const uint8_t GP50 = 0x69;
   static const uint8_t GP51 = 0x6A;
   static const uint8_t GP52 = 0x6B;
   static const uint8_t GP53 = 0x6C;
   static const uint8_t GP54 = 0x6D;
   static const uint8_t GP55 = 0x6E;
   static const uint8_t GP56 = 0x6F;

   static const uint8_t ADDRESS = 0x28;// Address of the EM7180 SENtral sensor hub

   static const uint8_t TEMP_OUT_H = 0x41;
   static const uint8_t TEMP_OUT_L = 0x42;

   /*
    *
    * Variables
    *
    */
   atomic<TEAM271_IMU_EM7180_STATE> EM7180State_ = { TEAM271_IMU_EM7180_STATE_RESET };

   uint8_t errorStatus = 0;

   /*
    * Sensor
    */
   atomic<uint8_t> ThreadSensorConnected_ = { 0 };
   atomic<uint8_t> ThreadSensorInitialized_ = { 0 };
   atomic<uint8_t> ThreadSensorError_ = { 0 };

   atomic<float> ThreadRoll_ = { 0 };
   atomic<float> ThreadPitch_ = { 0 };
   atomic<float> ThreadYaw_ = { 0 };
   atomic<float> ThreadDeclination_ = { 0 };
   atomic<float> ThreadYawZeroOffset_ = { 0 };

   atomic<float> ThreadQW_ = { 0 };
   atomic<float> ThreadQX_ = { 0 };
   atomic<float> ThreadQY_ = { 0 };
   atomic<float> ThreadQZ_ = { 0 };

   atomic<float> ThreadAX_ = { 0 };
   atomic<float> ThreadAY_ = { 0 };
   atomic<float> ThreadAZ_ = { 0 };

   atomic<float> ThreadGX_ = { 0 };
   atomic<float> ThreadGY_ = { 0 };
   atomic<float> ThreadGZ_ = { 0 };

   uint8_t _eventStatus = 0;

   void readThreeAxis( uint8_t regx,
                       double& x,
                       double& y,
                       double& z,
                       double scale);

public:
   /*
    *
    * Constructor/Destructor
    *
    */
   Team271EM7180( Team271Base* const argParent );

   virtual ~Team271EM7180( void );

   /*
    *
    * Getters
    *
    */
   const string GetClassName( void ) const override;

   /*
    *
    * Robot
    *
    */
   void PreRobotPeriodic( const bool argIsParent = false );

   /*
    *
    * Sensor Functions
    *
    */
   void UpdateRates( const float argConversion );

   /*
    *
    * IMU Functions
    *
    */
   void UpdateIMU( void ) override;

private:
   bool gotError( void ) const;

   bool gotQuaternion( void ) const;

   bool gotMagnetometer( void ) const;

   bool gotAccelerometer( void ) const;

   bool gotGyrometer( void ) const;

   bool gotBarometer( void ) const;

   void readMagnetometer( double& mx,
                          double& my,
                          double& mz );

   void readAccelerometer( double& ax,
                           double& ay,
                           double& az );

   void readGyrometer( double& gx,
                       double& gy,
                       double& gz );

   bool hasFeature( uint8_t features ) const;

   void readThreeAxis( uint8_t xreg,
                       int16_t& x,
                       int16_t& y,
                       int16_t& z );

   const char* getErrorString(void) const;

   uint8_t getProductId(void) const;
   uint8_t getRevisionId(void) const;
   uint16_t getRamVersion(void) const;
   uint16_t getRomVersion(void) const;

   uint8_t getSentralStatus( void ) const;

   void requestReset( void ) const;
   void setPassThroughMode( void ) const;
   void setMasterMode( void ) const;
   void setRunEnable( void ) const;
   void setRunDisable( void ) const;

   void setAccelLpfBandwidth(uint8_t bw) const;
   void setGyroLpfBandwidth(uint8_t bw) const;

   void setQRateDivisor(uint8_t divisor) const;
   void setMagRate(uint8_t rate) const;
   void setAccelRate(uint8_t rate) const;
   void setGyroRate(uint8_t rate) const;
   void setBaroRate(uint8_t rate) const;

   uint8_t getActualMagRate( void ) const;
   uint16_t getActualAccelRate( void ) const;
   uint16_t getActualGyroRate( void ) const;
   uint8_t getActualBaroRate( void ) const;
   uint8_t getActualTempRate( void ) const;

   void algorithmControlRequestParameterTransfer( void ) const;
   void algorithmControlReset( void ) const;

   void enableEvents( uint8_t mask ) const;

   void requestParamRead( uint8_t param ) const;
   uint8_t getParamAcknowledge( void ) const;
   uint8_t readSavedParamByte0( void ) const;
   uint8_t readSavedParamByte1( void ) const;
   uint8_t readSavedParamByte2( void ) const;
   uint8_t readSavedParamByte3( void ) const;

   uint8_t getRunStatus( void ) const;
   uint8_t getAlgorithmStatus( void ) const;
   uint8_t getPassThruStatus( void ) const;
   uint8_t getEventStatus( void ) const;
   uint8_t getSensorStatus( void ) const;
   uint8_t getErrorStatus( void ) const;

   void setGyroFs( uint16_t gyro_fs ) const;

   void setMagAccFs( uint16_t mag_fs,
                     uint16_t acc_fs ) const;

   void loadParamByte0( uint8_t value ) const;
   void loadParamByte1( uint8_t value ) const;
   void loadParamByte2( uint8_t value ) const;
   void loadParamByte3( uint8_t value ) const;

   void writeGp36( uint8_t value ) const;
   void writeGp37( uint8_t value ) const;
   void writeGp38( uint8_t value ) const;
   void writeGp39( uint8_t value ) const;
   void writeGp40( uint8_t value ) const;
   void writeGp50( uint8_t value ) const;
   void writeGp51( uint8_t value ) const;
   void writeGp52( uint8_t value ) const;
   void writeGp53( uint8_t value ) const;
   void writeGp54( uint8_t value ) const;
   void writeGp55( uint8_t value ) const;
   void writeGp56( uint8_t value ) const;

   void readAccelerometer(int16_t& ax,
                          int16_t& ay,
                          int16_t& az);

   void readMagnetometer(int16_t& mx,
                         int16_t& my,
                         int16_t& mz);

   void readGyrometer(int16_t& gx,
                      int16_t& gy,
                      int16_t& gz);

   void readQuaternion(float& qw,
                       float& qx,
                       float& qy,
                       float& qz);

   void readBarometer(float& pressure,
                      float& temperature);

   void setIntegerParam( uint8_t param,
                         uint32_t param_val ) const;

   void getFullScaleRanges( uint8_t& accFs,
                            uint16_t& gyroFs,
                            uint16_t& magFs );

   uint8_t readRegister( uint8_t subAddress ) const;

   void readRegisters( uint8_t subAddress,
                       uint8_t count,
                       uint8_t* dest ) const;

   void writeRegister( uint8_t subAddress,
                       uint8_t data ) const;

   static float uint32_reg_to_float(uint8_t *buf);
};

#endif

#endif /* TEAM271_GYRO_H_ */
