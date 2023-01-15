#include "Testing.h"

//User classes

void Testing::start()
{
	this->m_spriteSpeed = 800;
	this->m_score = 0;
	thisSprite = s2d::Sprite::getSpriteByName("Bird");
	up = s2d::Sprite::getSpriteByName("up");
	down = s2d::Sprite::getSpriteByName("down");
	camera = &s2d::GameObject::camera;
}

void Testing::update()
{
	std::string score = "Score: " + std::to_string(s2d::Time::fps);
	ImGui::SetCursorPos(ImVec2(800, 200));
	ImGui::Text(score.c_str());
	up->transform.position.x -= 700 * s2d::Time::deltaTime;
	down->transform.position.x -= 700 * s2d::Time::deltaTime;

	if (thisSprite->collider.collidingSprite != nullptr)
	{
		if (thisSprite->collider.collidingSprite->name == "up"
			|| thisSprite->collider.collidingSprite->name == "down")
		{
			//s/2d::Sprite* spr = new s2d::Sprite("name", s2d::Vector2(0, 0), "assets\\Sprites\\gameOver.png", true);
			//spr->sortingLayerIndex = 2;
			//spr->renderInstant();
		}
	}
	m_timeToPressAgain += s2d::Time::deltaTime;
	if (s2d::Input::onKeyPress(s2d::KeyBoardCode::Space) && m_timeToPressAgain > 0.1f)
	{
		m_timeToPressAgain = 0;
		thisSprite->physicsBody.velocity.y = 0;
		s2d::Physics::addForce(thisSprite, s2d::Vector2(0, 1), 0.2f);
	}
	if (up->transform.position.x < -1000)
	{
		up->transform.position.x = 1004.6;
		down->transform.position.x = 1004.6;

		std::random_device rd;  // Will be used to obtain a seed for the random number engine
		std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
		std::uniform_int_distribution<> dis(400, 800);
		this->m_score++;

		int random_number = dis(gen);

		up->transform.position.y = 0 - random_number;
		down->transform.position.y = up->transform.position.y + 1500;

	}

	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::A))
	{
		thisSprite->transform.position.x -= this->m_spriteSpeed * s2d::Time::deltaTime;
	}
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::W))
	{
		thisSprite->transform.position.y += this->m_spriteSpeed * s2d::Time::deltaTime;
	}
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::S))
	{
		thisSprite->transform.position.y -= this->m_spriteSpeed * s2d::Time::deltaTime;
	}
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::D))
	{
		thisSprite->transform.position.x += this->m_spriteSpeed * s2d::Time::deltaTime;
	}


}
 