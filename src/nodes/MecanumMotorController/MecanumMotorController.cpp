#include "MecanumMotorController.h"
#include <typeinfo>

MecanumMotorController::MecanumMotorController()
	: Worker("../../../config/MecanumMotorController.xml")
	, motorSpeeds_({0,0,0,0})
	, serial("STM32")
	, vectorMoveSubscriber_("VectorMove", "MoveVector", new MoveVectorPubSubType())
{}
void MecanumMotorController::init() {
	vectorMoveSubscriber_.init();
}

bool MecanumMotorController::run() {
	// get move vector message
	if (vectorMoveSubscriber_.getNumMessages() > 0) {
		MoveVector moveVector = vectorMoveSubscriber_.popOldestMessage();
		
		// generate motor speeds
		generateMotorSpeeds(moveVector);

		// apply motor speeds
		if (!applyMotorSpeeds())
			return false;
	}
	return true;
}

void MecanumMotorController::generateMotorSpeeds(MoveVector& msg) {	
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
	std::array <std::array<uint8_t, 4>, 3> transferMatrix{{
														{1, uint8_t(-1), 1, uint8_t(-1)},
														{1,  1, 1,  1},
														{0,  0, 0,  0}
												  }};
		
	// create motor speeds array
	motorSpeeds_.data[0] = 0;
	motorSpeeds_.data[1] = 0;
	motorSpeeds_.data[2] = 0;
	motorSpeeds_.data[3] = 0;

	// do matrix multiplication
	uint8_t i, j = 0;
	int16_t tmp;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 3; j++) {
			// check for overflow (the highest SIGNED value) 
			tmp = int8_t(motorSpeeds_.data[i]) + (int8_t(msg.data()[j]) * int8_t(transferMatrix[j][i]));
			if (tmp > std::numeric_limits<int8_t>::max())
				motorSpeeds_.data[i] = std::numeric_limits<int8_t>::max();
			else if (tmp < std::numeric_limits<int8_t>::min())
				motorSpeeds_.data[i] = std::numeric_limits<int8_t>::min();
			else
				motorSpeeds_.data[i] += msg.data()[j] * transferMatrix[j][i];
		}
	}
}

bool MecanumMotorController::applyMotorSpeeds() {
	std::cout << "Applying Motor Speeds: [" << int(motorSpeeds_.data[0]) << "],[" << int(motorSpeeds_.data[1]) << "],[" << int(motorSpeeds_.data[2]) << "],[" << int(motorSpeeds_.data[3]) << "]" << std::endl;
	
	// do hardware interface stuff here
	serial.writeData((motorSpeeds_.data));


	return true;
}

