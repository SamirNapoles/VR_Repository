/**
 * @file		leap.cpp
 * @brief	Minimal Leap Motion API wrapper
 *
 * @author	Achille Peternier (C) SUPSI [achille.peternier@supsi.ch]
 */
#pragma once


 //////////////
 // #INCLUDE //
 //////////////

    // Header:
#include "Leap.h"

////////////////////////
// BODY OF CLASS Leap //
////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Constructor.
 */
Leap::Leap() : connection { nullptr }, curFrame{ nullptr },
lastFrameId{ 0 }
{}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Destructor.
 */
Leap::~Leap()
{}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Initializes Leap Motion connection.
 * @return TF
 */
bool Leap::init()
{
    // Create and open connection:   
    if (LeapCreateConnection(nullptr, &connection) != eLeapRS_Success)
    {
        std::cout << "[ERROR] Unable to connect to the Leap Motion" << std::endl;
        return false;
    }
    if (LeapOpenConnection(connection) != eLeapRS_Success)
    {
        std::cout << "[ERROR] Unable to open connection to the Leap Motion" << std::endl;
        LeapDestroyConnection(connection);
        connection = nullptr;
        return false;
    }

    // Get device (only one supported):
    const uint32_t timeout = 1000;
    uint32_t computedArraySize = 0;
    for (uint32_t retry = 3; retry > 0; retry--)
    {
        LEAP_CONNECTION_MESSAGE msg;
        LeapPollConnection(connection, timeout, &msg);

        eLeapRS ret = LeapGetDeviceList(connection, nullptr, &computedArraySize);
        if (ret == eLeapRS_NotConnected)
            continue;
        else
            if (ret == eLeapRS_Success)
                if (computedArraySize)
                    break;
    }

    std::cout << "Number of available devices: " << computedArraySize << std::endl;
    if (computedArraySize != 1)
    {
        std::cout << "[ERROR] Invalid number of devices (must be 1)" << std::endl;
        LeapCloseConnection(connection);
        LeapDestroyConnection(connection);
        connection = nullptr;
        return false;
    }

    // Get ref to device:      
    if (LeapGetDeviceList(connection, &leapDevice, &computedArraySize) != eLeapRS_Success)
    {
        std::cout << "[ERROR] Unable to get ref to device" << std::endl;
        LeapCloseConnection(connection);
        LeapDestroyConnection(connection);
        connection = nullptr;
        return false;
    }

    // Done:
    return true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Releases Leap Motion resources.
 * @return TF
 */
bool Leap::free()
{
    // Clean up Leap Motion:
    LeapCloseConnection(connection);
    LeapDestroyConnection(connection);
    connection = nullptr;

    // Done:
    return true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Acquire new events.
 * @return TF
 */
bool Leap::update()
{
    // Poll for new events:
    const uint32_t timeout = 1000;

    LEAP_CONNECTION_MESSAGE msg;
    if (LeapPollConnection(connection, timeout, &msg) != eLeapRS_Success)
    {
        std::cout << "[ERROR] Unable to poll connection" << std::endl;
        return false;
    }

    // Only tracking events are processed:
    if (msg.type != eLeapEventType_Tracking)
        return false;

    // Update only when necessary:
    if (msg.tracking_event->tracking_frame_id > this->lastFrameId)
    {
        lastFrameId = msg.tracking_event->tracking_frame_id;
        curFrame = *msg.tracking_event;
    }

    // Done:
    return true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Gets a pointer to the last updated frame.
 * @return pointer to last frame, or nullptr on error
 */
const LEAP_TRACKING_EVENT* Leap::getCurFrame() const
{
    return &curFrame;
}