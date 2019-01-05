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

/*
 *
 * Base
 *
 */
#include "Generic\BaseObjects\Team271_Base.h"

/*
 *
 * Base -> Components
 *
 */
#include "Generic\BaseObjects\Components\Team271_IO.h"
#include "Generic\BaseObjects\Components\Team271_PWM.h"
#include "Generic\BaseObjects\Components\Team271_I2C.h"
#include "Generic\BaseObjects\Components\Team271_SPI.h"

#include "Generic\BaseObjects\Components\Team271_PDP.h"
#include "Generic\BaseObjects\Components\Team271_PCM.h"

#include "Generic\BaseObjects\Components\Team271_Motor.h"
#include "Generic\BaseObjects\Components\Team271_Solenoid.h"

/*
 *
 * Base -> Sensors
 *
 */
#if defined(ENABLE_VISION)
#  include "Vision\Team271_GripPipeline.h"
#  include "Vision\Team271_CameraServer.h"
#  include "Vision\Team271_Vision.h"
#endif

#if defined(T271_SENSOR_ENABLE)
#  include "Generic\BaseObjects\Sensors\Team271_Sensor.h"

#  include "Generic\BaseObjects\Sensors\Team271_SensorSRX.h"

#  if defined(T271_IMU_ENABLE)
#     include "Generic\BaseObjects\Sensors\Team271_IMU.h"

#  if defined(T271_EM7180_ENABLE)
#     include "Generic\BaseObjects\Sensors\Team271_EM7180.h"
#  endif

#  if defined(T271_NAVX_ENABLE)
#     include "Generic\BaseObjects\Sensors\Team271_NavX.h"
#  endif

#  endif
#endif

/*
 *
 * Base -> Sub Systems
 *
 */

#include "Generic\BaseObjects\SubSystems\Team271_Robot.h"
#include "Generic\BaseObjects\SubSystems\Team271_Shifter.h"
#include "Generic\BaseObjects\SubSystems\Team271_Transmission.h"

/*
 *
 * Base -> Sub Systems -> Auto -> Auto Actions
 *
 */
#include "Generic\BaseObjects\SubSystems\Auto\Team271_AutoAction.h"

/*
 * Base -> Sub Systems -> Auto
 */
#include "Generic\BaseObjects\SubSystems\Auto\Team271_AutoActionList.h"
#include "Generic\BaseObjects\SubSystems\Auto\Team271_AutoMode.h"
#include "Generic\BaseObjects\SubSystems\Auto\Team271_Auto.h"

/*
 *
 * Managers
 *
 */
#include "Generic\BaseObjects\Team271_BaseManager.h"
