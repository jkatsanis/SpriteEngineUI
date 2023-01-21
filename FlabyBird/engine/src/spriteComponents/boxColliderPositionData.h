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
		Position position[100];

		void resetPosition();	

		/// <summary>
		/// This method returns true if the sprite is solid while colliding in a sprite in a certain direction
		/// </summary>
		bool isNotCollidingInAnyDirection();

		/// <summary>
		/// returns true if the direction passed exists in the array
		/// </summary>
		/// <param name="p"></param>
		/// <returns></returns>
		bool isEqual(s2d::BoxColliderPositionData::Position p);
	};
}