#include <SDL2/SDL.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <functional>
#include <vector>
#include "config.h"
#include "Init.h"

bool running = true;
int test = 255; // 테스트용

std::vector<Button*> button_list;

int main(int argc, char *argv[])
{
	// 초기화
	if (!Init()) return 1;
	if (!Load()) return 1;
	
	Renderer renderer;
	
	SDL_Rect test_pos = {100, 1500, 300, 300};
	color test_col = {200, 200, 200};
	Button* tb = new Button(test_pos, test_col, "test");
	tb->SetImage(images[IMG_BUTTON_LEFT]);
	
	SDL_Rect test_pos_2 = {700, 1500, 300, 300};
	Button* tb_2 = new Button(test_pos_2, test_col, "test");
	tb_2->SetImage(images[IMG_BUTTON_RIGHT]);
	
	button_list.push_back(tb);
	button_list.push_back(tb_2);
	
	while (running) {
		Input();
		Update();
		Render(renderer);
		
		SDL_Delay(60);
	}
	Kill();
	return 0;	
}

// ================= Input =================

void Input(){
	while(SDL_PollEvent(&ev) != 0){
		switch(ev.type){
			case SDL_QUIT:
				running = false;
				break;
			case SDL_MOUSEMOTION:
				mouse_x = ev.button.x;
				mouse_y = ev.button.y;
				break;
			case SDL_FINGERDOWN:
				isTouching = true;
				break;
			case SDL_FINGERUP:
				isTouching = false;
				break;
		}
	}
}


// ================= Update =================

void Update(){	
	for(int i = 0; i < button_list.size(); i++){
		button_list[i]->Update();
	}
	
}

void Button::Update() {
	CheckButtonPressed({mouse_x, mouse_y});
}

/*
void Player::Update() {
	
}*/


// ================= Render =================



void Render(Renderer& renderer){
	for(int i = 0; i < button_list.size(); i++){
		button_list[i]->Draw(renderer);
	}
	
	SDL_Rect background_transform = {100, 100, SCREEN_WIDTH - 200, SCREEN_HEIGHT - 1000};
	
	// 받아온 서피스 위에 그림을 그림
	renderer.DrawRectangle(&background_transform, {200, 200, 200});
	/*
	SDL_Rect dest = {600, 300, 300, 300};
	
	SDL_BlitScaled(images[IMG_TEST], NULL, winSurface, &dest);*/
	
	
	// 그림을 그린 서피스를 업데이트함
	SDL_UpdateWindowSurface(window);
}



// ================= Init =================



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



// ================= Renderer =================



void Renderer::DrawImage(SDL_Surface* image, SDL_Rect* position) {
	SDL_BlitScaled(image, NULL, winSurface, position);
}


void Renderer::DrawRectangle(SDL_Rect* position, color col) {
	SDL_FillRect(winSurface, position, SDL_MapRGB(winSurface->format, col.r, col.g, col.b));
}



// ================= Load =================



bool Load(){
	loaded_image = 0;
	for(int i = 0; i < IMAGE_NUMBER; i++){
		SDL_Surface* temp = SDL_LoadBMP(image_list[i]);
		if (!temp){
			std::cout << "ERROR LOADING IMAGE! : " << SDL_GetError() << std::endl;
			return false;
		}
	
		
		images[i] = SDL_ConvertSurface(temp, winSurface->format, 0);
		if (!images[i]){
		std::cout << "ERROR CONVERTING IMAGE! : " << SDL_GetError() << std::endl;
		return false;
		}
	
		SDL_FreeSurface(temp);
		
		loaded_image++;
	}

	return true;
}

/*
constexpr int toIndex(Image_index img) {
    return static_cast<int>(img);
}


SDL_Surface* TextureManager::Load(Image_index img){
	SDL_Surface* loaded_image = image[toIndex(img)];
	
	SDL_Surface* temp = SDL_LoadBMP(path);
	if (!temp){
		std::cout << "ERROR LOADING IMAGE! : " << SDL_GetError() << std::endl;
		return nullptr;
	}
	
	SDL_Surface* image = SDL_ConvertSurface(temp, winSurface->format, 0);
	if (!image){
		std::cout << "ERROR CONVERTING IMAGE! : " << SDL_GetError() << std::endl;
		return nullptr;
	}
	
	images[loaded_image] = image;
	SDL_FreeSurface(temp);
	
	return image; 
}*/


// ================= Kill =================



void Kill(){	
	for(int i = 0; i<button_list.size(); i++){
		delete button_list[i];
	}
	
	// 다 했으면 윈도우 파☆괴
	SDL_DestroyWindow(window);
	
	// 이제 SDL 다 사용했으니 종료
	SDL_Quit();
}



// ================= Button =================



void Button::Draw(Renderer& renderer) {
	bool isPressed = GetPressedState();
	color buttonColor = GetColor();
	SDL_Rect transform = GetTransform(); 
	
	if(isPressed && isTouching){
		int clamp_amount = 15;
		color pressed_col = { std::clamp(buttonColor.r - clamp_amount, 15, 255), std::clamp(buttonColor.g  - clamp_amount, 15, 255), std::clamp(buttonColor.b  - clamp_amount, 15, 255)};
		SDL_SetSurfaceColorMod(sprite, pressed_col.r, pressed_col.g, pressed_col.b);
		renderer.DrawImage(sprite, &transform);
	}
	else {
				SDL_SetSurfaceColorMod(sprite, 255, 255, 255);
			renderer.DrawImage(sprite, &transform);
	}
}

bool Button::CheckButtonPressed(pos pressed_pos) {
	if (isPointFree(pressed_pos, GetTransform()))
		isPressed = true;
	else
		isPressed = false;
	return isPressed;
}



// ================= Other =================



bool isPointFree(pos point, SDL_Rect target){
	if (point.x < target.x + target.w && point.x > target.x && point.y < target.y + target.w && point.y > target.y) return true;
	return false;
	std::cout << "FUCKED!" << std::endl;
}