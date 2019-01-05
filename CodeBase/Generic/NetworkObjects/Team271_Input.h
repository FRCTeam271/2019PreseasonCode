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

#ifndef TEAM271_INPUT_H_
#define TEAM271_INPUT_H_

#define INPUT_DSO_WAIT_COUNT           ( 128 )

#define DSO_INIT_STATE                 ( TEAM271_DSO_STATE_INACTIVE )

static constexpr uint32_t kMaxJoystickAxes = 12;
static constexpr uint32_t kMaxJoystickPOVs = 12;
static constexpr uint32_t kMaxJoystickButtons = 15;
static constexpr uint32_t kMaxJoysticks = 6;

enum eInputJoystickPOV
{
   eInputJoystickPOVUp     = 0,
   eInputJoystickPOVRight  = 90,
   eInputJoystickPOVDown   = 180,
   eInputJoystickPOVLeft   = 270,
   eInputJoystickPOVMax
};

/*
 * Input USB Port Numbers
 *
 * ASSIGN All Input USB Ports Here
 * This can be a Joystick, Gamepad, Xbox Controller, Etc.
 */
enum eInputIndex
{
   eDriverLeft    = 0,
   eOperatorLeft  = 1,
   eOperatorRight = 2,
   eInputIndexMax
};

/*
 * Driver Left Input Button Index
 *
 * ASSIGN All Buttons Here
 * Set Correct Max Button Count for Input Device
 */
enum eDriverLeftInputButtonIndex
{
   eDriverLeftButton1                  = 0,
   eDriverLeftButtonShoot              = 1,
   eDriverLeftButtonHerdIn             = 4,
   eDriverLeftButtonHerdOut            = 5,
   eDriverLeftButtonHerderInOut        = 2,
   eDriverLeftButtonShooterOn          = 3,
   eDriverLeftButtonShifter            = 6,
   eDriverLeftButton8                  = 7,
   eDriverLeftButtonDSO1               = 8,
   eDriverLeftButtonDSO2               = 9,

   eDriverLeftButtonDriveLowGear       = 4,
   eDriverLeftButtonDriveMidLowGear    = 90,
   eDriverLeftButtonDriveMidHighGear   = 90,
   eDriverLeftButtonDriveHighGear      = 5,

   eDriverRightButtonDriveMidHighGear  = 1,
   eDriverRightButtonDriveHighGear     = 0,
};

/*
 * Driver Left Input Axis Index
 *
 * ASSIGN All Axis Here
 * Set Correct Max Axis Count for Input Device
 */
enum eDriverLeftInputAxisIndex
{
   eDriverLeftAxisDriveDriverY      = 1,
   eDriverLeftAxisDriveDriverX      = 0,
   eDriverLeftAxisDrivePassnegerY   = 5,
   eDriverLeftAxisDrivePassnegerX   = 2,

   eDriverLeftAxisElevationDown     = 4,
   eDriverLeftAxisElevationUp       = 3,
};

/*
 * Driver Right Input Button Index
 *
 * ASSIGN All Buttons Here
 * Set Correct Max Button Count for Input Device
 */
enum eDriverRightInputButtonIndex
{
   eDriverRightButton1     = 1,
   eDriverRightButton2     = 2,
   eDriverRightButton3     = 3,
   eDriverRightButton4     = 4,
   eDriverRightButton5     = 5,
   eDriverRightButton6     = 6,
   eDriverRightButton7     = 7,
   eDriverRightButton8     = 8,
   eDriverRightButton9     = 9,
   eDriverRightButton10    = 10,
};

/*
 * Operator Left Input Axis Index
 *
 * ASSIGN All Axis Here
 * Set Correct Max Axis Count for Input Device
 */
enum eOperatorLeftInputAxisIndex
{
   eOperatorLeftInputAxisLadderY = 1,
};

/*
 * Operator Left Input Button Index
 *
 * ASSIGN All Buttons Here
 * Set Correct Max Button Count for Input Device
 */
enum eOperatorLeftInputButtonIndex
{
   eOperatorLeftButtonLadderLowGear    = 5,
   eOperatorLeftButtonLadderHighGear   = 6,
   eOperatorLeftButtonBreakOff         = 2,
   eOperatorLeftButtonBreakOn          = 3,
   eOperatorLeftButtonHerderIn         = 4,
   eOperatorLeftButtonHerderOut        = 5,
   eOperatorLeftButton7                = 6,
   eOperatorLeftButtonDSO1             = 7,
   eOperatorLeftButtonDSO2             = 8,
   eOperatorLeftButton10               = 9,
};

/*
 * Operator Right Input Axis Index
 *
 * ASSIGN All Axis Here
 * Set Correct Max Axis Count for Input Device
 */
enum eOperatorRightInputAxisIndex
{
   eOperatorRightInputAxisArmY   = 1,
};

