#include <calculationComponents/vector2.h>

namespace s2d
{
	class BoxColliderPositionData
	{
	public:
		static int s_canCollideSpritesAround;
	public:
		BoxColliderPositionData();

		enum Position
		{
			Unknown = -1,
			Down = 0,
			Left,
			Right,
			Up,
			Collision
		};

		//This is the array containing the sprites which collide around us
		Position position[255 * 2];

		void resetPosition();	
		bool isEverythingUnknown();
		bool isEqual(s2d::BoxColliderPositionData::Position p);
	};
}