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

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

#include <QThread>
#include <QDebug>
#include <QMessagebox>

#include "ThreadNetwork.h"

#include "ui_DashBoardNew.h"

#include "T271Chart.h"
#include "DashBoard.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	DashBoardNew w;
	w.show();
	return a.exec();
}
