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

T271Chart::T271Chart(QString argChartName, MyThread* argNetworkThread, QChartView* argChartView)
   : ChartName_(argChartName), mThread(argNetworkThread), chartView(argChartView), QObject()
{
   chart = new QChart();
   chart->legend()->hide();
   chart->setTitle(argChartName);

   QPen tmpGridPen;
   tmpGridPen.setColor(QColor(255, 255, 255));
   tmpGridPen.setBrush(QBrush(QColor(255, 255, 255)));

   axisX = new QValueAxis();
   //axisX->setLinePenColor( QColor( 255, 255, 255 ) );
   //axisX->setGridLinePen( tmpGridPen );
   chart->addAxis(axisX, Qt::AlignBottom);

   axisY = new QValueAxis();
   axisY->setTitleText("Value");
   //axisY->setLinePenColor( QColor( 255, 255, 255 ) );
   //axisY->setGridLinePen( tmpGridPen );
   chart->addAxis(axisY, Qt::AlignLeft);

   seriesList.reserve(3);
   seriesList.append(new QLineSeries());
   seriesList.append(new QLineSeries());
   seriesList.append(new QLineSeries());

   for (uint32_t i = 0; i < seriesList.count(); i++)
   {
      seriesList[i]->setUseOpenGL(true);

      chart->addSeries(seriesList[i]);

      seriesList[i]->attachAxis(axisX);
      seriesList[i]->attachAxis(axisY);
   }

   chartView->setChart(chart);
   chartView->setRenderHint(QPainter::Antialiasing);

   // create an instance of MyThread
   mThread = argNetworkThread;
   /*
   // connect signal/slot
   connect(mThread, &MyThread::EnableChartUpdates,
      this, &T271Chart::onEnableChartUpdates, Qt::QueuedConnection);

   connect(mThread, &MyThread::AddSeries,
      this, &T271Chart::onAddSeries, Qt::QueuedConnection);

   connect(mThread, &MyThread::AddPoint,
      this, &T271Chart::onAddPoint, Qt::QueuedConnection);

   connect(mThread, &MyThread::AddPointList,
      this, &T271Chart::onAddPointList, Qt::QueuedConnection);
      */
}

T271Chart::~T271Chart()
{
   for (uint32_t i = 0; i < seriesList.count(); i++)
   {
      delete seriesList[i];
   }

   delete axisX;
   delete axisY;

   delete chart;
}

QString T271Chart::GetChartName(void)
{
   return ChartName_;
}

uint32_t T271Chart::GetSeriesCount(void)
{
   return seriesList.count();
}

void T271Chart::onEnableChartUpdates(bool argEnableChartUpdate)
{
   chartView->setUpdatesEnabled(argEnableChartUpdate);
}

void T271Chart::onAddSeries(QString argChartName)
{
   if (argChartName == ChartName_)
   {
      seriesList.append(new QLineSeries());

      uint32_t tmpIndex = seriesList.count() - 1;

      seriesList[tmpIndex]->setUseOpenGL(true);

      chart->addSeries(seriesList[tmpIndex]);

      seriesList[tmpIndex]->attachAxis(axisX);
      seriesList[tmpIndex]->attachAxis(axisY);
   }
}

void T271Chart::onUpdateRanges( void )
{
   axisX->setRange(xAxisMin, xAxisMax);

   double tmpMinScale = abs(yAxisMin)*0.05;

   if (tmpMinScale < 0.5)
   {
      tmpMinScale = 0.5;
   }
   else if (tmpMinScale > 3)
   {
      tmpMinScale = 3;
   }

   double tmpMaxScale = abs(yAxisMax)*0.05;

   if (tmpMaxScale < 0.5)
   {
      tmpMaxScale = 0.5;
   }
   else if (tmpMaxScale > 3)
   {
      tmpMaxScale = 3;
   }

   axisY->setRange(yAxisMin - tmpMinScale, yAxisMax + tmpMaxScale);
}

void T271Chart::onAddPoint(QString argChartName, qreal argSeriesIndex, qreal time, qreal argData, qreal startTime)
{
   if (argChartName == ChartName_ && argSeriesIndex < seriesList.count())
   {
      seriesList.at(argSeriesIndex)->append((qreal)time, argData);

      //xAxisMin = startTime;
      xAxisMin = time - 0.1f;
      xAxisMax = time;
      //axisX->setRange( startTime, time );
      //axisX->setRange(time - 0.1f, time);
      if (argData > yAxisMax)
      {
         yAxisMax = argData;
      }

      if (argData < yAxisMin)
      {
         yAxisMin = argData;
      }
      /*
      double tmpMinScale = abs(yAxisMin)*0.05;

      if (tmpMinScale < 0.5)
      {
         tmpMinScale = 0.5;
      }
      else if (tmpMinScale > 3)
      {
         tmpMinScale = 3;
      }

      double tmpMaxScale = abs(yAxisMax)*0.05;

      if (tmpMaxScale < 0.5)
      {
         tmpMaxScale = 0.5;
      }
      else if (tmpMaxScale > 3)
      {
         tmpMaxScale = 3;
      }
      */

      //axisY->setRange(axisY->min() - tmpMinScale, axisY->max() + tmpMaxScale);
      //axisY->setRange(yAxisMin - tmpMinScale, yAxisMax + tmpMaxScale);
      //axisY->setRange( 0, 0.03 );
   }
}

void T271Chart::onAddPointList(QString argChartName,
   qreal argSeriesIndex,
   QList<QPointF>* argData,
   qreal startTime)
{
   if (argData != nullptr)
   {
      if (argChartName == ChartName_ && argData->length() > 0)
      {
         if (argSeriesIndex < seriesList.count())
         {
            seriesList[argSeriesIndex]->append(*argData);

            for (int i = 0; i < argData->length(); i++)
            {
               //seriesX->append((*argData)[i].x(), (*argData)[i].y());
               //seriesX->append(argData->at(i));
               if (argData->at(i).y() > yAxisMax)
               {
                  yAxisMax = argData->at(i).y();
               }
               if (argData->at(i).y() < yAxisMin)
               {
                  yAxisMin = argData->at(i).y();
               }
            }

            //tmppoint = argData->at(0).x();
            //axisX->setRange((*argData)[0].x(), (*argData)[argData->length()-1].x());
            //axisX->setRange( startTime, ( *argData )[argData->length() - 1].x() );


            int32_t tmpCount = seriesList[argSeriesIndex]->points().length();

            if (tmpCount > 200)
            {
               axisX->setRange(seriesList[argSeriesIndex]->points()[tmpCount - 200].x(), seriesList[argSeriesIndex]->points()[tmpCount - 1].x());
            }
            else
            {
               axisX->setRange(seriesList[argSeriesIndex]->points()[0].x(), seriesList[argSeriesIndex]->points()[tmpCount - 1].x());
            }

            double tmpMinScale = abs(yAxisMin)*0.05;

            if (tmpMinScale < 0.5)
            {
               tmpMinScale = 0.5;
            }
            else if (tmpMinScale > 3)
            {
               tmpMinScale = 3;
            }

            double tmpMaxScale = abs(yAxisMax)*0.05;

            if (tmpMaxScale < 0.5)
            {
               tmpMaxScale = 0.5;
            }
            else if (tmpMaxScale > 3)
            {
               tmpMaxScale = 3;
            }

            axisY->setRange(yAxisMin - tmpMinScale, yAxisMax + tmpMaxScale);
            //axisY->setRange( 0, 0.030 );
            //axisY->setRange( -1, 1 );
         }
      }

      delete argData;
   }
}