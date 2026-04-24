// 여기에선 게임에서 쓰이는 함수, 구조체를 선언
#include "config.h"
#include <string>
#include <functional>
#include <SDL2/SDL.h>

enum TYPE {
	DEFAULT,
	BUTTON
};

struct pos{
	int x;
	int y;
};

struct color{
	int r;
	int g;
	int b;
};

// SDL 전역
SDL_Window* window = NULL;
SDL_Surface* winSurface = NULL;
SDL_Surface* images[IMAGE_NUMBER];
SDL_Event ev;

int loaded_image;

int mouse_x;
int mouse_y;
bool isTouching;

class Renderer {
public:
	void DrawImage(SDL_Surface* image, SDL_Rect* transform);
	void DrawRectangle(SDL_Rect* position, color col);
};

// 함수
bool Init();
bool Load();
void Input();
void Update();
void Render(Renderer& renderer);
void Kill();

void PlaySound();
bool isColliding(SDL_Rect rect_a, SDL_Rect rect_b);
bool isPointFree(pos point, SDL_Rect target);

class Object {
protected:
	SDL_Rect transform;
	SDL_Surface* sprite;
public:
	Object(SDL_Rect _transform) : transform(_transform) {}
	virtual ~Object() {}
	const SDL_Rect GetTransform() { return transform; }
	void SetTransform(SDL_Rect _transform) { transform = _transform; }
	SDL_Surface* GetImage() { return sprite; }
	void SetImage(SDL_Surface* _image) { sprite = _image; }
	virtual void Draw(Renderer& renderer) = 0;
	virtual void Update() = 0;
};

// 버튼
class Button : public Object{
private:
	std::string text;
	bool isPressed;
	color buttonColor;
public:
	Button(SDL_Rect rect, color col, std::string t) : Object(rect), buttonColor(col){
		text = t;
	}
	std::string GetText() { return text; }
	bool GetPressedState() { return isPressed; }
	color GetColor() { return buttonColor; }
	void Update();
	void Draw(Renderer& renderer);
	bool CheckButtonPressed(pos pressed_pos);
	~Button() {
	}
	// 콜백용
	// 외부에서 람다함수로 셋팅할것
	std::function<void()> onPressedDown;
	std::function<void()> onPressing;
	std::function<void()> onPressedUp;
};

class TextureManager {
public:
	SDL_Surface* Load(const char* path);
	
	~TextureManager() {
		for(int i = 0; i<IMAGE_NUMBER; i++){
			SDL_FreeSurface(images[i]);
		}
		// 서피스 해제
	}
};
/*

class Poop : public Object{
private:
	SDL_Rect transform;
public:
	Poop(SDL_Rect rect) : Object(rect) {}
	void Update();
	void Draw();
};

class Player : public Object{
private:
	bool isAlive;
public:
	Player(SDL_Rect rect) : Object(rect) {
		isAlive = true;
	}
	void Update();
	void Draw();
};
*/

class ScoreManager{
	
};