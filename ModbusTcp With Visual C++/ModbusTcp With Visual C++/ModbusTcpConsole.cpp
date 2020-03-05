#include "stdafx.h"
#include "ModbusTcpMaster.h"
#include "ModbusTcpConsole.h"

using WORD = unsigned short;
using DINT = signed int;

// Designed By Hoang Van Luu 
// Skype: katllu
// Mobile: 0909.886.483
// E-mail: hoangluu.automation@gmail.com
int main() {

	WORD startAddress = 0; // Start address of ML Area.
	array<DINT> ^values = gcnew array<DINT>(4);  // values is an array of 4 integers
	values[0] = 1200;
	values[1] = 1500;
	values[2] = 300;
	values[3] = 400;

	System::String ^IP = "192.168.1.1"; // IP Address of MP2300S Controller.
	WORD Port = 502; // Port of MP2300S Controller.

	MP2300SController ^objMP2300SController = gcnew MP2300SController(IP, Port); // Create new instance of MP2300SController.
	objMP2300SController->Connect(); // Connect to MP2300S Controller.
	objMP2300SController->WriteML(startAddress, values); // Write an array of 4 integers to MP2300S Controller.
	objMP2300SController->Disconnect(); // Disconnect with MP2300S Controller.
	return 0;
}

