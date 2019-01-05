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
#include "ThreadNetwork.h"

#include "ui_DashBoardNew.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

#include <QMutex>
#include <QThread>
#include <QDebug>

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "QtCharts"


class T271Chart : public QObject
{
	Q_OBJECT

private:
   QString ChartName_ = "";

   MyThread* mThread = nullptr;

   QChart* chart = nullptr;
   QChartView* chartView = nullptr;
   QValueAxis* axisX = nullptr;
   QValueAxis* axisY = nullptr;

   QList<QLineSeries*> seriesList;

   double xAxisMax = 0;
   double xAxisMin = 0;

   double yAxisMax = 0;
   double yAxisMin = 0;

public:
   T271Chart(QString argChartName, MyThread* argNetworkThread, QChartView* argChartView );
	~T271Chart();

   QString GetChartName(void);
   uint32_t GetSeriesCount( void );

public slots:
   void onEnableChartUpdates( bool argEnableChartUpdate );

   void onUpdateRanges(void);

   void onAddSeries(QString argChartName);
   void onAddPoint(QString argChartName, qreal argSeriesIndex, qreal time, qreal argData, qreal startTime );
   void onAddPointList(QString argChartName, qreal argSeriesIndex, QList<QPointF>* argData, qreal startTime);

};
