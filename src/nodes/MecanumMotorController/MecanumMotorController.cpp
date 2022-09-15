#include "MecanumMotorController.h"
#include "../../common/HardwareInterface/HardwareInterface.h"

MecanumMotorController::MecanumMotorController()
	: Worker("../../../config/MecanumMotorController.xml")
	, motorSpeeds({0,0,0,0})
	, vectorMoveSubscriber_("VectorMove", "MoveVector", new MoveVectorPubSubType())
{}
void MecanumMotorController::init() {
	vectorMoveSubscriber_.init();
}

bool MecanumMotorController::run() {
	// get move vector message
	if (vectorMoveSubscriber_.getNumMessages() > 0) {
		MoveVector moveVector = vectorMoveSubscriber_.popOldestMessage();
		std::cout << moveVector.data()[0] << moveVector.data()[1] << moveVector.data()[2] << std::endl;
		// generate motor speeds
		std::array<int16_t, 4> motorSpeeds = generateMotorSpeeds(moveVector);

		// apply motor speeds
		applyMotorSpeeds(motorSpeeds);
	}
	return true;
}

std::array<int16_t, 4> MecanumMotorController::generateMotorSpeeds(MoveVector& msg) {	
	/*
	*						  ______
	*	motorSpeeds[0] --> []|		|[] <-- motorSpeeds[1]
	*						 |		|
	*						 |		|
	*	motorSpeeds[2] --> []|______|[] <-- motorSpeeds[3]
	* 
	*					^
	*					|
	*		msg[1] -->  |____>	<-- msg[1]
	*		msg[2] --> rotation
	* 
	*
	*	- brake <-- motor speed value set to 0
	*/
	// create transfer function matrix IGNORING ROTATION FOR NOW
	std::array<std::array<int16_t, 4>, 3> transferMatrix{{
														{1 ,-1, 1, -1},
														{1,  1, 1,  1},
														{0,  0, 0,  0}
														}};

	// create motor speeds array
	std::array<int16_t, 4> motorSpeeds{ 0,0,0,0 };

	// do matrix multiplication
	uint8_t i, j = 0;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 3; j++) {
			motorSpeeds[i] += msg.data()[j] * transferMatrix[j][i];
		}
	}
	return motorSpeeds;
}

bool MecanumMotorController::applyMotorSpeeds(std::array<int16_t, 4> motorSpeeds) {
	std::cout << "Applying Motor Speeds: [" << int(motorSpeeds[0]) << "],[" << int(motorSpeeds[1]) << "],[" <<int( motorSpeeds[2]) << "],[" << int(motorSpeeds[3]) << "]" << std::endl;
	
	// do hardware interface stuff here

	return true;
}

