#pragma comment(lib, "SDL2_mixer.lib")
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>
#include<iostream>
#include <stdlib.h>
#include<cmath>
#include<string>
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

using namespace std;

SDL_Texture* LoadText(SDL_Renderer*& renderer, char* text, TTF_Font* font, int colorR, int colorG, int colorB)
{
	SDL_Surface* textSurface = NULL;
	SDL_Color color = { colorR, colorG, colorB };
	textSurface = TTF_RenderText_Blended(font, text, color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FreeSurface(textSurface);
	return texture;
}

void DrawText(SDL_Renderer*& renderer, SDL_Texture* texture, int x, int y, int w, int h)
{
	SDL_Rect dst = { x, y, w, h };
	SDL_RenderCopy(renderer, texture, NULL, &dst);
}

void loadmusic(Mix_Music* fon, const string& name)
{
	fon = Mix_LoadMUS(name.c_str());
	Mix_PlayMusic(fon, -1);
}

void sound(Mix_Chunk* Sound, const string& name)
{
	Sound = Mix_LoadWAV(name.c_str());
	Mix_PlayChannel(-1, Sound, 0);
}

int main(int args, char** argv)
{
	int scene = 0, x1 = 0, x2 = SCREEN_WIDTH - 30, y1 = SCREEN_HEIGHT / 2 - 100, y2 = SCREEN_HEIGHT / 2 - 100, MusicVolume = 0, MusicX = SCREEN_WIDTH / 2 - 165, MusicY = SCREEN_HEIGHT - (SCREEN_HEIGHT - 200) - 10.5, a = 0, b = 0;
	bool quit = false, move = false;
	char start[] = { "Начать игру" }, settings[] = { "Настройки" }, exit[] = { "Выход из игры" }, fonMusic[] = { "Музыка" };
	if (SDL_Init(SDL_INIT_EVERYTHING) != NULL)
	{
		cout << "SDL_Error:" << SDL_GetError() << endl;
	}
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		cout << "IMG_Error:" << IMG_GetError() << endl;
	}
	if (TTF_Init() != NULL)
	{
		cout << "TTF_Error:" << TTF_GetError() << endl;
	}
	if ((Mix_Init(MIX_INIT_MP3) & MIX_INIT_MP3) != MIX_INIT_MP3)
	{
		cout << "MIX_Error:" << Mix_GetError() << endl;
	}
	SDL_Window* window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		cout << "SDL_Error:" << SDL_GetError() << endl;
	}
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == NULL)
	{
		cout << "SDL_Error:" << SDL_GetError() << endl;
	}
	SDL_Event event;
	TTF_Font* font = TTF_OpenFont("Text.ttf", 100);
	int score1 = 0, score2 = 0;
	char ScoreText1[100], ScoreText2[100];
	SDL_Texture* ScoreTexture1;
	_itoa_s(score1, ScoreText1, 10);
	ScoreTexture1 = LoadText(renderer, ScoreText1, font, 255, 255, 255);
	SDL_Texture* ScoreTexture2;
	_itoa_s(score2, ScoreText2, 10);
	ScoreTexture2 = LoadText(renderer, ScoreText2, font, 255, 255, 255);
	SDL_Surface* SurfaceKryg = IMG_Load("kryg.png");
	SDL_Texture* KrygTexture = SDL_CreateTextureFromSurface(renderer, SurfaceKryg);
	SDL_FreeSurface(SurfaceKryg);
	DrawText(renderer, KrygTexture, SCREEN_WIDTH / 2 - 75 / 2, SCREEN_HEIGHT / 2 - 75 / 2, 75, 75);
	SDL_Surface* SurfaceRaketki = IMG_Load("raketki.jpg");
	SDL_Texture* RaketkiTexture1 = SDL_CreateTextureFromSurface(renderer, SurfaceRaketki);
	SDL_Texture* RaketkiTexture2 = SDL_CreateTextureFromSurface(renderer, SurfaceRaketki);
	SDL_FreeSurface(SurfaceRaketki);
	SDL_Texture* start1 = LoadText(renderer, start, font, 255, 255, 255);
	SDL_Texture* settings1 = LoadText(renderer, settings, font, 255, 255, 255);
	SDL_Texture* exit1 = LoadText(renderer, exit, font, 255, 255, 255);
	SDL_Texture* fonMusic1 = LoadText(renderer, fonMusic, font, 255, 255, 255);
	Mix_Chunk* Sound = NULL;
	Mix_Music* fon = NULL;
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
	SDL_Surface* SurfaceLine = IMG_Load("line.png");
	SDL_Texture* LineTexture = SDL_CreateTextureFromSurface(renderer, SurfaceLine);
	SDL_FreeSurface(SurfaceLine);
	SDL_Surface* SurfaceKryg1 = IMG_Load("kryg1.png");
	SDL_Texture* Kryg1Texture = SDL_CreateTextureFromSurface(renderer, SurfaceKryg1);
	SDL_FreeSurface(SurfaceKryg1);
	while (!quit)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) quit = true;
			if (scene == 0)
			{
				if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
				{
					//loadmusic(fon, "fon.mp3");
					int x1, y1;
					SDL_GetMouseState(&x1, &y1);
					if (x1 >= SCREEN_WIDTH / 2 - 150 && x1 <= SCREEN_WIDTH / 2 - 150 + 300 && y1 >= SCREEN_HEIGHT - (SCREEN_HEIGHT - 150) && y1 <= (SCREEN_HEIGHT - (SCREEN_HEIGHT - 150)) + 75)
					{
						scene = 1;
						cout << scene;
					}
					if (x1 >= SCREEN_WIDTH / 2 - 150 && x1 <= SCREEN_WIDTH / 2 - 150 + 300 && y1 >= (SCREEN_HEIGHT / 2) - (75 / 2) && y1 <= ((SCREEN_HEIGHT / 2) - (75 / 2)) + 75)
					{
						scene = 2;
						cout << scene;
					}
					if (x1 >= SCREEN_WIDTH / 2 - 150 && x1 <= SCREEN_WIDTH / 2 - 150 + 300 && y1 >= SCREEN_HEIGHT - 225 && y1 <= SCREEN_HEIGHT - 225 + 75)
					{
						quit = true;
					}
				}
			}
			if (scene == 2)
			{
				if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
				{
					if (event.button.x >= SCREEN_WIDTH / 2 - 165  && event.button.x <= SCREEN_WIDTH / 2 - 165 + 330 && event.button.y >= SCREEN_HEIGHT - (SCREEN_HEIGHT - 200) && event.button.y <= SCREEN_HEIGHT - (SCREEN_HEIGHT - 200) + 6)
					{
						MusicX = event.button.x - 15;
					}
				}
			}
		}
		if (scene == 1)
		{
			const Uint8* state = SDL_GetKeyboardState(NULL);
			if (state[SDL_SCANCODE_W])
			{
				y1 -= 10;
				if (y1 < 0) y1 = 0;
				DrawText(renderer, RaketkiTexture1, 0, y1, 30, 200);
				cout << y1;
			}
			if (state[SDL_SCANCODE_S])
			{
				y1 += 10;
				if (y1 > SCREEN_HEIGHT - 200) y1 = SCREEN_HEIGHT - 200;
				DrawText(renderer, RaketkiTexture1, 0, y1, 30, 200);
				cout << y1;
			}
			if (state[SDL_SCANCODE_UP])
			{
				y2 -= 10;
				if (y2 < 0) y2 = 0;
				DrawText(renderer, RaketkiTexture1, 0, y2, 30, 200);
				cout << y2;
			}
			if (state[SDL_SCANCODE_DOWN])
			{
				y2 += 10;
				if (y2 > SCREEN_HEIGHT - 200) y2 = SCREEN_HEIGHT - 200;
				DrawText(renderer, RaketkiTexture1, 0, y2, 30, 200);
				cout << y2;
			}
			SDL_Delay(30);
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		if (scene == 0)
		{
			SDL_SetRenderDrawColor(renderer, 0, 18, 55, 255);
			SDL_RenderClear(renderer);
			SDL_SetRenderDrawColor(renderer, 178, 34, 34, 255);
			SDL_Rect rect1 = { SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT - (SCREEN_HEIGHT - 150), 300, 75 };
			SDL_RenderFillRect(renderer, &rect1);
			SDL_Rect rect2 = { SCREEN_WIDTH / 2 - 150, (SCREEN_HEIGHT / 2) - (75 / 2), 300, 75 };
			SDL_RenderFillRect(renderer, &rect2);
			SDL_Rect rect3 = { SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT - 225, 300, 75 };
			SDL_RenderFillRect(renderer, &rect3);
			DrawText(renderer, start1, SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT - (SCREEN_HEIGHT - 150), 300, 75);
			DrawText(renderer, settings1, SCREEN_WIDTH / 2 - 150, (SCREEN_HEIGHT / 2) - (75 / 2), 300, 75);
			DrawText(renderer, exit1, SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT - 225, 300, 75);
		}
		if (scene == 1)
		{
			SDL_SetRenderDrawColor(renderer, 0, 18, 55, 255);
			SDL_Rect rect1 = { 0, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT };
			SDL_RenderFillRect(renderer, &rect1);
			SDL_SetRenderDrawColor(renderer, 178, 34, 34, 255);
			SDL_Rect rect2 = { SCREEN_WIDTH / 2, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
			SDL_RenderFillRect(renderer, &rect2);
			DrawText(renderer, ScoreTexture1, 75, 50, 75, 75);
			DrawText(renderer, ScoreTexture1, SCREEN_WIDTH - 150, 50, 75, 75);
			DrawText(renderer, KrygTexture, SCREEN_WIDTH / 2 - 75 / 2, SCREEN_HEIGHT / 2 - 75 / 2, 75, 75);
			DrawText(renderer, RaketkiTexture1, x1, y1, 30, 200);
			DrawText(renderer, RaketkiTexture2, x2 , y2, 30, 200);
		}
		if (scene == 2)
		{
			SDL_SetRenderDrawColor(renderer, 0, 18, 55, 255);
			SDL_RenderClear(renderer);
			//Mix_VolumeMusic(100);
			DrawText(renderer, fonMusic1, SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT - (SCREEN_HEIGHT - 75), 100, 75);
			DrawText(renderer, LineTexture, SCREEN_WIDTH / 2 - 165, SCREEN_HEIGHT - (SCREEN_HEIGHT - 200), 330, 6);
			DrawText(renderer, Kryg1Texture, MusicX, MusicY, 30, 30);
		}
		SDL_RenderPresent(renderer);
	}
	SDL_DestroyTexture(Kryg1Texture);
	SDL_DestroyTexture(fonMusic1);
	SDL_DestroyTexture(LineTexture);
	SDL_DestroyTexture(start1);
	SDL_DestroyTexture(settings1);
	SDL_DestroyTexture(exit1);
	SDL_DestroyTexture(ScoreTexture2);
	SDL_DestroyTexture(KrygTexture);
	SDL_DestroyTexture(RaketkiTexture1);
	SDL_DestroyTexture(RaketkiTexture2);
	SDL_DestroyTexture(ScoreTexture1);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
	return 1;
}