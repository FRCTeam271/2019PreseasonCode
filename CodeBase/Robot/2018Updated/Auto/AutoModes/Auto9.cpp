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

#include "Team271_Includes.h"

#if defined(T271_AUTO_ENABLE)
/*
 *
 * Auto 9
 * Calibrate
 *
 */
void Team271_Auto9( void )
{
   const uint32_t tmpAutoModeNum = 9U;

   Team271AutoMode* tmpAutoMode = nullptr;
   AutoActionList* tmpAutoActions = nullptr;
   Team271AutoAction* tmpAutoAction = nullptr;
   //Team271AutoActionDrive* tmpAutoActionDrive = nullptr;
   //Team271AutoActionLadder* tmpAutoActionLadder = nullptr;

   if( Auto_ != nullptr )
   {
      /*
       * Get the Auto Mode
       */
      tmpAutoMode = Auto_->GetAutoMode( tmpAutoModeNum );

      if( tmpAutoMode != nullptr )
      {
         /*
          *
          *
          * Auto Start
          *
          *
          */
         tmpAutoAction = tmpAutoActions->AddAutoAction( new Team271AutoAction( eAutoActionTypeNone ) );
      }
   }
}
#endif
