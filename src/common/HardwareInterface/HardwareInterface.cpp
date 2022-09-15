#include "HardwareInterface.h"
#include "../xmlParser/tinyxml2/tinyxml2.h"

HardwareInterface::HardwareInterface(const char*) {
	tinyxml2::XMLDocument doc;
	doc.LoadFile(config);
	tinyxml2::XMLElement* root = doc.RootElement();
	tinyxml2::XMLElement* device = root->FirstChildElement("Device");
	device_ = device->GetText();
	
	tinyxml2::XMLElement* parity = device->NextSiblingElement("Parity");
	settings_.parity = bool(parity->GetText());
}

bool writeData(const char* data, uint8_t size) {
	
}