// author Emmett Byrne
// date 28-11-17
#include "AnimateLicense.h"
#include <math.h>



AnimateLicense::AnimateLicense(sf::Vector2f tPos, sf::Vector2f sPos) :
	m_paused{ false },
	m_currentKeyFrame{ 0 }
{

	if (!m_font.loadFromFile("ASSETS\\FONTS\\AYearWithoutRain.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}
	m_title.setFont(m_font);
	m_sub.setFont(m_font);

	m_title.setString("TitleText");
	m_sub.setString("SubText");

	m_title.setStyle(sf::Text::Bold);

	m_title.setCharacterSize(30);
	m_sub.setCharacterSize(20);

	m_title.setOrigin(m_title.getLocalBounds().width / 2, m_title.getLocalBounds().height / 2);
	m_sub.setOrigin(m_sub.getLocalBounds().width / 2, m_sub.getLocalBounds().height / 2);

	m_keyframe[0].m_startPositionTitle = tPos;
	m_keyframe[0].m_startPositionSub = sPos;

	m_titlePosition = tPos;
	m_subPosition = sPos;
}

AnimateLicense::~AnimateLicense()
{
}

void AnimateLicense::update(sf::Time t)
{
	if (!m_paused)
	{
		if (m_keyframe[m_currentKeyFrame].m_timePassed < m_keyframe[m_currentKeyFrame].m_duration)
		{
			m_keyframe[m_currentKeyFrame].m_timePassed += t;

			if (m_keyframe[m_currentKeyFrame].m_type == AnimType::move)
			{
				move(t);
			}
			if (m_keyframe[m_currentKeyFrame].m_type == AnimType::somersault)
			{
				somersault(t);
			}
		}
		else
		{
			if (m_currentKeyFrame < 3)
			{
				m_titlePosition = m_keyframe[m_currentKeyFrame].m_targetPositionTitle;
				m_subPosition = m_keyframe[m_currentKeyFrame].m_targetPositionSub;

				m_keyframe[m_currentKeyFrame + 1].m_startPositionTitle = m_keyframe[m_currentKeyFrame].m_targetPositionTitle;
				m_keyframe[m_currentKeyFrame + 1].m_startPositionSub = m_keyframe[m_currentKeyFrame].m_targetPositionSub;

				m_currentKeyFrame++;

				if (m_keyframe[m_currentKeyFrame].m_type == AnimType::somersault)
				{
					setupSomersault();
				}

				m_title.setRotation(0);
			}
			else
			{
				m_paused = true;
			}
		}
	}
	else
	{
		std::cout << "Paused" << std::endl;
	}


	m_title.setPosition(m_titlePosition);
	m_sub.setPosition(m_subPosition);
}

void AnimateLicense::render(sf::RenderWindow & window)
{
	window.draw(m_title);
	window.draw(m_sub);
}

void AnimateLicense::setupSomersault()
{
	m_keyframe[m_currentKeyFrame].grav *= m_title.getLocalBounds().height;

	float temp = m_keyframe[m_currentKeyFrame].m_targetPositionTitle.x - m_keyframe[m_currentKeyFrame].m_startPositionTitle.x;
	float temp2 = m_keyframe[m_currentKeyFrame].m_duration.asSeconds() * cos(m_keyframe[m_currentKeyFrame].angle * acos(-1.0f) / 180.0);
	m_keyframe[m_currentKeyFrame].initialVelocity = temp / temp2;

	m_keyframe[m_currentKeyFrame].vel = sf::Vector2f(0.0f, m_keyframe[m_currentKeyFrame].initialVelocity * sin(m_keyframe[m_currentKeyFrame].angle * acos(-1.0f) / 180.0));
}

void AnimateLicense::setKeyframe(int key, sf::Time dur, sf::Vector2f tPos, sf::Vector2f sPos, AnimType type)
{
	m_keyframe[key].m_duration = dur;
	m_keyframe[key].m_timePassed = sf::Time::Zero;
	m_keyframe[key].m_targetPositionTitle = tPos;
	m_keyframe[key].m_targetPositionSub = sPos;
	m_keyframe[key].m_type = type;
}

bool AnimateLicense::isPaused()
{
	return m_paused;
}

void AnimateLicense::move(sf::Time t)
{
	sf::Vector2f moveTitle(m_keyframe[m_currentKeyFrame].m_targetPositionTitle - m_keyframe[m_currentKeyFrame].m_startPositionTitle);
	sf::Vector2f moveSub(m_keyframe[m_currentKeyFrame].m_targetPositionSub - m_keyframe[m_currentKeyFrame].m_startPositionSub);

	float scaler = m_keyframe[m_currentKeyFrame].m_duration.asSeconds() / t.asSeconds();
	moveTitle /= scaler;
	moveSub /= scaler;

	m_titlePosition += moveTitle;
	m_subPosition += moveSub;
}

void AnimateLicense::somersault(sf::Time t)
{
	sf::Vector2f moveTitle(m_keyframe[m_currentKeyFrame].m_targetPositionTitle - m_keyframe[m_currentKeyFrame].m_startPositionTitle);
	sf::Vector2f moveSub(m_keyframe[m_currentKeyFrame].m_targetPositionSub - m_keyframe[m_currentKeyFrame].m_startPositionSub);

	float scaler = m_keyframe[m_currentKeyFrame].m_duration.asSeconds() / t.asSeconds();
	moveTitle /= scaler;
	moveSub /= scaler;

	m_titlePosition += moveTitle;
	m_subPosition += moveSub;

	m_title.rotate(360.0f / scaler);

	m_keyframe[m_currentKeyFrame].vel = m_keyframe[m_currentKeyFrame].vel + m_keyframe[m_currentKeyFrame].grav * t.asSeconds();
	m_titlePosition += m_keyframe[m_currentKeyFrame].vel*t.asSeconds() + (m_keyframe[m_currentKeyFrame].grav*(t.asSeconds() * t.asSeconds()) * 0.5f);
}
