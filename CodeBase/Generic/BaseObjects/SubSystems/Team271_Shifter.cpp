#include "Team271_Includes.h"

#if defined( T271_SHIFTER_ENABLE )

/*
 *
 * Team271Shifter
 *
 */
Team271Shifter::Team271Shifter( Team271Base* const argParent,
                                const string& argName,
                                const uint32_t argCreateSolenoids )
      : Team271Base( argParent,
                     argName,
                     t271_shifter_data_index::T271_DATA_COUNT ),
        IsSolCreator_( argCreateSolenoids )
{
   LOG_TRACE( "%s - Constructor Start",
              GetName().c_str() );

   Write( t271_shifter_data_index::T271_SHIFTER_DATA_SPEED_MODIFIER,
          0.25f );

   Write( t271_shifter_data_index::T271_SHIFTER_DATA_SPEED_MODIFIER_TIME,
          0.040f );

   Write( t271_shifter_data_index::T271_SHIFTER_DATA_GEAR_PREV,
          TEAM271_GEAR_COUNT );

   /*
    * Initialize Shifter Structures
    */
   for( uint8_t i = 0; i < TEAM271_GEAR_COUNT; i++ )
   {
      /* Clear out the Solenoid List */
      SolTransShifter_[i] = nullptr;
   }

   LOG_TRACE( "%s - Constructor End",
              GetName().c_str() );
}

Team271Shifter::~Team271Shifter( void )
{

}

/*
 *
 * Getters
 *
 */
const string Team271Shifter::GetClassName( void ) const
{
   return "Team271Shifter";
}

eGear Team271Shifter::GetGear( void ) const
{
   return Read<eGear>( t271_shifter_data_index::T271_SHIFTER_DATA_GEAR_CURRENT );
}

const string Team271Shifter::GetGearName( void ) const
{
   return GetGearName( GetGear() );
}

const string Team271Shifter::GetGearName( const eGear argGear ) const
{
   string tmpReturnValue = "Invalid";

   if( argGear >= TEAM271_GEAR_COUNT )
   {
      LOG_ERROR( "%s - Invalid Gear Index",
                 GetName().c_str(),
                 argGear );
   }

   return tmpReturnValue;
}

float Team271Shifter::GetSpeedModifier( void ) const
{
   return Read<float>( t271_shifter_data_index::T271_SHIFTER_DATA_SPEED_MODIFIER );
}

float Team271Shifter::GetSpeedModifierTime( void ) const
{
   return Read<float>( t271_shifter_data_index::T271_SHIFTER_DATA_SPEED_MODIFIER_TIME );
}

const bool Team271Shifter::GetSolState( const eGear argGear,
                                        const uint32_t argIndex ) const
{
   const bool tmpSolState[TEAM271_GEAR_COUNT][TEAM271_GEAR_COUNT] = { { false,
                                                                        false,
                                                                        false,
                                                                        false },
                                                                      { false,
                                                                        false,
                                                                        false,
                                                                        false },
                                                                      { false,
                                                                        false,
                                                                        false,
                                                                        false },
                                                                      { false,
                                                                        false,
                                                                        false,
                                                                        false } };

   bool tmpReturn = false;

   if( argGear >= TEAM271_GEAR_COUNT )
   {
      LOG_ERROR( "%s - Invalid Gear Index",
                 GetName().c_str(),
                 argGear );
   }
   else if( argIndex >= TEAM271_GEAR_COUNT )
   {
      LOG_ERROR( "%s - Invalid Sol Index",
                 GetName().c_str(),
                 argGear );
   }
   else
   {
      tmpReturn = tmpSolState[argGear][argIndex];
   }

   return tmpReturn;
}

/*
 *
 * Setters
 *
 */
void Team271Shifter::SetGear( const eGear argGear )
{
   if( argGear >= TEAM271_GEAR_COUNT )
   {
      LOG_ERROR( "%s - Invalid Gear Index: %d",
                 GetName().c_str(),
                 argGear );
   }
   else
   {
      /*
       * Update the Current Gear
       */
      Write( t271_shifter_data_index::T271_SHIFTER_DATA_GEAR_CURRENT,
             argGear );
   }
}

/*
 *
 * Robot
 *
 */
void Team271Shifter::PreRobotPeriodic( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::PreRobotPeriodic();

   Set( t271_shifter_data_index::T271_SHIFTER_DATA_GEAR_PREV,
        t271_shifter_data_index::T271_SHIFTER_DATA_GEAR_CURRENT );
}

void Team271Shifter::PostRobotPeriodic( const bool argIsParent )
{
   if( GetGear() != Read<eGear>( t271_shifter_data_index::T271_SHIFTER_DATA_GEAR_PREV ) )
   {
      /*
       * Change all Solenoid States
       */
      for( uint8_t i = 0; i < TEAM271_GEAR_COUNT; i++ )
      {
         if( SolTransShifter_[i] != nullptr )
         {
            if( GetSolState( GetGear(), i ) == false )
            {
               SolTransShifter_[i]->SetOutput( TEAM271_SOLENOID_STATE_OFF );
            }
            else
            {
               SolTransShifter_[i]->SetOutput( TEAM271_SOLENOID_STATE_ON );
            }
         }
      }
   }

   /*
    * Base Call
    */
   Team271Base::PostRobotPeriodic();
}
#endif
