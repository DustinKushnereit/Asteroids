#include "Block.h"
#include "gameObject.h"

Block::Block(float screenWidth, float screenHeight)
{
	screenX = screenWidth;
	screenY = 800.0f;
	mIsAlive = true;
}

void Block::Init()
{
	mIsAlive = true;

	blockWidth = 20;
	blockHeight = 100;

	//blockBuffer.loadFromFile("Content/blockHit.wav");
	blockSound.setVolume(80);
	blockSound.setBuffer(blockBuffer);

	velocity.x = 0.1f;
	velocity.y = 0.1f;

	mBlock.setSize(sf::Vector2f(blockWidth, blockHeight));
	mBlock.setOrigin(0, 0);
	mBlock.setOutlineThickness(3);
	mBlock.setOutlineColor(sf::Color::Green);
	mBlock.setPosition(500, 200);
}

void Block::Draw(sf::RenderWindow *window)
{
	if (mIsAlive == true)
		window->draw(mBlock);
}

void Block::setFillColor(sf::Color newColor)
{
	mBlock.setFillColor(newColor);
}

void Block::setOutlineColor(sf::Color newColor)
{
	mBlock.setOutlineColor(newColor);
}


void Block::CheckCollisions(Ball* m_ball)
{
	if (mIsAlive == true)
	{
		if (mBlock.getPosition().x <= 0)
			mBlock.setPosition(sf::Vector2f(0, mBlock.getPosition().y));

		if (mBlock.getPosition().x >= screenX - blockWidth)
			mBlock.setPosition(sf::Vector2f(screenX - blockWidth, mBlock.getPosition().y));

		if ((m_ball->gameBall.getPosition().x - m_ball->GetBallRadius() <= mBlock.getPosition().x + blockWidth + 1) && (m_ball->gameBall.getPosition().x + m_ball->GetBallRadius() >= mBlock.getPosition().x))
		{
			if ((m_ball->gameBall.getPosition().y - m_ball->GetBallRadius() <= mBlock.getPosition().y + blockHeight) && (m_ball->gameBall.getPosition().y + m_ball->GetBallRadius() >= mBlock.getPosition().y))
			{
				if (ballLastPos.x + m_ball->GetBallRadius() >= mBlock.getPosition().x && ballLastPos.x - m_ball->GetBallRadius() <= mBlock.getPosition().x + blockWidth)
				{
					if (m_ball->Velocity().y >= 0)
					{
						m_ball->Velocity(sf::Vector2f(m_ball->Velocity().x, m_ball->Velocity().y  * -1));
						m_ball->Velocity(sf::Vector2f(m_ball->Velocity().x * 1.03f, m_ball->Velocity().y * 1.03f));
						m_ball->gameBall.setPosition(m_ball->gameBall.getPosition().x, mBlock.getPosition().y - m_ball->GetBallRadius());
						mIsAlive = false;
					}
				}
				else if (ballLastPos.y - m_ball->GetBallRadius() <= mBlock.getPosition().y + blockHeight && ballLastPos.y + m_ball->GetBallRadius()>= mBlock.getPosition().y)
				{
					m_ball->Velocity(sf::Vector2f(m_ball->Velocity().x * -1, m_ball->Velocity().y));
					m_ball->Velocity(sf::Vector2f(m_ball->Velocity().x * 1.05f, m_ball->Velocity().y * 1.05f));
					mIsAlive = false;
				}
				blockSound.play();
			}
		}
		ballLastPos = m_ball->gameBall.getPosition();
	}

	/*if (mIsAlive == true)
	{
		if (mBlock.getPosition().x <= 0)
			mBlock.setPosition(sf::Vector2f(0, mBlock.getPosition().y));

		if (mBlock.getPosition().x >= screenX - blockWidth)
			mBlock.setPosition(sf::Vector2f(screenX - blockWidth, mBlock.getPosition().y));

		if ((m_ball->mBall.getPosition().x - m_ball->GetBallRadius() <= mBlock.getPosition().x + blockWidth + 1) && (m_ball->mBall.getPosition().x + m_ball->GetBallRadius() >= mBlock.getPosition().x))
		{
			if ((m_ball->mBall.getPosition().y - m_ball->GetBallRadius() <= mBlock.getPosition().y + blockHeight) && (m_ball->mBall.getPosition().y + m_ball->GetBallRadius() >= mBlock.getPosition().y))
			{
				if (mBallLastPos.x + m_ball->GetBallRadius() >= mBlock.getPosition().x && mBallLastPos.x - m_ball->GetBallRadius() <= mBlock.getPosition().x + blockWidth)
				{
					if (m_ball->Velocity().y >= 0)
					{
						m_ball->Velocity(sf::Vector2f(m_ball->Velocity().x, m_ball->Velocity().y  * -1));
						m_ball->Velocity(sf::Vector2f(m_ball->Velocity().x * 1.03f, m_ball->Velocity().y * 1.03f));
						m_ball->mBall.setPosition(m_ball->mBall.getPosition().x, mBlock.getPosition().y - m_ball->GetBallRadius());
						mIsAlive = false;
					}
				}
				else if (mBallLastPos.y - m_ball->GetBallRadius() <= mBlock.getPosition().y + blockHeight && mBallLastPos.y + m_ball->GetBallRadius() >= mBlock.getPosition().y)
				{
					m_ball->Velocity(sf::Vector2f(m_ball->Velocity().x * -1, m_ball->Velocity().y));
					m_ball->Velocity(sf::Vector2f(m_ball->Velocity().x * 1.05f, m_ball->Velocity().y * 1.05f));
					mIsAlive = false;
				}
				blockSound.play();
			}
		}
		mBallLastPos = m_ball->mBall.getPosition();
	}*/
}