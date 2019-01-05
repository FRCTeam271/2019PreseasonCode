#include "ctre/Phoenix.h"

using namespace ctre::phoenix;
using namespace ctre::phoenix::motorcontrol::can;
/**
 * Constructor
 * @param deviceNumber [0,62]
 */
VictorSPX::VictorSPX(int deviceNumber) :
    BaseMotorController(deviceNumber | 0x01040000) {
		//HAL_Report(HALUsageReporting::kResourceType_CTRE_future1, deviceNumber + 1);
	}


/**
 * Gets all PID set persistant settings.
 *
 * @param pid               Object with all of the PID set persistant settings
 * @param pidIdx            0 for Primary closed-loop. 1 for auxiliary closed-loop.
 * @param timeoutMs
 *              Timeout value in ms. If nonzero, function will wait for
 *              config success and report an error if it times out.
 *              If zero, no blocking or checking is performed.
 */
ctre::phoenix::ErrorCode VictorSPX::ConfigurePID(const VictorSPXPIDSetConfiguration &pid, int pidIdx, int timeoutMs) {
    ErrorCollection errorCollection;

    //------ sensor selection ----------//

	errorCollection.NewError(BaseConfigurePID(pid, pidIdx, timeoutMs));
    errorCollection.NewError(ConfigSelectedFeedbackSensor(pid.selectedFeedbackSensor, pidIdx, timeoutMs));


	return errorCollection._worstError;
}
/**
 * Gets all PID set persistant settings (overloaded so timeoutMs is 50 ms
 * and pidIdx is 0).
 *
 * @param pid               Object with all of the PID set persistant settings
 */
void VictorSPX::GetPIDConfigs(VictorSPXPIDSetConfiguration &pid, int pidIdx, int timeoutMs)
{
	BaseGetPIDConfigs(pid, pidIdx, timeoutMs);
	pid.selectedFeedbackSensor = (RemoteFeedbackDevice) ConfigGetParameter(eFeedbackSensorType, pidIdx, timeoutMs);

}

/**
 * Configures all peristant settings.
 *
 * @param allConfigs        Object with all of the persistant settings
 * @param timeoutMs
 *              Timeout value in ms. If nonzero, function will wait for
 *              config success and report an error if it times out.
 *              If zero, no blocking or checking is performed.
 *
 * @return Error Code generated by function. 0 indicates no error.
 */
ErrorCode VictorSPX::ConfigAllSettings(const VictorSPXConfiguration &allConfigs, int timeoutMs) {
    ErrorCollection errorCollection;

	errorCollection.NewError(BaseConfigAllSettings(allConfigs, timeoutMs));

	//------ remote limit switch ----------//
	errorCollection.NewError(ConfigForwardLimitSwitchSource(allConfigs.forwardLimitSwitchSource, allConfigs.forwardLimitSwitchNormal, allConfigs.forwardLimitSwitchDeviceID, timeoutMs));

	errorCollection.NewError(ConfigReverseLimitSwitchSource(allConfigs.reverseLimitSwitchSource, allConfigs.reverseLimitSwitchNormal, allConfigs.reverseLimitSwitchDeviceID, timeoutMs));


	//--------PIDs---------------//

    errorCollection.NewError(ConfigurePID(allConfigs.primaryPID, 0, timeoutMs));

    errorCollection.NewError(ConfigurePID(allConfigs.auxilaryPID, 1, timeoutMs));

    errorCollection.NewError(ConfigSensorTerm(SensorTerm::SensorTerm_Sum0, allConfigs.sum_0, timeoutMs));

    errorCollection.NewError(ConfigSensorTerm(SensorTerm::SensorTerm_Sum1, allConfigs.sum_1, timeoutMs));

    errorCollection.NewError(ConfigSensorTerm(SensorTerm::SensorTerm_Diff0, allConfigs.diff_0, timeoutMs));

    errorCollection.NewError(ConfigSensorTerm(SensorTerm::SensorTerm_Diff1, allConfigs.diff_1, timeoutMs));


    return errorCollection._worstError;
}
/**
 * Gets all persistant settings.
 *
 * @param allConfigs        Object with all of the persistant settings
 * @param timeoutMs
 *              Timeout value in ms. If nonzero, function will wait for
 *              config success and report an error if it times out.
 *              If zero, no blocking or checking is performed.
 */
void VictorSPX::GetAllConfigs(VictorSPXConfiguration &allConfigs, int timeoutMs) {

	BaseGetAllConfigs(allConfigs, timeoutMs);

	GetPIDConfigs(allConfigs.primaryPID, 0, timeoutMs);
	GetPIDConfigs(allConfigs.auxilaryPID, 1, timeoutMs);
    allConfigs.sum_0 = (RemoteFeedbackDevice) ConfigGetParameter(eSensorTerm, 0, timeoutMs);
    allConfigs.sum_1 = (RemoteFeedbackDevice) ConfigGetParameter(eSensorTerm, 1, timeoutMs);
    allConfigs.diff_0 = (RemoteFeedbackDevice) ConfigGetParameter(eSensorTerm, 2, timeoutMs);
    allConfigs.diff_1 = (RemoteFeedbackDevice) ConfigGetParameter(eSensorTerm, 3, timeoutMs);

	allConfigs.forwardLimitSwitchSource = (RemoteLimitSwitchSource) ConfigGetParameter(eLimitSwitchSource, 0, timeoutMs);
	allConfigs.reverseLimitSwitchSource = (RemoteLimitSwitchSource) ConfigGetParameter(eLimitSwitchSource, 1, timeoutMs);

}

