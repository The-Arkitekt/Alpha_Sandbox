#include "MecanumMotorController.h"
#include <typeinfo>

MecanumMotorController::MecanumMotorController()
	: Worker("../../../config/MecanumMotorController.xml")
	, serial("STM32")
	, vectorMoveSubscriber_("VectorMove", "MoveVector", new MoveVectorPubSubType())
{}

void MecanumMotorController::configWorker(tinyxml2::XMLElement*) {
	serial.config();
}

void MecanumMotorController::initWorker() {
	vectorMoveSubscriber_.init();
}

bool MecanumMotorController::runWorker() {
	// get move vector message
	if (vectorMoveSubscriber_.getNumMessages() > 0) {
		MoveVector moveVector = vectorMoveSubscriber_.popOldestMessage();
		
		// generate motor speeds
		generateMotorSpeeds(moveVector);

		// apply motor speeds
		if (!applyMotorSpeeds()) {
			return false;
		}
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
	std::vector <std::vector<int8_t>> transferMatrix
	{ 
		{1, -1, 1, -1},
		{1,  1, 1,  1},
		{0,  0, 0,  0}
	};
	// create motor speeds vector
	motorSpeeds_[0] = 0;
	motorSpeeds_[1] = 0;
	motorSpeeds_[2] = 0;
	motorSpeeds_[3] = 0;

	// do matrix multiplication
	uint8_t i, j = 0;
	int16_t tmp;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 3; j++) {
			// check for overflow (the highest SIGNED value) 
			tmp = motorSpeeds_[i] + (msg.data()[j] * transferMatrix[j][i]);
			if (tmp > std::numeric_limits<int8_t>::max())
				motorSpeeds_[i] = std::numeric_limits<int8_t>::max();
			else if (tmp < std::numeric_limits<int8_t>::min())
				motorSpeeds_[i] = std::numeric_limits<int8_t>::min();
			else
				motorSpeeds_[i] += msg.data()[j] * transferMatrix[j][i];
		}

	}
}

bool MecanumMotorController::applyMotorSpeeds() {
	std::cout << "Applying Motor Speeds: [" << int(motorSpeeds_[0]) << "],[" << int(motorSpeeds_[1]) << "],[" << int(motorSpeeds_[2]) << "],[" << int(motorSpeeds_[3]) << "]" << std::endl;
	//initialize serial port to have a max 100 ms blocking and no byte minimum
	if (serial.initPort(10, 0) < 0)
		return false;

	if (!serial.writeData(motorSpeeds_))
		return false;

	serial.closePort();
	/*
	std::vector<int8_t> readBuf;
	if (!serial.readData(&readBuf, 4))
		return false;

	int i = 0;
	std::cout << "Num bytes read: " << readBuf.size() << ", message: ";
	for (i = 0; i < readBuf.size(); i++) {
		std::cout << int(readBuf[i]);
	}
	std::cout << std::endl;	
	*/
	return true;
}

