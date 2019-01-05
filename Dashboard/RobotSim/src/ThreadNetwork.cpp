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

#include "ThreadNetwork.h"

MyThread::MyThread( QObject *parent, bool b ) :
   QThread( parent ), Stop( b )
{
}

void MyThread::run(void)
{
   MainInit();

   /*
    * Wait Forever
    */
   while (true)
   {
      QMutex mutex;
      // prevent other threads from changing the "Stop" value
      mutex.lock();
      if (this->Stop)
      {
         break;
      }
      mutex.unlock();

      uint8_t tmpReceiveStatus = 0;

      tmpReceiveStatus = NetworkLoopSub();

      MainLoop();

      NetworkLoopPub();

      this->msleep( 10 );
   }

   MainCleanup();
}
