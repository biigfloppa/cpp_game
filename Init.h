// 여기에선 게임에서 쓰이는 함수, 구조체를 선언
#include <iostream>
#include <string>
#include <SDL2/SDL.h>

#define IMAGE_NUMBER 1

// 이미지 종류
enum {
	IMG_TEST
};

struct pos{
	int x;
	int y;
}

// 함수
bool Init();
bool Load();
bool isColliding(SDL_Rect rect_a, SDL_Rect rect_b);
bool isPointFree(pos point, SDL_Rect target);
void Render();
void Kill();

class Object {
private:
	SDL_Rect transform;
}

// 버튼
class Button : public Object {
private:
	std::string text;
public:
	Button(SDL_Rect r) : transform(r){}
	SDL_Rect GetTransform() { return transform; }
	bool isPressed();
};

// SDL 전역
SDL_Window* window = NULL;
SDL_Surface* winSurface = NULL;
SDL_Event ev;



SDL_Surface* images[IMAGE_NUMBER];