#ifndef MECANUMMOTORCONTROLLER_H
#define MECANUMMOTORCONTROLLER_H

#include <array>

#include "../../base/Worker.h"
#include "../../../messages/MoveVector/MoveVectorPubSubTypes.h"
#include "../../common/HardwareInterface/SerialInterface.h"

class MecanumMotorController : public Worker {
public:
	MecanumMotorController();

private:
	void init();
	bool run();
	void generateMotorSpeeds(MoveVector&);
	bool applyMotorSpeeds();

	struct speedArray {
		char data[4];
	}motorSpeeds_;

	SerialInterface serial;

	Subscriber<MoveVector, MoveVectorPubSubType>vectorMoveSubscriber_;
};

#endif // MECANUMMOTORCONTROLLER_H