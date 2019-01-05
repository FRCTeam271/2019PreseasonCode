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

/*
 * ZMQ
 */
#include "zmq.hpp"
#include "zmq_addon.hpp"
#include "zhelpers.hpp"
#include "zmsg.hpp"

using namespace zmq;

/*
 * Qt
 */
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

#include <QMutex>
#include <QThread>
#include <QDebug>
#include <QMessagebox>

using namespace std;

class MyThread : public QThread
{
	Q_OBJECT
public:
	explicit MyThread(QObject *parent = 0, bool b = false);
	void run();

	// if Stop = true, the thread will break
	// out of the loop, and will be disposed
	bool Stop;

signals:
	// To communicate with Gui Thread
	// we need to emit a signal
   void UpdateEnabled( uint32_t argData );
   void UpdateAutonomous( uint32_t argData );
   void UpdateTest( uint32_t argData );
   void UpdateTBECount( uint32_t argData );

   void EnableChartUpdates(bool argEnableChartUpdate);

   void AddSeries(QString argChartName);
   void AddPoint(QString argChartName, qreal argSeriesIndex, qreal time, qreal argData, qreal startTime );
   void AddPointList(QString argChartName, qreal argSeriesIndex, QList<QPointF>* argData, qreal startTime);
};