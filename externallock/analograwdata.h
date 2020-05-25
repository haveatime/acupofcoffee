#ifndef ANALOG__RAW__DATA__H
#define ANALOG__RAW__DATA__H

#include "common/portableexternallock.h"

class AnalogRawData
{
public:
	AnalogRawData( );

	void SetValue( float val );

private:
	float value;
};

class AnalogRawDataThreadSafe: public abcplat::basic_lockable_adapter<abcplat::recursive_mutex>
{
public:
	AnalogRawDataThreadSafe();

	void SetValue(float val);

private:
    abcplat::externally_locked<AnalogRawData, AnalogRawDataThreadSafe > shareddata;
};

#endif
