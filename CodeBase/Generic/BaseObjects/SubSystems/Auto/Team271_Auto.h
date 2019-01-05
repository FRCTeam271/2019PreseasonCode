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

#ifndef TEAM271_AUTO_H_
#define TEAM271_AUTO_H_

#if defined(T271_AUTO_ENABLE)

#define AUTO_MAX_MODES      ( 10 )

void Team271_Auto0( void );
void Team271_Auto1( void );
void Team271_Auto2( void );
void Team271_Auto3( void );
void Team271_Auto4( void );
void Team271_Auto5( void );
void Team271_Auto6( void );
void Team271_Auto7( void );
void Team271_Auto8( void );
void Team271_Auto9( void );
void Team271_Auto10( void );

/*
 *
 * Shared Network Data
 *
 */
typedef struct
{
   uint8_t Mode_ = 0;
   uint8_t Delay_ = 0;
}Team271AutoData;

class Team271Auto : public Team271Base
{
public:
   Team271AutoData SharedAutoData_;

private:
   Team271AutoMode _AutoModes[AUTO_MAX_MODES];

public:
   Team271Auto( void );

   /*
    *
    * Robot
    *
    */
   void RobotInit( const bool argIsParent = false ) override;

   /*
    *
    * Autonomous
    *
    */
   void AutonomousInit( const bool argIsParent = false ) override;
   void AutonomousPeriodic( const bool argIsParent = false ) override;

   /*
    *
    * Auto
    *
    */
   void CreateAuto( void );

   Team271AutoMode* GetAutoMode( const uint32_t argAutoMode );

   uint32_t IsLeft( void ) const;
};

#endif

#endif /* TEAM271_AUTO_H_ */
