#include "event.h"

s2d::Event::Event()
{
	key = s2d::KeyBoardCode::Unknown;
	type = s2d::Event::None;
}

bool s2d::Event::s_handle_other_events = true;

