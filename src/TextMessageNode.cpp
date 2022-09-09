#include <iostream>

#include "TextMessageNode.h"

TextMessageNode::TextMessageNode() {
	_textMessagePublisher = nullptr;
	_textMessageSubscriber = nullptr;
}

TextMessageNode::~TextMessageNode() {
	if (_initialized) {
		delete(_textMessagePublisher);
	}
}

bool TextMessageNode::init() {
	if (_initialized) {
		return false;
	}

	_textMessagePublisher = new Publisher<TextMessage, TextMessagePubSubType>("TextMessageTopic", "TextMessage", new TextMessagePubSubType());
	_textMessageSubscriber = new Subscriber<TextMessage, TextMessagePubSubType>("TextMessageTopic", "TextMessage", new TextMessagePubSubType());

	_textMessagePublisher->init();
	_textMessageSubscriber->init();

	_initialized = true;
	return true;
}

bool TextMessageNode::run() {
	if (!_initialized) {
		return false;
	}

	TextMessage tm_;
	tm_.index(0);
	tm_.message("Hello");

	uint32_t samples = 10;
	uint32_t sent = 0;

	while (sent < samples) {
		
		if (_textMessagePublisher->publish(tm_)) {
			sent++;
			std::cout << "Message: " << tm_.message() << " with index: " << tm_.index()
				<< " SENT" << std::endl;
			tm_.index(tm_.index() + 1);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		while (_textMessageSubscriber->getNumMessages() > 0) {
			TextMessage received = _textMessageSubscriber->popOldestMessage();
			std::cout << "Message: " << received.message() << ", with index: " << received.index()
				<< " RECEIVED" << std::endl;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	}
	return true;
}