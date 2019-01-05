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

#define MAX_NETWORK_POINTS 10

MyThread::MyThread(QObject *parent, bool b) :
   QThread(parent), Stop(b)
{
}

// run() will be called when a thread starts
void MyThread::run(void)
{
   uint32_t time = 0;
   double tmpDriveValue = 0;

   qreal startTime = 0;
   uint8_t startTimeInit = 0;

   MainInit();

   QMutex mutex;

   /*
    * Wait Forever
    */
   while (true)
   {
      // prevent other threads from changing the "Stop" value
      mutex.lock();
      if (this->Stop)
      {
         break;
      }
      mutex.unlock();

      uint32_t tmpReceiveStatus = 0;

      tmpReceiveStatus = DashboardSub_.SubLoop();

      if (tmpReceiveStatus > 0)
      {
         if (startTimeInit == 0)
         {
            startTime = GlobalNetworkObject_.Read<float>(t271_global_data_index::T271_GLOBAL_DATA_NETWORK_TIME_STAMP);
            startTimeInit = 1;
         }

         emit UpdateEnabled((uint32_t)Robot_->IsEnabled());
         emit UpdateAutonomous((uint32_t)Robot_->IsAutonomous());
         emit UpdateTest((uint32_t)Robot_->IsTest());
         emit UpdateTBECount(GlobalNetworkObject_.Read<float>(t271_global_data_index::T271_GLOBAL_DATA_TBE_COUNT));

         if (GlobalNetworkObject_.GetDirty(t271_global_data_index::T271_GLOBAL_DATA_TIME_MAIN_DELTA) == 1)
         {
            emit AddPoint("chartPower",
               0,
               GlobalNetworkObject_.Read<float>(t271_global_data_index::T271_GLOBAL_DATA_NETWORK_TIME_STAMP),
               GlobalNetworkObject_.Read<float>(t271_global_data_index::T271_GLOBAL_DATA_TIME_MAIN_DELTA), // Blue
               startTime);
         }

         if (tmpReceiveStatus < 200)
         {
            emit AddPoint("chartPower",
               2,
               GlobalNetworkObject_.Read<float>(t271_global_data_index::T271_GLOBAL_DATA_NETWORK_TIME_STAMP),
               tmpReceiveStatus, // Orange
               startTime);
         }
         else
         {
            emit AddPoint("chartPower",
               2,
               GlobalNetworkObject_.Read<float>(t271_global_data_index::T271_GLOBAL_DATA_NETWORK_TIME_STAMP),
               tmpReceiveStatus, // Orange
               startTime);
         }

         if (Drive_->GetDirty(t271_drive_data_index::T271_DRIVE_DATA_DRIVER_VALUE) == 1)
         {
            emit AddPoint("Drive_Input",
               0,
               GlobalNetworkObject_.Read<float>(t271_global_data_index::T271_GLOBAL_DATA_NETWORK_TIME_STAMP),
               Drive_->Read<float>(t271_drive_data_index::T271_DRIVE_DATA_DRIVER_VALUE), // Blue
               startTime);
         }

         if (Drive_->GetDirty(t271_drive_data_index::T271_DRIVE_DATA_PASS_VALUE) == 1)
         {
            emit AddPoint("Drive_Input",
               1,
               GlobalNetworkObject_.Read<float>(t271_global_data_index::T271_GLOBAL_DATA_NETWORK_TIME_STAMP),
               Drive_->Read<float>(t271_drive_data_index::T271_DRIVE_DATA_PASS_VALUE), // Green
               startTime);
         }
 
         emit AddPoint("Drive_IMU",
            2,
            GlobalNetworkObject_.Read<float>(t271_global_data_index::T271_GLOBAL_DATA_NETWORK_TIME_STAMP),
            Drive_->GetIMU()->GetYaw(), // Orange
            startTime);
      }

      DashboardSub_.ClearDirty();

      //NetworkLoop_Dashboard_Pub();

      //NetworkLoop_Dashboard_Pub_ClearDirty();

      this->msleep(50);
   }

   MainCleanup();
}