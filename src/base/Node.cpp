#include "Node.h"

std::atomic_int Node::_counter_{ 0 };

Node::Node()
	: nodeID_(++_counter_)
	, initialized_(false)
{}

int Node::getNodeID() {
	int ret = nodeID_;
	return ret;
}