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

#include "T271Chart.h"

#include "DashBoard.h"

DashBoardNew::DashBoardNew(QWidget *parent)
   : QMainWindow(parent)
{
   ui.setupUi(this);

   lEnabled = findChild<QLabel*>("lEnabled");
   lAutonomous = findChild<QLabel*>("lAutonomous");
   lTest = findChild<QLabel*>("lTest");
   lTBECount = findChild<QLabel*>("lTBECount");

   // create an instance of MyThread
   mThread = new MyThread(this);

   PowerChart_ = new T271Chart("chartPower", mThread, findChild<QChartView*>("chartPower"));
   MiscChart_ = new T271Chart("chartMisc", mThread, findChild<QChartView*>("chartMisc"));

   chartDrive_Input = new T271Chart("Drive_Input", mThread, findChild<QChartView*>("chartDrive_Input"));
   chartDrive_IMU = new T271Chart("Drive_IMU", mThread, findChild<QChartView*>("chartDrive_IMU"));

   charts.append(PowerChart_);
   charts.append(MiscChart_);
   charts.append(chartDrive_Input);
   charts.append(chartDrive_IMU);

   // connect signal/slot
   connect(mThread, &MyThread::UpdateEnabled,
      this, &DashBoardNew::onUpdateEnabled, Qt::QueuedConnection);

   connect(mThread, &MyThread::UpdateAutonomous,
      this, &DashBoardNew::onUpdateAutonomous, Qt::QueuedConnection);

   connect(mThread, &MyThread::UpdateTest,
      this, &DashBoardNew::onUpdateTest, Qt::QueuedConnection);

   connect(mThread, &MyThread::UpdateTBECount,
      this, &DashBoardNew::onUpdateTBECount, Qt::QueuedConnection);



   // connect signal/slot
   connect(mThread, &MyThread::EnableChartUpdates,
      this, &DashBoardNew::onEnableChartUpdates, Qt::QueuedConnection);

   connect(mThread, &MyThread::AddSeries,
      this, &DashBoardNew::onAddSeries, Qt::QueuedConnection);

   connect(mThread, &MyThread::AddPoint,
      this, &DashBoardNew::onAddPoint, Qt::QueuedConnection);

   connect(mThread, &MyThread::AddPointList,
      this, &DashBoardNew::onAddPointList, Qt::QueuedConnection);

   mThread->start();
}

DashBoardNew::~DashBoardNew()
{
   mThread->Stop = true;

   while (mThread->isRunning())
   {

   }
}

void DashBoardNew::onUpdateEnabled(uint32_t argData)
{
   if (argData == 1)
   {
      lEnabled->setText("Yes");
   }
   else
   {
      lEnabled->setText("No");
   }
}

void DashBoardNew::onUpdateAutonomous(uint32_t argData)
{
   if (argData == 1)
   {
      lAutonomous->setText("Yes");
   }
   else
   {
      lAutonomous->setText("No");
   }
}

void DashBoardNew::onUpdateTest(uint32_t argData)
{
   if (argData == 1)
   {
      lTest->setText("Yes");
   }
   else
   {
      lTest->setText("No");
   }
}

void DashBoardNew::onUpdateTBECount(uint32_t argData)
{
   lTBECount->setText(QString::number(argData));
}

void DashBoardNew::Button(void)
{
   //QMessageBox::about(this, "Game Message", QString(GSMEntry_.GetString("").c_str()));
}

void DashBoardNew::onEnableChartUpdates(bool argEnableChartUpdate)
{
   for (uint32_t i = 0; i < charts.count(); i++)
   {
      if (charts[i] != nullptr)
      {
         charts[i]->onEnableChartUpdates(argEnableChartUpdate);
      }
   }
}

void DashBoardNew::onAddSeries(QString argChartName)
{
}

void DashBoardNew::onAddPoint(QString argChartName, qreal argSeriesIndex, qreal time, qreal argData, qreal startTime)
{
   onEnableChartUpdates(false);

   for (uint32_t i = 0; i < charts.count(); i++)
   {
      if (charts[i] != nullptr)
      {
         if (argChartName == charts[i]->GetChartName() && argSeriesIndex < charts[i]->GetSeriesCount())
         {
            charts[i]->onAddPoint(argChartName, argSeriesIndex, time, argData, startTime);
            charts[i]->onUpdateRanges();
            break;
         }
      }
   }

   onEnableChartUpdates(true);
}

void DashBoardNew::onAddPointList(QString argChartName, qreal argSeriesIndex, QList<QPointF>* argData, qreal startTime)
{
   onEnableChartUpdates(false);

   for (uint32_t i = 0; i < charts.count(); i++)
   {
      if (charts[i] != nullptr)
      {
         if (argChartName == charts[i]->GetChartName() && argSeriesIndex < charts[i]->GetSeriesCount())
         {
            charts[i]->onAddPointList(argChartName, argSeriesIndex, argData, startTime);
            break;
         }
      }
   }

   onEnableChartUpdates(true);
}