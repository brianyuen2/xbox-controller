#include "XBoxController.h"
#include <iostream>
#include <cmath>

#define MAX_RANGE 32767
using namespace std;




// Constructor which sets the private variables of the xinput wrapper and id to the arguments
GamePad::XBoxController::XBoxController(XInputWrapper* xinput, DWORD id) {
	this->xinput = xinput;
	this->controllerId = id;
}

// Functions
// All of the bool functions use xinputgetstate to check the state of the gamepad and checks if the bit value is true.
bool GamePad::XBoxController::IsConnected() {
	if (xinput->XInputGetState(controllerId, &State) == ERROR_DEVICE_NOT_CONNECTED) {
		return false;
	}
	else {
		return true;
	}
}

DWORD GamePad::XBoxController::GetControllerId() {
	return XBoxController::controllerId; // works
}

bool GamePad::XBoxController::PressedA() {
	xinput->XInputGetState(controllerId, &State);
	if (State.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
		return true;
	}
	else {
		return false;
	}
}

bool GamePad::XBoxController::PressedB() {
	xinput->XInputGetState(controllerId, &State);
	if (State.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
		return true;
	}
	else {
		return false;
	}
}

bool GamePad::XBoxController::PressedX() {
	xinput->XInputGetState(controllerId, &State);
	if (State.Gamepad.wButtons & XINPUT_GAMEPAD_X) {
		return true;
	}
	else {
		return false;
	}
}

bool GamePad::XBoxController::PressedY() {
	xinput->XInputGetState(controllerId, &State);
	if (State.Gamepad.wButtons & XINPUT_GAMEPAD_Y) {
		return true;
	}
	else {
		return false;
	}
}

bool GamePad::XBoxController::PressedLeftShoulder() {
	xinput->XInputGetState(controllerId, &State);
	if (State.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
		return true;
	}
	else {
		return false;
	}
}

bool GamePad::XBoxController::PressedRightShoulder() {
	xinput->XInputGetState(controllerId, &State);
	if (State.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		return true;
	}
	else {
		return false;
	}
}

bool GamePad::XBoxController::PressedRightThumb() {
	xinput->XInputGetState(controllerId, &State);
	if (State.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) {
		return true;
	}
	else {
		return false;
	}
}

bool GamePad::XBoxController::PressedLeftThumb() {
	xinput->XInputGetState(controllerId, &State);
	if (State.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) {
		return true;
	}
	else {
		return false;
	}
}

bool GamePad::XBoxController::PressedStart() {
	xinput->XInputGetState(controllerId, &State);
	if (State.Gamepad.wButtons & XINPUT_GAMEPAD_START) {
		return true;
	}
	else {
		return false;
	}
}

bool GamePad::XBoxController::PressedBack() {
	xinput->XInputGetState(controllerId, &State);
	if (State.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) {
		return true;
	}
	else {
		return false;
	}
}

bool GamePad::XBoxController::PressedDownDpad() {
	xinput->XInputGetState(controllerId, &State);
	if (State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
		return true;
	}
	else {
		return false;
	}
}

bool GamePad::XBoxController::PressedUpDpad() {
	xinput->XInputGetState(controllerId, &State);
	if (State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) {
		return true;
	}
	else {
		return false;
	}
}

bool GamePad::XBoxController::PressedLeftDpad() {
	xinput->XInputGetState(controllerId, &State);
	if (State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) {
		return true;
	}
	else {
		return false;
	}
}

bool GamePad::XBoxController::PressedRightDpad() {
	xinput->XInputGetState(controllerId, &State);
	if (State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) {
		return true;
	}
	else {
		return false;
	}
}

// The vibrate function sets the motors to whatever speed is put in as arguments
void GamePad::XBoxController::Vibrate(WORD left, WORD right) {
	_XINPUT_VIBRATION Vibration;
	Vibration.wLeftMotorSpeed = left;
	Vibration.wRightMotorSpeed = right;
	xinput->XInputSetState(controllerId, &Vibration);
}

// Both trigger location functions use xinputgetstate to check the location of the trigger
BYTE GamePad::XBoxController::RightTriggerLocation() {
	xinput->XInputGetState(0, &State);
	//cout << State.Gamepad.bRightTrigger << endl;
	return State.Gamepad.bRightTrigger;
}

BYTE GamePad::XBoxController::LeftTriggerLocation() {
	xinput->XInputGetState(controllerId, &State);
	//cout << State.Gamepad.bRightTrigger << endl;
	return State.Gamepad.bLeftTrigger;
}

//Both thumb location functions use xinputgetstate to get the x and y coordinates, a local coordinate object is created to store the locations, 
// the coordinate is then checked to determine if it is in the deadzone radius, if it is, it sets both coordinates to zero, if it is not,
// the radius is scaled using my formula and then broken up into x and y parts before getting returned
GamePad::Coordinate GamePad::XBoxController::LeftThumbLocation() {
	xinput->XInputGetState(controllerId, &State);
	GamePad::Coordinate coordinateLeft = GamePad::Coordinate::Coordinate(State.Gamepad.sThumbLX, State.Gamepad.sThumbLY);
	double x = coordinateLeft.GetX();
	double y = coordinateLeft.GetY();

	if ((x * x + y * y) < deadzoneRadius*deadzoneRadius) {
		coordinateLeft.SetX(0);
		coordinateLeft.SetY(0);
	}
	else {
		double radius = sqrt(x*x + y * y);

		if (radius > MAX_RANGE) {
			radius = MAX_RANGE;
		}
		double angle = atan2(y, x);
		double radiusScaled = (MAX_RANGE / double(MAX_RANGE - deadzoneRadius))*(radius - deadzoneRadius);
		x = radiusScaled * cos(angle);
		y = radiusScaled * sin(angle);
		coordinateLeft.SetX(x);
		coordinateLeft.SetY(y);
	}
	return coordinateLeft;
}

GamePad::Coordinate GamePad::XBoxController::RightThumbLocation() {
	xinput->XInputGetState(controllerId, &State);
	GamePad::Coordinate coordinateRight = GamePad::Coordinate::Coordinate(State.Gamepad.sThumbRX, State.Gamepad.sThumbRY);
	double x = coordinateRight.GetX();
	double y = coordinateRight.GetY();
	
	if ((x * x + y * y) < deadzoneRadius*deadzoneRadius) {
		coordinateRight.SetX(0);
		coordinateRight.SetY(0);
	}
	else {
		double radius = sqrt(x*x + y * y);
		
		if (radius > MAX_RANGE) {
			radius = MAX_RANGE;
		}
		double angle = atan2(y,x);
		double radiusScaled = (MAX_RANGE / double(MAX_RANGE - deadzoneRadius))*(radius - deadzoneRadius);
		x = radiusScaled * cos(angle);
		y = radiusScaled * sin(angle);
		coordinateRight.SetX(x);
		coordinateRight.SetY(y);
	}
	return coordinateRight;
}

// This function sets the private variable deadzoneradius to the argument put in
void GamePad::XBoxController::SetDeadzone(unsigned int radius) {
	deadzoneRadius = radius;
}

