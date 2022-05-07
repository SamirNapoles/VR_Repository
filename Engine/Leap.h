/**
 * @file		leap.h
 * @brief	Minimal Leap Motion API wrapper
 *
 * @author	Achille Peternier (C) SUPSI [achille.peternier@supsi.ch]
 */
#pragma once



 //////////////
 // #INCLUDE //
 //////////////

	// GLM:
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>   

// C/C++:
#include <iostream>

// Leap Motion SDK:
#include <LeapC.h>


////////////////
// CLASS Leap //
////////////////

/**
 * @brief Leap Motion wrapper.
 */
class Leap
{
	//////////
public: //
//////////

	// Const/dest:	 
	Leap();
	~Leap();

	// Init/free:
	virtual bool init();
	virtual bool free();

	// Polling:
	virtual bool update();
	const LEAP_TRACKING_EVENT* getCurFrame() const;


	///////////	 
private:	//
///////////			

   // Leap Motion:
	LEAP_CONNECTION connection;
	LEAP_DEVICE_REF leapDevice;
	LEAP_TRACKING_EVENT curFrame;
	signed long long lastFrameId;
};
