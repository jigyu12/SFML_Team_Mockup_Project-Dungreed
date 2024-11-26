#pragma once

class Monster : public SpriteGo
{
protected:


public:
	Monster(const std::string& texId = "", const std::string& name = "");
	~Monster() = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};