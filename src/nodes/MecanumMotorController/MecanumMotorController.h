#ifndef MECANUMMOTORCONTROLLER_H
#define MECANUMMOTORCONTROLLER_H

#include <vector>

#include "../../base/Worker.h"
#include "../../../messages/MoveVector/MoveVectorPubSubTypes.h"
#include "../../common/HardwareInterface/SerialInterface.h"

class MecanumMotorController : public Worker {
public:
	MecanumMotorController();

private:
	void configWorker(tinyxml2::XMLElement*);
	void initWorker();
	bool runWorker();

	void generateMotorSpeeds(MoveVector&);
	bool applyMotorSpeeds();

	std::vector<int8_t> motorSpeeds_{0,0,0,0};

	SerialInterface serial;

	Subscriber<MoveVector, MoveVectorPubSubType>vectorMoveSubscriber_;
};

#endif // MECANUMMOTORCONTROLLER_H