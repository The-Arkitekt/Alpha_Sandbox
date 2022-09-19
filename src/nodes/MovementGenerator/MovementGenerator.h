#ifndef MOVEMENTGENERATOR_H
#define MOVEMENTGENERATOR_H

#include <array>

#include "../../base/Worker.h"
#include "../../../messages/TextMessage/TextMessagePubSubTypes.h"
#include "../../../messages/MoveVector/MoveVectorPubSubTypes.h"

class MovementGenerator : public Worker {
public:
	MovementGenerator();

private:
	void configWorker(tinyxml2::XMLElement*);
	void initWorker();
	bool runWorker();

	MoveVector generateVector(TextMessage&);

	Subscriber<TextMessage, TextMessagePubSubType>moveCommandSubscriber_;
	Publisher<MoveVector, MoveVectorPubSubType>vectorMovePublisher_;
};


#endif