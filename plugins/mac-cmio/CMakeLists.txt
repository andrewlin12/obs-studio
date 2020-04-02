project(mac-cmio)

#set(CMAKE_PREFIX_PATH "${QTDIR}")

find_library(COREMEDIA CoreMedia)
find_library(COREMEDIAIO CoreMediaIO)
find_library(IOKIT IOKit)
find_library(COREVIDEO CoreVideo)
find_library(COREAUDIO CoreAudio)
find_library(AUDIOUNIT AudioUnit)
find_library(COREFOUNDATION CoreFoundation)
find_library(IOSURF IOSurface)
find_library(COCOA Cocoa)

#find_package(Qt5Core REQUIRED)
#find_package(Qt5Widgets REQUIRED)

include_directories(${COREMEDIA}
                    ${COREMEDIAIO}
                    ${IOKIT}
                    ${COREVIDEO}
                    ${COREAUDIO}
                    ${AUDIOUNIT}
                    ${COREFOUNDATION}
                    ${IOSURF}
                    ${COCOA}
                    "${CMAKE_SOURCE_DIR}/UI/obs-frontend-api"
                    ./CoreAudio/PublicUtility/
                    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Base/
                    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Properties/
                    ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/Assistant/
                    ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/Assistant/Client/
                    ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/Assistant/MIG/generated/
                    ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/Assistant/Server/
                    ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/Assistant/Server/Device/
                    ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/
                    ./CoreMediaIO/PublicUtility/
                    ./CoreMediaIO/PublicUtility/CoreMediaAssistant/
                    ./CoreMediaIO/PublicUtility/CoreVideoAssistant/
                    ./CoreMediaIO/PublicUtility/DALAssistant/
                    ./CoreMediaIO/PublicUtility/IOKitAssistant/
                    ./CoreMediaIO/PublicUtility/IOStreamAssistant/
                    ./CoreMediaIO/PublicUtility/IOSurfaceAssistant/
                    ./CoreMediaIO/PublicUtility/IOVideoAssistant/
                    ./CoreMediaIO/PublicUtility/POSIXThreadAssistant/
                    ./IOVideo/
                    )

