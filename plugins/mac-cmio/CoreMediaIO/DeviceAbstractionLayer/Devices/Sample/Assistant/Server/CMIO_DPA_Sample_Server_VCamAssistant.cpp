/*
	    File: CMIO_DPA_Sample_Server_VCamAssistant.cpp
	Abstract: Server which handles all the IPC between the various Sample DAL PlugIn instances.
	 Version: 1.2

*/

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Self Include
#include "CMIO_DPA_Sample_Server_VCamAssistant.h"

// Internal Includes
#include "CMIO_DPA_Sample_Server_VCamDevice.h"

#pragma mark -
namespace CMIO {
namespace DPA {
namespace Sample {
namespace Server {
#pragma mark Static Globals
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Static Globals
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
VCamAssistant *VCamAssistant::sInstance = nullptr;

#pragma mark -
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// VCamAssistant()
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
VCamAssistant::VCamAssistant() : Assistant()
{
	CreateDevices();
}

#pragma mark -
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Instance()
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
VCamAssistant *VCamAssistant::Instance()
{
	if (!sInstance) {
		sInstance = new VCamAssistant();
	}

	return sInstance;
}

Device *VCamAssistant::GetDevice()
{
	return *mDevices.begin();
}

void VCamAssistant::SetStartStopHandlers(std::function<void()> &start_lambda,
					 std::function<void()> &stop_lambda)
{
	mStartLambda = start_lambda;
	mStopLambda = stop_lambda;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CreateDevices()
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void VCamAssistant::CreateDevices()
{
	// Grab the mutex for the Assistant's state
	CAMutex::Locker locker(mStateMutex);

	// Get the current device count
	UInt32 deviceCount = (UInt32)mDevices.size();

	// Create the new device
	VCamDevice *device = new VCamDevice();

	// Add it to the set of discovered devices whose capabilities are known
	mDevices.insert(device);

	// If any devices were successfully added, notify interested clients that a state change has taken place so they can call UpdateDeviceStates() at their convenience
	if (deviceCount != mDevices.size()) {
		// Send out the devices state changed message
		for (ClientNotifiers::iterator it =
			     mDeviceStateNotifiers.begin();
		     it != mDeviceStateNotifiers.end(); ++it)
			SendDeviceStatesChangedMessage((*it).second);

		// All the 'send-once' rights are now used up, so erase everything in the multimap
		mDeviceStateNotifiers.erase(mDeviceStateNotifiers.begin(),
					    mDeviceStateNotifiers.end());
	}
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// StartStream()
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
kern_return_t VCamAssistant::StartStream(Client client, UInt64 guid,
					 mach_port_t messagePort,
					 CMIOObjectPropertyScope scope,
					 CMIOObjectPropertyElement element)
{
	kern_return_t kern_return = Assistant::StartStream(
		client, guid, messagePort, scope, element);
	mStartLambda();
	return kern_return;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// StopStream()
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
kern_return_t VCamAssistant::StopStream(Client client, UInt64 guid,
					CMIOObjectPropertyScope scope,
					CMIOObjectPropertyElement element)
{
	kern_return_t kern_return =
		Assistant::StopStream(client, guid, scope, element);
	mStopLambda();
	return kern_return;
}
}
}
}
}
