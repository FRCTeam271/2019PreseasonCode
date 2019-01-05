#pragma once

#include <string>
#include <stdint.h>
#include <time.h>
#include <functional>
#include <vector>
#include <sstream>
#include <mutex>
#include <iostream>
#include <fstream>
#include <map>
#include <sys/stat.h>
#include <dirent.h>
#include <thread>
#include <map>
#include <execinfo.h>
#include <set>

#include <FRC_NetworkCommunication/CANSessionMux.h>  // tCANStreamMessage

#include "ctre/phoenix/ErrorCode.h"
#include "ctre/phoenix/paramEnum.h"
#include "ctre/phoenix/HsvToRgb.h"
#include "ctre/phoenix/Utilities.h"
#include "ctre/phoenix/Stopwatch.h"
#include "ctre/phoenix/LinearInterpolation.h"
#include "ctre/phoenix/CustomParamConfiguration.h"

#include "ctre/phoenix/CANifierControlFrame.h"
#include "ctre/phoenix/CANifierFaults.h"
#include "ctre/phoenix/CANifierStatusFrame.h"
#include "ctre/phoenix/CANifierStickyFaults.h"
#include "ctre/phoenix/CANifierVelocityMeasPeriod.h"
#include "ctre/phoenix/CANifier.h"

#include "ctre/phoenix/CCI/CANifier_CCI.h"

#include "ctre/phoenix/CCI/Logger_CCI.h" // c_Logger_*
#include "ctre/phoenix/CCI/MotController_CCI.h"

#include "ctre/phoenix/CTRLogger.h"

#include "ctre/phoenix/LowLevel/Logger_LowLevel.h"
#include "ctre/phoenix/LowLevel/ResetStats.h"
#include "ctre/phoenix/LowLevel/CANBusAddressable.h"
#include "ctre/phoenix/LowLevel/Device_LowLevel.h"
#include "ctre/phoenix/LowLevel/CTRE_Native_CAN.h"

#include "ctre/phoenix/Motion/SetValueMotionProfile.h"
#include "ctre/phoenix/Motion/MotionProfileStatus.h"
#include "ctre/phoenix/Motion/TrajectoryPoint.h"

#include "ctre/phoenix/MotorControl/ControlMode.h"
#include "ctre/phoenix/MotorControl/ControlFrame.h"
#include "ctre/phoenix/MotorControl/VelocityMeasPeriod.h"
#include "ctre/phoenix/MotorControl/NeutralMode.h"
#include "ctre/phoenix/MotorControl/FeedbackDevice.h"
#include "ctre/phoenix/MotorControl/StatusFrame.h"
#include "ctre/phoenix/MotorControl/LimitSwitchType.h"
#include "ctre/phoenix/MotorControl/StickyFaults.h"

#include "ctre/phoenix/MotorControl/SensorTerm.h"
#include "ctre/phoenix/MotorControl/RemoteSensorSource.h"

#include "ctre/phoenix/MotorControl/DemandType.h"
#include "ctre/phoenix/MotorControl/Faults.h"
#include "ctre/phoenix/MotorControl/FollowerType.h"
#include "ctre/phoenix/MotorControl/SensorCollection.h"

#include "ctre/phoenix/LowLevel/MotController_LowLevel.h"
#include "ctre/phoenix/LowLevel/MotControllerWithBuffer_LowLevel.h"

#include "ctre/phoenix/MotorControl/IFollower.h"
#include "ctre/phoenix/MotorControl/IMotorController.h"
#include "ctre/phoenix/MotorControl/IMotorControllerEnhanced.h"

#include "ctre/phoenix/MotorControl/CAN/BaseMotorController.h"
#include "ctre/phoenix/MotorControl/CAN/TalonSRX.h"
#include "ctre/phoenix/MotorControl/CAN/VictorSPX.h"

#include "ctre/phoenix/MotorControl/DeviceCatalog.h"
#include "ctre/phoenix/MotorControl/GroupMotorControllers.h"

#include "ctre/phoenix/Sensors/PigeonIMU_ControlFrame.h"
#include "ctre/phoenix/Sensors/PigeonIMU_Faults.h"
#include "ctre/phoenix/Sensors/PigeonIMU_StatusFrame.h"
#include "ctre/phoenix/Sensors/PigeonIMU_StickyFaults.h"
#include "ctre/phoenix/Sensors/PigeonIMU.h"

#include "ctre/phoenix/LowLevel/PigeonIMU_LowLevel.h"

#include "ctre/phoenix/CCI/PigeonIMU_CCI.h"
#include "ctre/phoenix/LowLevel/CANifier_LowLevel.h"

#include "ctre/phoenix/Signals/MovingAverage.h"

#include "ctre/phoenix/Tasking/ILoopable.h"
#include "ctre/phoenix/Tasking/IProcessable.h"
#include "ctre/phoenix/Tasking/Schedulers/SequentialScheduler.h"
#include "ctre/phoenix/Tasking/Schedulers/ConcurrentScheduler.h"
#include "ctre/phoenix/Tasking/ButtonMonitor.h"

#include "ctre/phoenix/core/GadgeteerUartClient.h"

using namespace ctre;
using namespace ctre::phoenix;
using namespace ctre::phoenix::motion;
using namespace ctre::phoenix::motorcontrol;
using namespace ctre::phoenix::motorcontrol::can;
#ifndef CTR_EXCLUDE_WPILIB_CLASSES
using namespace ctre::phoenix::sensors;
#endif
using namespace ctre::phoenix::signals;
using namespace ctre::phoenix::tasking;
using namespace ctre::phoenix::tasking::schedulers;
