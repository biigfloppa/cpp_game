// 이 파일에는 게임 설정들을 저장함다
#pragma once

#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 2340

#define IMAGE_NUMBER 3

const char* image_list[] = {
	"Button_left.bmp",
	"Button_right.bmp",
	"sample.bmp"
};

// 이미지 종류
enum {
	IMG_BUTTON_LEFT,
	IMG_BUTTON_RIGHT,
	IMG_TEST
};