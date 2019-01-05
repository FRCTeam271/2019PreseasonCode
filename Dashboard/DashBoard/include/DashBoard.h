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

#pragma once
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

#include <QMutex>
#include <QThread>
#include <QDebug>
#include <QMessagebox>

#include "ThreadNetwork.h"

#include "T271Chart.h"
#include "ui_DashBoardNew.h"

class DashBoardNew : public QMainWindow
{
	Q_OBJECT

public:
	DashBoardNew(QWidget *parent = Q_NULLPTR);
	~DashBoardNew();

private:
	Ui::DashBoardNew ui;

	MyThread* mThread = nullptr;

   QLabel* lEnabled = nullptr;
   QLabel* lAutonomous = nullptr;
   QLabel* lTest = nullptr;
   QLabel* lTBECount = nullptr;

   T271Chart* PowerChart_ = nullptr;

   T271Chart* TimingChart_ = nullptr;

   T271Chart* MiscChart_ = nullptr;

   T271Chart* chartDrive_Input = nullptr;

   T271Chart* chartDrive_IMU = nullptr;

   QList<T271Chart*> charts;

public slots:
	void onUpdateEnabled( uint32_t argData );
   void onUpdateAutonomous( uint32_t argData );
   void onUpdateTest( uint32_t argData );
   void onUpdateTBECount( uint32_t argData );

   void onEnableChartUpdates(bool argEnableChartUpdate);

   void onAddSeries(QString argChartName);
   void onAddPoint(QString argChartName, qreal argSeriesIndex, qreal time, qreal argData, qreal startTime);
   void onAddPointList(QString argChartName, qreal argSeriesIndex, QList<QPointF>* argData, qreal startTime);

private slots:
	void Button();
};
