#pragma once
class TextGo;

class Button : public GameObject
{
protected:

	sf::RectangleShape buttonBackground;
	sf::Vector2i textureRect;

	TextGo* buttonText;

	std::function<void()> Clicked;

	bool pressed;
	bool mouseon;

public:
	Button(const std::string& name = "");
	~Button() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void SetPressed(bool pressed);
	bool isOnMouse() { return mouseon; }

	void Set(const sf::Vector2f& buttonSize, int charSize, const sf::Color& charColor = sf::Color::Black);
	void SetString(const std::string& text, bool useTable = false);
	void SetString(const std::string& textid, const std::wstring& text);
	void SetCharSize(float size);
	void SetButtonSize(const sf::Vector2f& size);

	void SetClickedEvent(const std::function<void()>& event);
};
