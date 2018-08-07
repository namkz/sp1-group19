#ifndef _UTILS_H
#define _UTILS_H

#include "Framework\console.h"
#include <string>

enum ANCHOR {
	TOP_LEFT = 0, TOP_MIDDLE, TOP_RIGHT,
	MIDDLE_LEFT = 4, CENTRE, MIDDLE_RIGHT,
	BOTTOM_LEFT = 8, BOTTOM_MIDDLE, BOTTOM_RIGHT
};

void convertCoordAnchor(COORD, std::string, ANCHOR);
#endif