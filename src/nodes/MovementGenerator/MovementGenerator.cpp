#include "MovementGenerator.h"
#include <iostream>

MovementGenerator::MovementGenerator()
	: Worker("../../../config/MovementGenerator.xml")
	, moveCommandSubscriber_("MoveCommand", "TextMessage", new TextMessagePubSubType())
	, vectorMovePublisher_("VectorMove", "MoveVector", new MoveVectorPubSubType())
{}

void MovementGenerator::configWorker(tinyxml2::XMLElement* root) {
	// add MovementGenerator specific configurations here
	return;
}

void MovementGenerator::initWorker() {
	moveCommandSubscriber_.init();
	vectorMovePublisher_.init();
}

bool MovementGenerator::runWorker() {
	// get move command messages
	if (moveCommandSubscriber_.getNumMessages() > 0) {
		TextMessage msg = moveCommandSubscriber_.popOldestMessage();

		// generate move vector
		MoveVector moveVector = generateVector(msg);

		// publish move vector
		vectorMovePublisher_.publish(moveVector);
	}
	return true;
}

MoveVector MovementGenerator::generateVector(TextMessage& msg) {
	MoveVector vector;
	if (msg.message().size() > 1) {
		vector.data() = { 0,0,0 };
		return vector;
	}
	switch (msg.message()[0]) {
	case('w'):
		vector.data() = { 0, 50, 0 };
		break;
	case('d'):
		vector.data() = { 50, 0, 0 };
		break;
	case('s'):
		vector.data() = { 0, -50, 0 };
		break;
	case('a'):
		vector.data() = { -50, 0, 0 };
		break;
	default:
		vector.data() = { 0,0,0 };
	}
	return vector;
}