set(mac-cmio_HEADERS
    ./CoreAudio/PublicUtility/AUOutputBL.h
    ./CoreAudio/PublicUtility/CAAtomic.h
    ./CoreAudio/PublicUtility/CAAtomicStack.h
    ./CoreAudio/PublicUtility/CAAudioBufferList.h
    ./CoreAudio/PublicUtility/CAAudioChannelLayout.h
    ./CoreAudio/PublicUtility/CAAudioUnit.h
    ./CoreAudio/PublicUtility/CAAudioUnitOutputCapturer.h
    ./CoreAudio/PublicUtility/CAAudioValueRange.h
    ./CoreAudio/PublicUtility/CAAutoDisposer.h
    ./CoreAudio/PublicUtility/CABitOperations.h
    ./CoreAudio/PublicUtility/CABool.h
    ./CoreAudio/PublicUtility/CABufferList.h
    ./CoreAudio/PublicUtility/CABundleLocker.h
    ./CoreAudio/PublicUtility/CAByteOrder.h
    ./CoreAudio/PublicUtility/CACFArray.h
    ./CoreAudio/PublicUtility/CACFData.h
    ./CoreAudio/PublicUtility/CACFDictionary.h
    ./CoreAudio/PublicUtility/CACFDistributedNotification.h
    ./CoreAudio/PublicUtility/CACFMachPort.h
    ./CoreAudio/PublicUtility/CACFMessagePort.h
    ./CoreAudio/PublicUtility/CACFNumber.h
    ./CoreAudio/PublicUtility/CACFObject.h
    ./CoreAudio/PublicUtility/CACFPlugIn.h
    ./CoreAudio/PublicUtility/CACFPreferences.h
    ./CoreAudio/PublicUtility/CACFString.h
    ./CoreAudio/PublicUtility/CAComponent.h
    ./CoreAudio/PublicUtility/CAComponentDescription.h
    ./CoreAudio/PublicUtility/CADebugMacros.h
    ./CoreAudio/PublicUtility/CADebugPrintf.h
    ./CoreAudio/PublicUtility/CADebugger.h
    ./CoreAudio/PublicUtility/CAException.h
    ./CoreAudio/PublicUtility/CAExtAudioFile.h
    ./CoreAudio/PublicUtility/CAFilePathUtils.h
    ./CoreAudio/PublicUtility/CAGuard.h
    ./CoreAudio/PublicUtility/CAHostTimeBase.h
    ./CoreAudio/PublicUtility/CALogMacros.h
    ./CoreAudio/PublicUtility/CAMath.h
    ./CoreAudio/PublicUtility/CAMixMap.h
    ./CoreAudio/PublicUtility/CAMutex.h
    ./CoreAudio/PublicUtility/CAPThread.h
    ./CoreAudio/PublicUtility/CAProcess.h
    ./CoreAudio/PublicUtility/CAPropertyAddress.h
    ./CoreAudio/PublicUtility/CAReferenceCounted.h
    ./CoreAudio/PublicUtility/CARingBuffer.h
    ./CoreAudio/PublicUtility/CASettingsStorage.h
    ./CoreAudio/PublicUtility/CASharedLibrary.h
    ./CoreAudio/PublicUtility/CAStreamBasicDescription.h
    ./CoreAudio/PublicUtility/CAStreamRangedDescription.h
    ./CoreAudio/PublicUtility/CAThreadSafeList.h
    ./CoreAudio/PublicUtility/CATink.h
    ./CoreAudio/PublicUtility/CATokenMap.h
    ./CoreAudio/PublicUtility/CAVectorUnit.h
    ./CoreAudio/PublicUtility/CAVectorUnitTypes.h
    ./CoreAudio/PublicUtility/CAVolumeCurve.h
    ./CoreAudio/PublicUtility/CAXException.h
    ./CoreAudio/PublicUtility/MatrixMixerVolumes.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Base/CMIO_DP_Command.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Base/CMIO_DP_Control.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Base/CMIO_DP_ControlDictionary.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Base/CMIO_DP_Device.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Base/CMIO_DP_DeviceCommands.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Base/CMIO_DP_DeviceSettings.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Base/CMIO_DP_HogMode.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Base/CMIO_DP_Object.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Base/CMIO_DP_PlugIn.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Base/CMIO_DP_Property.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Base/CMIO_DP_Stream.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Base/CMIO_DP_UserInfo.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Properties/CMIO_DP_Property_Base.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Properties/CMIO_DP_Property_ClientSyncDiscontinuity.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Properties/CMIO_DP_Property_Clock.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Properties/CMIO_DP_Property_Deck.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Properties/CMIO_DP_Property_DeviceIsAlive.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Properties/CMIO_DP_Property_DeviceMaster.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Properties/CMIO_DP_Property_EndOfData.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Properties/CMIO_DP_Property_FirstOutputPresentationTimeStamp.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Properties/CMIO_DP_Property_FormatList.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Properties/CMIO_DP_Property_HogMode.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Properties/CMIO_DP_Property_NoData.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Properties/CMIO_DP_Property_OutputBuffers.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Properties/CMIO_DP_Property_SMPTETimeCallback.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Properties/CMIO_DP_Property_ScheduledOutputNotificationProc.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Properties/CMIO_DP_Property_SuspendedByUser.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/Assistant/CMIO_DPA_Sample_Shared.h
    # ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/Assistant/Client/CMIO_DPA_Sample_ClientExtras.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/Assistant/MIG/generated/CMIODPASampleServer.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/Assistant/Server/CMIO_DPA_Sample_Server_Assistant.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/Assistant/Server/CMIO_DPA_Sample_Server_Common.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/Assistant/Server/CMIO_DPA_Sample_Server_MIGInterface.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/Assistant/Server/CMIO_DPA_Sample_Server_VCamAssistant.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/Assistant/Server/Device/CMIO_DPA_Sample_Server_ClientStream.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/Assistant/Server/Device/CMIO_DPA_Sample_Server_Deck.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/Assistant/Server/Device/CMIO_DPA_Sample_Server_Device.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/Assistant/Server/Device/CMIO_DPA_Sample_Server_Frame.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/Assistant/Server/Device/CMIO_DPA_Sample_Server_Stream.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/Assistant/Server/Device/CMIO_DPA_Sample_Server_VCamDevice.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/Assistant/Server/Device/CMIO_DPA_Sample_Server_VCamInputStream.h
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/SamplePrefix.h
    ./CoreMediaIO/PublicUtility/CMIODebugMacros.h
    ./CoreMediaIO/PublicUtility/CMIODebugPrintf.h
    ./CoreMediaIO/PublicUtility/CMIO_BitField.h
    ./CoreMediaIO/PublicUtility/CMIO_Buffer.h
    # ./CoreMediaIO/PublicUtility/CMIO_CC_608_Scraper.h
    ./CoreMediaIO/PublicUtility/CMIO_PropertyAddress.h
    ./CoreMediaIO/PublicUtility/CMIO_SMPTETimeBase.h
    ./CoreMediaIO/PublicUtility/CoreMediaAssistant/CMIO_CMA_BlockBuffer.h
    ./CoreMediaIO/PublicUtility/CoreMediaAssistant/CMIO_CMA_FormatDescription.h
    ./CoreMediaIO/PublicUtility/CoreMediaAssistant/CMIO_CMA_SampleBuffer.h
    ./CoreMediaIO/PublicUtility/CoreMediaAssistant/CMIO_CMA_SimpleQueue.h
    ./CoreMediaIO/PublicUtility/CoreMediaAssistant/CMIO_CMA_Time.h
    ./CoreMediaIO/PublicUtility/CoreVideoAssistant/CMIO_CVA_Buffer.h
    ./CoreMediaIO/PublicUtility/CoreVideoAssistant/CMIO_CVA_Image_Buffer.h
    ./CoreMediaIO/PublicUtility/CoreVideoAssistant/CMIO_CVA_Pixel_Buffer.h
    # ./CoreMediaIO/PublicUtility/DALAssistant/CMIO_DALA_Control.h
    # ./CoreMediaIO/PublicUtility/DALAssistant/CMIO_DALA_Device.h
    # ./CoreMediaIO/PublicUtility/DALAssistant/CMIO_DALA_Object.h
    # ./CoreMediaIO/PublicUtility/DALAssistant/CMIO_DALA_Stream.h
    # ./CoreMediaIO/PublicUtility/DALAssistant/CMIO_DALA_System.h
    ./CoreMediaIO/PublicUtility/IOKitAssistant/CMIO_IOKA_Iterator.h
    ./CoreMediaIO/PublicUtility/IOKitAssistant/CMIO_IOKA_NotificationPort.h
    ./CoreMediaIO/PublicUtility/IOKitAssistant/CMIO_IOKA_Object.h
    ./CoreMediaIO/PublicUtility/IOKitAssistant/CMIO_IOKA_PowerNotificationPort.h
    ./CoreMediaIO/PublicUtility/IOKitAssistant/CMIO_IOKA_VirtualRange.h
    ./CoreMediaIO/PublicUtility/IOStreamAssistant/CMIO_IOSA_Assistance.h
    ./CoreMediaIO/PublicUtility/IOSurfaceAssistant/CMIO_SA_Assistance.h
    ./CoreMediaIO/PublicUtility/IOVideoAssistant/CMIO_IOVA_Assistance.h
    ./CoreMediaIO/PublicUtility/POSIXThreadAssistant/CMIO_PTA_Assistance.h
    ./CoreMediaIO/PublicUtility/POSIXThreadAssistant/CMIO_PTA_CFMachPortThread.h
    ./CoreMediaIO/PublicUtility/POSIXThreadAssistant/CMIO_PTA_NotificationPortThread.h
    ./IOVideo/IOVideoDebugMacros.h
    )

