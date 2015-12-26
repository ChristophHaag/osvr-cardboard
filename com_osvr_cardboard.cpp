/** @file
    @brief Comprehensive example: Implementation of a dummy Hardware Detect
   Callback that creates a dummy device when it is "detected"

    @date 2014

    @author
    Sensics, Inc.
    <http://sensics.com/osvr>
*/

// Copyright 2014 Sensics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Internal Includes
#include <osvr/PluginKit/PluginKit.h>
#include <osvr/PluginKit/AnalogInterfaceC.h>
#include <osvr/PluginKit/TrackerInterfaceC.h>

// Generated JSON header file
#include "com_osvr_cardboard_json.h"

// Library/third-party includes
// - none

// Standard includes
#include <iostream>
#include <unistd.h>//TODO: remove
// Anonymous namespace to avoid symbol collision
namespace {

class CardboardDevice {
  public:
    CardboardDevice(OSVR_PluginRegContext ctx) : m_myVal(0) {
        /// Create the initialization options
        OSVR_DeviceInitOptions opts = osvrDeviceCreateInitOptions(ctx);

        
        osvrDeviceAnalogConfigure(opts, &m_analog, 10);
	osvrDeviceTrackerConfigure(opts, &m_tracker);

        /// Create the device token with the options
        m_dev.initAsync(ctx, "CardBoard Server", opts);

        /// Send JSON descriptor
        m_dev.sendJsonDescriptor(com_osvr_cardboard_json);

        /// Register update callback
        m_dev.registerUpdateCallback(this);
    }

    float up = 0.01;
    OSVR_ReturnCode update() {
        /// This dummy loop just wastes time, to pretend to be your plugin
        /// blocking to wait for the arrival of data. It should be completely
        /// removed from your plugin.
        usleep(16000);
        /// End time-waster loop.

        /// Make up some dummy data that changes to report.

        if (m_myVal > .25 || m_myVal < - .25) {
            up = - up;
        }
	m_myVal = (m_myVal + up);

        //std::cout << "PLUGIN: Report " << m_myVal << std::endl;

        OSVR_Pose3 hmd_pose;
        hmd_pose.translation.data[0] = m_myVal;
        hmd_pose.translation.data[1] = m_myVal;
        hmd_pose.translation.data[2] = m_myVal;
        hmd_pose.rotation.data[0] = 0;
        hmd_pose.rotation.data[1] = 0;
        hmd_pose.rotation.data[2] = 0;
        hmd_pose.rotation.data[3] = 0;

        osvrDeviceTrackerSendPose(m_dev, m_tracker, &hmd_pose, 0);
	
        /// Report the value of channel 0
        osvrDeviceAnalogSetValue(m_dev, m_analog, 0, 0);
        osvrDeviceAnalogSetValue(m_dev, m_analog, 0, 1);
        osvrDeviceAnalogSetValue(m_dev, m_analog, 0, 2);
        osvrDeviceAnalogSetValue(m_dev, m_analog, 0, 3);
        osvrDeviceAnalogSetValue(m_dev, m_analog, 0, 4);
        osvrDeviceAnalogSetValue(m_dev, m_analog, 0, 5);
        osvrDeviceAnalogSetValue(m_dev, m_analog, 0, 6);
        osvrDeviceAnalogSetValue(m_dev, m_analog, 0, 7);
        osvrDeviceAnalogSetValue(m_dev, m_analog, 0, 8);
        osvrDeviceAnalogSetValue(m_dev, m_analog, 0, 9);

        return OSVR_RETURN_SUCCESS;
    }

  private:
    osvr::pluginkit::DeviceToken m_dev;
    OSVR_AnalogDeviceInterface m_analog;
    OSVR_TrackerDeviceInterface m_tracker;
    double m_myVal;
};

class HardwareDetection {
  public:
    HardwareDetection() : m_found(false) {}
    OSVR_ReturnCode operator()(OSVR_PluginRegContext ctx) {

        std::cout << "PLUGIN: Got a hardware detection request" << std::endl;
        if (!m_found) {
            std::cout << "PLUGIN: We have detected our fake device! Doing "
                         "setup stuff!"
                      << std::endl;
            m_found = true;

            /// Create our device object
            osvr::pluginkit::registerObjectForDeletion(
                ctx, new CardboardDevice(ctx));
        }
        return OSVR_RETURN_SUCCESS;
    }

  private:
    /// @brief Have we found our device yet? (this limits the plugin to one
    /// instance)
    bool m_found;
};
} // namespace

OSVR_PLUGIN(com_osvr_Cardboard) {
    osvr::pluginkit::PluginContext context(ctx);

    /// Register a detection callback function object.
    context.registerHardwareDetectCallback(new HardwareDetection());

    return OSVR_RETURN_SUCCESS;
}
