#include "Enemy.h"

Enemy::Enemy(int id,  sf::Vector2i position, Map* map, const std::string& type, sf::RenderWindow* window, sf::RenderTexture* texture) :
Entity(window, texture), type_(type), TILESIZE(gconsts::Gameplay::TILESIZE), target_(nullptr), isInCombat_(false), id_(id)
{
	sprite_.setPosition(static_cast<float> (position.x * TILESIZE), static_cast<float>(position.y * TILESIZE));
	map->setTileEnemyBlocked(position);
	if (type == gconsts::Gameplay::BAT_TYPE)
		health_ = 10;
}

Enemy::~Enemy()
{
	delete[] rects.rects;
}

bool Enemy::init()
{
	if (!loadTextureRects())
		return(false);
	if (!loadSpriteTexture())
		return(false);
	if (!font_.loadFromFile(gconsts::Assets::FONT_LOCATION))
		return(false);

	sprite_.setTexture(texture_);
	sprite_.setScale(2.f, 2.f);
	setTextureRect();

	text_.setFont(font_);
	text_.setCharacterSize(12);
	text_.setString(std::to_string(health_));
	setTextPos();

	return(true);
}

bool Enemy::loadSpriteTexture()
{
	if (type_ == gconsts::Gameplay::BAT_TYPE)
	{
		if (!texture_.loadFromFile("res//entities//bat.png"))
			return(false);
	}
	return(true);
}

void Enemy::render() const
{
	renderTexture_->draw(text_);
	renderTexture_->draw(sprite_);
}

void Enemy::update(const sf::Time& delta)
{
	setTextureRect();
}

bool Enemy::loadTextureRects()
{
	std::ifstream fromFile;

	fromFile.open("res//entities//" + type_ + "rects.txt", std::ios::in);

	if (fromFile.fail())
	{
		std::cout << "Failed to open " << type_ << "rects.txt" << std::endl;
		return(false);
	}
	else
	{


		fromFile >> subImageCount_;

		rects.rects = new sf::IntRect[subImageCount_];

		for (size_t i(0); i < static_cast<unsigned> (subImageCount_); ++i)
		{
			fromFile >> rects.rects[i].left;
			fromFile >> rects.rects[i].top;
			fromFile >> rects.rects[i].width;
			fromFile >> rects.rects[i].height;
		}
	}

	return(true);
}

void Enemy::setTextureRect()
{

	if (type_ == gconsts::Gameplay::BAT_TYPE)
	{
		bool rectSet(false);
		if (state_ == FLAP_UP  && !rectSet)
		{
			sprite_.setTextureRect(rects.rects[FLAP_DOWN]);
			state_ = FLAP_DOWN;
			rectSet = true;
		}
		if (state_ == FLAP_DOWN && !rectSet)
		{
			sprite_.setTextureRect(rects.rects[FLAP_UP]);
			state_ = FLAP_UP;
		}

	}
}

std::string Enemy::getEnemyType() const
{
	return(type_);
}

void Enemy::takeDamage(int damage)
{
	damage = abs(damage); //prevent negative damage
	if (health_ - damage > 0)
		health_ -= damage;
	else
		health_ = 0;

	text_.setString(std::to_string(health_));
	setTextPos();
}

bool Enemy::isAlive() const
{
	return(health_ > 0);
}

void Enemy::setCombatTarget(Entity* e)
{
	assert(e != nullptr);
	target_ = e;

}

void Enemy::removeTarget()
{
	target_ = nullptr;
}

bool Enemy::doesHaveTarget() const
{
	return(target_ != nullptr);
}

void Enemy::setTextPos()
{
	float textWidth = static_cast<float> (text_.getCharacterSize() * text_.getString().getSize());

	sf::Vector2f location(0.f, 0.f);

	location.x = (sprite_.getPosition().x + sprite_.getGlobalBounds().width / 2) - textWidth / 2.3f;
	location.y = sprite_.getPosition().y - text_.getCharacterSize();
	
	text_.setPosition(location);
}

int Enemy::getEnemyID() const
{
	return(id_);
}