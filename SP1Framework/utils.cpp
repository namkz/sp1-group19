#include "utils.h"

COORD convertCoordAnchor(COORD c, std::string text, ANCHOR anchor)
{
	switch(anchor % 4)
	{
	case 0:
		return c;
	case 1:
		return COORD (c.X + (text.length() / 2)), c.Y);
	case 2:
		return COORD (c.X + text.length(), c.Y);
	}
}