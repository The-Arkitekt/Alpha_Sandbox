#include "MoveCommandHelper.h"
#include <ncurses.h>
#include <iostream>
#include <string>

MoveCommandHelper::MoveCommandHelper()
	: Node("../../../config/MoveCommandHelper.xml")
	, moveCommandPublisher_("MoveCommand", "TextMessage", new TextMessagePubSubType())
{}

void MoveCommandHelper::init() {
	moveCommandPublisher_.init();
}

bool MoveCommandHelper::run() {
	// get user input
	int input;
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