#include "MoveCommandHelper.h"
#include <ncurses.h>
#include <iostream>
#include <string>

MoveCommandHelper::MoveCommandHelper()
	: Node("../../../config/MoveCommandHelper.xml")
	, moveCommandPublisher_("MoveCommand", "TextMessage", new TextMessagePubSubType())
{}

void MoveCommandHelper::configNode(tinyxml2::XMLElement* root) {
	// Nothing else needed to config
	return;
}

void MoveCommandHelper::initNode() {
	moveCommandPublisher_.init();
}

bool MoveCommandHelper::runNode() {
	// get user input
	int input = 0;
	TextMessage msg;
	
	initscr();
	clear();
	cbreak();

	printw("\nEnter Command: \n> ");
	flushinp();
	input = getch(); 
	refresh();

	if (input == 'q') {
		endwin();
		return false;
	}

	msg.message(std::string(1, char(input)));
	if (!moveCommandPublisher_.publish(msg)) {
		printw( "Failed to send message!\n");
	}
	return true;
}