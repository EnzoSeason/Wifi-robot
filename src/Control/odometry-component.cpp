// Includes, project.
#include "odometry-component.hpp"

// Includes, standard.
#define _USE_MATH_DEFINES
#include <cmath>
#include <maps.hpp>

using namespace sy31;

const double NB_TICKS_PER_WHEEL_TURN = 2448;
const double WIFIBOT_ENTRAX_IN_METER = 0.4;
const double WIFIBOT_RADIUS_IN_METER = 0.0625;
const double TWOPI = 2 * M_PI;

////////////////////////////////////////////////////////////////
// RTMaps - Input
////////////////////////////////////////////////////////////////
MAPS_BEGIN_INPUTS_DEFINITION(OdometryComponent)
  MAPS_INPUT("iVelocity", MAPS::FilterInteger32, MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

////////////////////////////////////////////////////////////////
// RTMaps - Output
////////////////////////////////////////////////////////////////
MAPS_BEGIN_OUTPUTS_DEFINITION(OdometryComponent)
  MAPS_OUTPUT("oState", MAPS::Float64, NULL, NULL, 3)
MAPS_END_OUTPUTS_DEFINITION

////////////////////////////////////////////////////////////////
// RTMaps - Properties
////////////////////////////////////////////////////////////////
MAPS_BEGIN_PROPERTIES_DEFINITION(OdometryComponent)
MAPS_END_PROPERTIES_DEFINITION

////////////////////////////////////////////////////////////////
// RTMaps - Actions
////////////////////////////////////////////////////////////////
MAPS_BEGIN_ACTIONS_DEFINITION(OdometryComponent)
  MAPS_ACTION("ResetOdometry", OdometryComponent::ResetOdometry)
MAPS_END_ACTIONS_DEFINITION

////////////////////////////////////////////////////////////////
// RTMaps - Definition
////////////////////////////////////////////////////////////////
MAPS_COMPONENT_DEFINITION(
  OdometryComponent, "OdometryComponent", "1.0", 128,
  MAPS::Threaded|MAPS::Sequential,MAPS::Threaded,
  1, // Nb of inputs
  1, // Nb of outputs
  0, // Nb of properties
  1) // Nb of actions

////////////////////////////////////////////////////////////////
// RTMaps - Birth
////////////////////////////////////////////////////////////////
void
OdometryComponent::Birth()
{
  // Initialize the robot.
	mPreviousTimestamp = MAPS::CurrentTime();
  // Initialize the timestamp.
}

////////////////////////////////////////////////////////////////
// RTMaps - Core
////////////////////////////////////////////////////////////////
void
OdometryComponent::Core() 
{
  // Start reading from the inputs.
  MAPSIOElt* input = StartReading(Input(0));
  if (input == NULL)
    return;
  // Check the input size, we're waiting for (vleft, vright).
  if (input->VectorSize() != 2)
    Error("input.size invalid, must contain two elements");

  // Compute the elapsed time in seconds.
  
  double dt = (MAPS::CurrentTime() -  mPreviousTimestamp)*0.000001;
  // Get the wheel speeds.
  double vLeft  = static_cast<double>(input->Integer32(0));
  double vRight = static_cast<double>(input->Integer32(1));
  // Convert the wheel speeds.
  double Left = (vLeft * TWOPI)/(2448*0.041);
  double Right = (vRight*TWOPI)/(2448*0.041);
  // Update the odometry.
  mPreviousTimestamp = MAPS::CurrentTime();
  mWifibot.updateOdometry(dt,Left,Right);
  // Start writing in the outputs.
  MAPSIOElt* output = StartWriting(Output(0));
  // Set the output size: (x, y, th).
  output->VectorSize() = 3;
  // Fill the output vector.
  output->Float64(0) = mWifibot.getPosition().x;
  output->Float64(1) = mWifibot.getPosition().y;
  output->Float64(2) = mWifibot.getPosition().th;
  // Writing is complete.
  StopWriting(output);

  // Update the previous timestamp.
  // ...

}

////////////////////////////////////////////////////////////////
// RTMaps - Death
////////////////////////////////////////////////////////////////
void
OdometryComponent::Death()
{
  // Couic! :(
}

void
OdometryComponent::reset()
{
  mWifibot.resetOdometry();
}

void
OdometryComponent::ResetOdometry(MAPSModule *module, int actionNb)
{
	if (MAPS::IsRunning())
	{
		OdometryComponent* oc = (OdometryComponent*) module;
    oc->reset();
	}
}
