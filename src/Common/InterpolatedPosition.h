#pragma once

#include "Common.h"
#include "MessageIdentifiers.h"

class InterpolatedPosition
{
public:
	InterpolatedPosition();
	InterpolatedPosition(float initalTargetX, float initalTargetY, float initialRotation, RakNet::Time currentTime);
	~InterpolatedPosition();

	void setNewTarget(float newTargetX, float newTargetY, float targetRotation, RakNet::Time currentTime, RakNet::Time newTargetTime);
	void setNewTarget(PlayerVariables playerState, RakNet::Time currentTime, RakNet::Time newTargetTime);
	PlayerVariables getNextPosition(RakNet::Time currentTime);

private:
	PlayerVariables start;
	PlayerVariables current;
	PlayerVariables target;

	RakNet::Time startTime;
	RakNet::Time targetTime;
};

