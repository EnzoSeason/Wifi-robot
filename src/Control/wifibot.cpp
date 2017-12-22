// Includes, project.
#include "wifibot.hpp"
#include<math.h>

using namespace sy31;

Wifibot::Wifibot(double entrax, double wheelRadius)
{
  mEntrax = entrax;
  mWheelRadius = wheelRadius;
}

void
Wifibot::updateOdometry(double dt, double left, double right)
{
	mPosition.x = mPosition.x + getLinearSpeed(left, right) * dt *cos(mPosition.th);
	mPosition.y = mPosition.y + getLinearSpeed(left, right) * dt * sin(mPosition.th);
	mPosition.th = mPosition.th + getAngularSpeed(left,right)*dt;
}

void
Wifibot::resetOdometry()
{
	mPosition.x = (0.0);
	mPosition.y = (0.0);
	mPosition.th = (0.0);
}

void
Wifibot::setEntrax(double entrax)
{
  mEntrax = entrax;
}

void
Wifibot::setWheelRadius(double radius)
{
  mWheelRadius = radius;
}

double
Wifibot::getLinearSpeed(double left, double right) const
{
  return mWheelRadius*(left + right)/2;
}

double
Wifibot::getAngularSpeed(double left, double right) const
{
  return mWheelRadius*(right - left)/mEntrax;
}
