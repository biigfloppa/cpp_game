#include "config.h"
#include "Init.h"
#include <SDL2/SDL.h>

bool running = true;
int test = 255; // 테스트용

int main(int argc, char *argv[])
{
	// 초기화
	if (!Init()) return 1;
	if (!Load()) return 1;
	
	
	while (running) {
	
	Render();
	
	while(SDL_PollEvent(&ev) != 0){
		switch(ev.type){
			case SDL_QUIT:
				running = false;
				break;
			/*
			case SDL_MOUSEBUTTONDOWN:
				switch(ev.button.button)
				{
					case SDL_BUTTON_LEFT:
						test = 0;
						break;
				}*/
			case SDL_FINGERDOWN:
				test = 0;
				break;
			case SDL_FINGERUP:
				test = 255;
				break;
		}
		}
	}
	std::cout << test << std::endl;
	Kill();
	return 0;
}

void Render(){
	SDL_Rect rect[3] = {
		{200, 800, 100, 100},
		{300, 300, 100, 500},
		{400, 800, 100, 100}
		};
	
	// 받아온 서피스 위에 그림을 그림
	for(int i = 0; i < 3; i++){
	SDL_FillRect(winSurface, &rect[i], SDL_MapRGB(winSurface->format, 255, 255, test));
}
	
	SDL_Rect dest = {600, 300, 300, 300};
	
	SDL_BlitScaled(images[IMG_TEST], NULL, winSurface, &dest);
	
	// 그림을 그린 서피스를 업데이트함
	SDL_UpdateWindowSurface(window);
}

bool Init(){
	// 가장 먼저 SDL 함수들을 초기화
	if (SDL_Init( SDL_INIT_EVERYTHING ) < 0) {
		std::cout << "SDL INIT ERROR! : " << SDL_GetError() << std::endl;
		return false;
	}
	
	// 그 다음 윈도우를 만들어서 저장
	window = SDL_CreateWindow("Some Name", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!window){
		std::cout << "WINDOW INIT ERROR! : " << SDL_GetError() << std::endl;
		return false;
	}
	
	// 윈도우에서 그릴 수 있는 공간인 "서피스"를 받아옴
	winSurface = SDL_GetWindowSurface(window);
	if (!winSurface){
		std::cout << "WINDOW SURFACE INIT ERROR! : " << SDL_GetError() << std::endl;
		return false;
	}
	return true;
}

bool Load(){
	SDL_Surface* temp = SDL_LoadBMP("sample.bmp");
	
	if (!temp){
		std::cout << "ERROR LOADING IMAGE! : " << SDL_GetError() << std::endl;
		return false;
	}
	
	images[IMG_TEST] = SDL_ConvertSurface(temp, winSurface->format, 0);
	if (!images[IMG_TEST]){
		std::cout << "ERROR CONVERTING IMAGE! : " << SDL_GetError() << std::endl;
		return false;
	}
	
	SDL_FreeSurface(temp);
	
	return true;
}

void Kill(){
	for(int i = 0; i<IMAGE_NUMBER; i++){
		SDL_FreeSurface(images[i]);
	}
	
	// 다 했으면 윈도우 파☆괴
	SDL_DestroyWindow(window);
	
	// 이제 SDL 다 사용했으니 종료
	SDL_Quit();
}

bool isPointFree(pos point, SDL_Rect target){
	if (point.x < target.x + target.w && point.x > target.x && point.y < target.y + target.w && point.y > target.y) return true;
	return false;
}

bool Button::isPressed(){
	while(SDL_PollEvent(&ev) != 0){
		switch(ev.type){
			case SDL_FINGERDOWN:
			{
				int x = ev.tfinger.x * SCREEN_WIDTH;
				int y = ev.tfinger.y * SCREEN_HEIGHT
				if isPointFree({x, y}, Button.GetTransform())
					return true;				
				}
				else return false;
				}
			} break;
		}
}