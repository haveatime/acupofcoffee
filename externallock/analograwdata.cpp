#include "analograwdata.h"

AnalogRawData::AnalogRawData()
	:value()
{
}

void AnalogRawData::SetValue(float val)
{
	value = val;
}

AnalogRawDataThreadSafe::AnalogRawDataThreadSafe()
	: shareddata(*this)
{
}

void AnalogRawDataThreadSafe::SetValue(float val)
{
    abcplat::strict_lock<AnalogRawDataThreadSafe> guard(*this);
	shareddata.get(guard).SetValue(val);
}

