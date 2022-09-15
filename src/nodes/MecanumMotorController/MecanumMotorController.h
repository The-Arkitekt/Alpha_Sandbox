#ifndef MECANUMMOTORCONTROLLER_H
#define MECANUMMOTORCONTROLLER_H

#include <array>

#include "../../base/Worker.h"
#include "../../../messages/MoveVector/MoveVectorPubSubTypes.h"

class MecanumMotorController : public Worker {
public:
	MecanumMotorController();

private:
	void init();
	bool run();
	std::array<int16_t, 4> generateMotorSpeeds(MoveVector&);
	bool applyMotorSpeeds(std::array<int16_t, 4>);


	std::array<int16_t, 4> motorSpeeds;
	Subscriber<MoveVector, MoveVectorPubSubType>vectorMoveSubscriber_;
};

#endif // MECANUMMOTORCONTROLLER_H