/*
 * Operator Right Input Button Index
 *
 * ASSIGN All Buttons Here
 * Set Correct Max Button Count for Input Device
 */
enum eOperatorRightInputButtonIndex
{
   eOperatorRightButton1         = 5,
   eOperatorRightButton2         = 6,
   eOperatorRightButton3         = 2,
   eOperatorRightButton4         = 3,
   eOperatorRightButtonHerderIn  = 4,
   eOperatorRightButtonHerderOut = 5,
   eOperatorRightButton7         = 6,
   eOperatorRightButton8         = 7,
   eOperatorRightButton9         = 8,
   eOperatorRightButton10        = 9,
};

#if defined(T271_INPUT_ENABLE)

/*
 *
 * Structures
 *
 */
typedef struct
{
   int16_t count;
   int16_t axes[kMaxJoystickAxes];
} T271_JoystickAxesInt;

typedef struct
{
   int16_t count;
   float axes[kMaxJoystickAxes];
} T271_JoystickAxes;

typedef struct
{
   int16_t count;
   int16_t povs[kMaxJoystickPOVs];
}T271_JoystickPOVs;

typedef struct
{
   uint32_t buttons;
   uint8_t count;
} T271_JoystickButtons;

typedef struct
{
   uint8_t isXbox;
   uint8_t type;
   char name[256];
   uint8_t axisCount;
   uint8_t axisTypes[kMaxJoystickAxes];
   uint8_t buttonCount;
   uint8_t povCount;
} T271_JoystickDescriptor;

/*
 *
 * Shared Network Data
 *
 */
class t271_input_data_index : public t271_network_data_index
{
public:
   typedef enum : uint8_t
   {
      T271_INPUT_DATA_DSO_STATE = t271_network_data_index::T271_DATA_COUNT,

      T271_INPUT_DATA_USER_BUTTON,
      T271_INPUT_DATA_USER_BUTTON_ACTION,

      T271_DATA_COUNT,
   } t271_input_data_index_t;
};

class Team271Input : public Team271NetworkObject
{
private:
   /*
    *
    * Variables
    *
    */
   atomic_flag UpdateFlag_ = ATOMIC_FLAG_INIT;

   T271_JoystickDescriptor _Desc[eInputIndexMax];

   T271_JoystickAxes _Axes[eInputIndexMax];
   T271_JoystickPOVs _Povs[eInputIndexMax];
   T271_JoystickButtons _Buttons[eInputIndexMax];

   T271_JoystickAxes _AxesPrev[eInputIndexMax];
   T271_JoystickPOVs _PovsPrev[eInputIndexMax];
   T271_JoystickButtons _ButtonsPrev[eInputIndexMax];

   /*
    * User Button
    */
   bool _InputUserButtonStatePrev = false;

   /*
    * Double Secret Override
    */
   TEAM271_DSO_STATE _DSOStatePrev = DSO_INIT_STATE;
   uint32_t _DSOToggleCnt = 0;

public:
   static Team271Input& Instance( void );

   static TEAM271_DSO_STATE InstanceGetDSOState( void );

   static double InstanceGetAxis( const eInputIndex newInputIndex,
                                  const uint32_t newAxis );

   static double InstanceGetAxisPrev( const eInputIndex newInputIndex,
                                      const uint32_t newAxis );

   static int32_t InstanceGetPOVState( const eInputIndex newInputIndex,
                                       const uint32_t newPOV );

   static int32_t InstanceGetPOVStatePrev( const eInputIndex newInputIndex,
                                           const uint32_t newPOV );

   static bool InstanceGetButtonState( const eInputIndex newInputIndex,
                                       const uint32_t newButton );

   static bool InstanceGetButtonStatePrev( const eInputIndex newInputIndex,
                                           const uint32_t newButton );

   Team271Input( void );
   ~Team271Input( void );

   Team271Input( Team271Input const& ) = delete;
   void operator=( Team271Input const& ) = delete;

   /*
    *
    * Getters
    *
    */
   uint32_t GetHasNewData( void ) const;

   bool IsUserButtonAction( void ) const;

   TEAM271_DSO_STATE GetDSOState( void ) const;

   double GetAxis( const eInputIndex newInputIndex,
                   const uint32_t newAxis ) const;

   double GetAxisPrev( const eInputIndex newInputIndex,
                       const uint32_t newAxis ) const;

   int32_t GetPOVState( const eInputIndex newInputIndex,
                        const uint32_t newPOV ) const;

   int32_t GetPOVStatePrev( const eInputIndex newInputIndex,
                            const uint32_t newPOV ) const;

   bool GetButtonState( const eInputIndex newInputIndex,
                        const uint32_t newButton ) const;

   bool GetButtonStatePrev( const eInputIndex newInputIndex,
                            const uint32_t newButton ) const;

   /*
    *
    * Updates
    *
    */
   void UpdateInput( void );
};

#endif

#endif /* TEAM271_INPUT_H_ */
