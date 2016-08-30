#include "InterpolatedPosition.h"

InterpolatedPosition::InterpolatedPosition()
{
	
}

InterpolatedPosition::InterpolatedPosition(float initalTargetX, float initalTargetY, float initialRotation, RakNet::Time currentTime)
{
	target.pos.x = initalTargetX;
	target.pos.y = initalTargetY;
	target.rotation = initialRotation;

	start = target;
	current = target;

	startTime = currentTime;
	targetTime = currentTime;
}

InterpolatedPosition::~InterpolatedPosition()
{
}

void InterpolatedPosition::setNewTarget(float newTargetX, float newTargetY, float targetRotation, RakNet::Time currentTime, RakNet::Time newTargetTime)
{
	//if (newTargetX != target.pos.x || newTargetY != target.pos.y)

	start = current;
	target.pos.x = newTargetX;
	target.pos.y = newTargetY;
	target.rotation = targetRotation;

	//When the ships rotatio changed from 360 to 0 it would freak out
	//thanks StackExchange
	if (target.rotation - start.rotation > 180.0)
	{
		start.rotation += 360.0f;
	}
	else if (target.rotation - start.rotation < -180.0)
	{
		start.rotation -= 360.0f;
	}

	startTime = currentTime;
	targetTime = newTargetTime;
}

void InterpolatedPosition::setNewTarget(PlayerVariables playerState, RakNet::Time currentTime, RakNet::Time newTargetTime)
{
	start = current;
	target = playerState;

	//When the ships rotatio changed from 360 to 0 it would freak out
	//Thanks StackExchange
	if (target.rotation - start.rotation > 180.0)
	{
		start.rotation += 360.0f;
	}
	else if (target.rotation - start.rotation < -180.0)
	{
		start.rotation -= 360.0f;
	}

	startTime = currentTime;
	targetTime = newTargetTime;
}

PlayerVariables InterpolatedPosition::getNextPosition(RakNet::Time currentTime)
{
	if (targetTime == startTime)
	{
		return target;
	}

	float t = (float)(currentTime-startTime) / (float)(targetTime-startTime);

	if (t >= 1.0f)
	{
		return target;
	}
	else if (t < 0)
	{
		return target;
	}
	else
	{
		float oneMinusT = (1 - t);
		float newX = oneMinusT*start.pos.x + t*target.pos.x;
		float newY = oneMinusT*start.pos.y + t*target.pos.y;
		float newRotation = oneMinusT*start.rotation + t*target.rotation;

		current.pos.x = newX;
		current.pos.y = newY;
		current.rotation = newRotation;

		return current;
	}
}