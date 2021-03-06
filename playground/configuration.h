#ifndef __CONFIGURATION__
#define __CONFIGURATION__

#include "WorldExt.h"

namespace Enki {
	typedef enum {
		MODE_GUI,
		MODE_NO_VIEWER
	} Mode;

	/**
	 * Environmental temperature.  This value is used to initialise the heat
	 * model.  The heat model is composed of a rectangular grid.  Heat flows from
	 * hotter grid cells to colder grid cells.  Cells in the border exchange heat
	 * with the outside world whose temperature is given by this variable.
	 */
	extern double env_temp;

	/**
	 * Arena radius in centimetres.
	 */
	extern int radius;

	extern double heat_scale;

	extern int heat_border_size;

	extern double maxVibration;

	extern Mode mode;

	extern double timerPeriodSec;

	/**
	 * Process configuration options that can be passed in the command line
	 * or be in the 'Playground.cfg' file.
	 */
	void processConfiguration (int argc, char *argv[]);

	/**
	 * Create a world for the corresponding mode.
	 */
	WorldExt *createWorld ();

}

#endif // __CONFIGURATION__
