/*
	    File: CMIO_DPA_SampleVCam_Server_VCamAssistant.h
	Abstract: Server which handles all the IPC between the various Sample DAL PlugIn instances.
	 Version: 1.2

*/

#if !defined(__CMIO_DPA_Sample_Server_VCamAssistant_h__)
#define __CMIO_DPA_Sample_Server_VCamAssistant_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "CMIO_DPA_Sample_Server_Assistant.h"

namespace CMIO {
namespace DPA {
namespace Sample {
namespace Server {
class VCamAssistant : public Assistant {
	// Construction/Destruction
public:
	static VCamAssistant *Instance();
	Device *GetDevice();
	void SetStartStopHandlers(std::function<void()> &start_lambda,
				  std::function<void()> &stop_lambda);
	virtual kern_return_t
	StartStream(mach_port_t client, UInt64 guid, mach_port_t messagePort,
		    CMIOObjectPropertyScope scope,
		    CMIOObjectPropertyElement element) override;
	virtual kern_return_t
	StopStream(mach_port_t client, UInt64 guid,
		   CMIOObjectPropertyScope scope,
		   CMIOObjectPropertyElement element) override;

public:
	VCamAssistant();
	static VCamAssistant *sInstance;

private:
	void CreateDevices();
	std::function<void()> mStartLambda;
	std::function<void()> mStopLambda;
};
}
}
}
}
#endif
