// Copyright Oliver Scott 2019

#include "TankTrack.h"

void UTankTrack::SetThrottle(float Throttle)
{
	//TODO: Clamp Throttle Value, to limit player speed.
	auto ForceApplied = GetForwardVector() * Throttle *TrackMaxDrivingForce;
	auto ForceLocation = GetComponentLocation();
	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}