set(mac-cmio_SOURCES
    ./CoreAudio/PublicUtility/AUOutputBL.cpp
    ./CoreAudio/PublicUtility/CAAudioBufferList.cpp
    ./CoreAudio/PublicUtility/CAAudioChannelLayout.cpp
    ./CoreAudio/PublicUtility/CAAudioChannelLayoutObject.cpp
    ./CoreAudio/PublicUtility/CAAudioUnit.cpp
    ./CoreAudio/PublicUtility/CAAudioValueRange.cpp
    ./CoreAudio/PublicUtility/CABufferList.cpp
    ./CoreAudio/PublicUtility/CABundleLocker.cpp
    ./CoreAudio/PublicUtility/CACFArray.cpp
    ./CoreAudio/PublicUtility/CACFDictionary.cpp
    ./CoreAudio/PublicUtility/CACFDistributedNotification.cpp
    ./CoreAudio/PublicUtility/CACFMachPort.cpp
    ./CoreAudio/PublicUtility/CACFMessagePort.cpp
    ./CoreAudio/PublicUtility/CACFNumber.cpp
    ./CoreAudio/PublicUtility/CACFPreferences.cpp
    ./CoreAudio/PublicUtility/CACFString.cpp
    ./CoreAudio/PublicUtility/CAComponent.cpp
    ./CoreAudio/PublicUtility/CAComponentDescription.cpp
    ./CoreAudio/PublicUtility/CADebugMacros.cpp
    ./CoreAudio/PublicUtility/CADebugPrintf.cpp
    ./CoreAudio/PublicUtility/CADebugger.cpp
    ./CoreAudio/PublicUtility/CAFilePathUtils.cpp
    ./CoreAudio/PublicUtility/CAGuard.cpp
    ./CoreAudio/PublicUtility/CAHostTimeBase.cpp
    ./CoreAudio/PublicUtility/CAMutex.cpp
    ./CoreAudio/PublicUtility/CAPThread.cpp
    ./CoreAudio/PublicUtility/CAPersistence.cpp
    ./CoreAudio/PublicUtility/CAProcess.cpp
    ./CoreAudio/PublicUtility/CARingBuffer.cpp
    ./CoreAudio/PublicUtility/CASettingsStorage.cpp
    ./CoreAudio/PublicUtility/CASharedLibrary.cpp
    ./CoreAudio/PublicUtility/CAStreamBasicDescription.cpp
    ./CoreAudio/PublicUtility/CAStreamRangedDescription.cpp
    ./CoreAudio/PublicUtility/CAVectorUnit.cpp
    ./CoreAudio/PublicUtility/CAVolumeCurve.cpp
    ./CoreAudio/PublicUtility/CAXException.cpp
    ./CoreAudio/PublicUtility/MatrixMixerVolumes.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Base/CMIO_DP_Command.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Base/CMIO_DP_Control.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Base/CMIO_DP_ControlDictionary.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Base/CMIO_DP_Device.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Base/CMIO_DP_DeviceCommands.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Base/CMIO_DP_DeviceSettings.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Base/CMIO_DP_HardwarePlugInInterface.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Base/CMIO_DP_HogMode.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Base/CMIO_DP_Object.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Base/CMIO_DP_PlugIn.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Base/CMIO_DP_Property.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Base/CMIO_DP_Stream.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Base/CMIO_DP_UserInfo.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Properties/CMIO_DP_Property_Base.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Properties/CMIO_DP_Property_ClientSyncDiscontinuity.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Properties/CMIO_DP_Property_Clock.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Properties/CMIO_DP_Property_Deck.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Properties/CMIO_DP_Property_DeviceIsAlive.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Properties/CMIO_DP_Property_DeviceMaster.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Properties/CMIO_DP_Property_EndOfData.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Properties/CMIO_DP_Property_FirstOutputPresentationTimeStamp.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Properties/CMIO_DP_Property_FormatList.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Properties/CMIO_DP_Property_HogMode.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Properties/CMIO_DP_Property_NoData.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Properties/CMIO_DP_Property_OutputBuffers.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Properties/CMIO_DP_Property_SMPTETimeCallback.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Properties/CMIO_DP_Property_ScheduledOutputNotificationProc.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/DP/Properties/CMIO_DP_Property_SuspendedByUser.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/IOV/PlugIn/CMIO_DP_IOV_Control.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/IOV/PlugIn/CMIO_DP_IOV_ControlDictionary.cpp
    # ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/Assistant/Client/CMIO_DPA_Sample_ClientExtras.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/Assistant/MIG/generated/CMIODPASampleServer.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/Assistant/MIG/CMIODPASample.defs
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/Assistant/Server/CMIO_DPA_Sample_Server_Assistant.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/Assistant/Server/CMIO_DPA_Sample_Server_MIGInterface.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/Assistant/Server/CMIO_DPA_Sample_Server_VCamAssistant.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/Assistant/Server/CMIO_DPA_Sample_VCamServer.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/Assistant/Server/Device/CMIO_DPA_Sample_Server_ClientStream.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/Assistant/Server/Device/CMIO_DPA_Sample_Server_Deck.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/Assistant/Server/Device/CMIO_DPA_Sample_Server_Device.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/Assistant/Server/Device/CMIO_DPA_Sample_Server_Frame.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/Assistant/Server/Device/CMIO_DPA_Sample_Server_Stream.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/Assistant/Server/Device/CMIO_DPA_Sample_Server_VCamDevice.cpp
    ./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/Assistant/Server/Device/CMIO_DPA_Sample_Server_VCamInputStream.cpp
    ./CoreMediaIO/PublicUtility/CMIODebugMacros.cpp
    ./CoreMediaIO/PublicUtility/CMIODebugPrintf.cpp
    # ./CoreMediaIO/PublicUtility/CMIO_CC_608_Scraper.cpp
    ./CoreMediaIO/PublicUtility/CMIO_SMPTETimeBase.cpp
    # ./CoreMediaIO/PublicUtility/DALAssistant/CMIO_DALA_Control.cpp
    # ./CoreMediaIO/PublicUtility/DALAssistant/CMIO_DALA_Device.cpp
    # ./CoreMediaIO/PublicUtility/DALAssistant/CMIO_DALA_Object.cpp
    # ./CoreMediaIO/PublicUtility/DALAssistant/CMIO_DALA_Stream.cpp
    # ./CoreMediaIO/PublicUtility/DALAssistant/CMIO_DALA_System.cpp
    ./CoreMediaIO/PublicUtility/IOKitAssistant/CMIO_IOKA_NotificationPort.cpp
    ./CoreMediaIO/PublicUtility/IOStreamAssistant/CMIO_IOSA_Assistance.cpp
    ./CoreMediaIO/PublicUtility/IOSurfaceAssistant/CMIO_SA_Assistance.cpp
    ./CoreMediaIO/PublicUtility/IOVideoAssistant/CMIO_IOVA_Assistance.cpp
    ./CoreMediaIO/PublicUtility/POSIXThreadAssistant/CMIO_PTA_CFMachPortThread.cpp
    ./CoreMediaIO/PublicUtility/POSIXThreadAssistant/CMIO_PTA_NotificationPortThread.cpp
    ./plugin-main.cpp
)

set_source_files_properties(./CoreMediaIO/DeviceAbstractionLayer/Devices/Sample/Assistant/MIG/CMIODPASample.defs
    PROPERTIES
        XCODE_FILE_ATTRIBUTES "Client"
)

add_library(mac-cmio MODULE
	${mac-cmio_SOURCES}
	${mac-cmio_HEADERS}
#	${Qt5Core_INCLUDES}
#	${Qt5Widgets_INCLUDES}
    )

target_link_libraries(mac-cmio
	libobs
    obs-frontend-api
    ${COREMEDIA}
    ${COREMEDIAIO}
    ${IOKIT}
    ${COREVIDEO}
	${COREAUDIO}
	${AUDIOUNIT}
	${COREFOUNDATION}
	${IOSURF}
	${COCOA}
#	Qt5::Core
#	Qt5::Widgets
    )

install_obs_plugin_with_data(mac-cmio data)
