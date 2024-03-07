#pragma comment(lib, "SDL2_mixer.lib")
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>
#include<iostream>
#include<stdlib.h>
#include<cmath>
#include<string>
#include<vector>
#include<ctime>
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

using namespace std;

SDL_Window* window;
SDL_Event event;
TTF_Font* font;
SDL_Renderer* renderer;
int scene = 0;
bool quit = false;
bool isClick = false;
bool isStart = false;
bool isCollide = false;
bool isLower = false;
bool isDown = false;
bool isCollidePalka = false, isCollidePalkaCur = false;
int index = -1, xPress, yPress, indexPalki = -1;

void init() //Инициализация
{
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
	window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN); //Создание окна
	renderer = SDL_CreateRenderer(window, -1, 0); //Создание отрисовщика
	font = TTF_OpenFont("123.ttf", 200); //Создание шрифта
}

SDL_Texture* LoadText(char* text, TTF_Font* font, int colorR, int colorG, int colorB)
{
	SDL_Surface* textSurface = NULL; //Временный интерфейс
	SDL_Color color = { colorR, colorG, colorB };
	textSurface = TTF_RenderText_Blended(font, text, color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FreeSurface(textSurface);
	return texture;
}

void DrawText(SDL_Texture* texture, int x, int y, int w, int h) //
{
	SDL_Rect dst = { x, y, w, h }; //Создаёшь объект
	SDL_RenderCopy(renderer, texture, NULL, &dst);
}

void choiceMenu() //Создание кнопок главного меню
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	char startChar[] = { "Начать игру" }, exitChar[] = { "Выход" };
	SDL_Texture* startTexture = LoadText(startChar, font, 0, 0, 0);
	SDL_Texture* exitTexture = LoadText(exitChar, font, 0, 0, 0);
	SDL_Rect startRect = { SCREEN_WIDTH / 2 - 150, (SCREEN_HEIGHT / 2) - (75 / 2) - 75, 300, 75 };
	SDL_Rect exitRect = { SCREEN_WIDTH / 2 - 150, (SCREEN_HEIGHT / 2) - (75 / 2) + 75, 300, 75 };
	SDL_RenderDrawRect(renderer, &startRect);
	SDL_RenderDrawRect(renderer, &exitRect);
	DrawText(startTexture, SCREEN_WIDTH / 2 - 150, (SCREEN_HEIGHT / 2) - (75 / 2) - 75, 300, 75);
	DrawText(exitTexture, SCREEN_WIDTH / 2 - 150, (SCREEN_HEIGHT / 2) - (75 / 2) + 75, 300, 75);
	if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && !isClick)
	{
		isClick = true;
		if (event.button.x >= SCREEN_WIDTH / 2 - 150 && event.button.x <= SCREEN_WIDTH / 2 - 150 + 300 && event.button.y >= (SCREEN_HEIGHT / 2) - (75 / 2) - 75 && event.button.y <= (SCREEN_HEIGHT / 2) - (75 / 2) - 75 + 75)
		{
			isClick = false;
			isDown = false;
			scene = 1;
		}
		if (event.button.x >= SCREEN_WIDTH / 2 - 150 && event.button.x <= SCREEN_WIDTH / 2 - 150 + 300 && event.button.y >= (SCREEN_HEIGHT / 2) - (75 / 2) + 75 && event.button.y <= (SCREEN_HEIGHT / 2) - (75 / 2) + 75 + 75)
		{

			isClick = false;
			isDown = false;
			scene = 2;
		}
	}
	if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT && isClick)
	{
		isClick = false;
	}
	SDL_DestroyTexture(startTexture);
	SDL_DestroyTexture(exitTexture);
}

class Palka
{
public:
	SDL_Rect lowBlock;
	SDL_Rect upBlock;
	Palka(double x, double y, double w, double h)
	{
		lowBlock.x = x;
		lowBlock.y = y;
		lowBlock.w = w;
		lowBlock.h = h;
		upBlock.x = x + 100;
		upBlock.y = SCREEN_HEIGHT / 2;
		upBlock.w = 20;
		upBlock.h = 344;
	}
	void Draw()
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(renderer, &lowBlock);
		SDL_RenderFillRect(renderer, &upBlock);
	}
};

class Block
{
public:
	int startx, starty;
	SDL_Rect block;
	SDL_Rect blockT;
	SDL_Color color;
	Block(double x, double y, double w, double h, SDL_Color c)
	{
		startx = x;
		starty = y;
		block.x = x;
		block.y = y;
		block.w = w;
		block.h = h;
		color = c;
		blockT.x = x;
		blockT.y = y;
		blockT.w = w;
		blockT.h = h;
	}
	void setPosition(double x, double y)
	{
		block.x = x;
		block.y = y;
		blockT.x = x;
		blockT.y = y;
	}
	void Draw()
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(renderer, &block);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderDrawRect(renderer, &blockT);
	}
};

vector<Palka> palki;
vector<Block> blocks;

bool RectCollision(SDL_Rect& one, SDL_Rect& two) // столкновение вида "AABB-AABB"
{
	// Перекрытие по оси x?
	bool collisionX = one.x + one.w >= two.x &&
		two.x + two.w >= one.x;

	// Перекрытие по оси y?
	bool collisionY = one.y + one.h >= two.y &&
		two.y + two.h >= one.y;

	// Если перекрытия происходят относительно обеих осей, то мы имеем столкновение
	return collisionX && collisionY;
}

bool MouseCollision(int x, int y, SDL_Rect rect)
{
	if (x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h) return true;
	return false;
}

void Game()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	if (!isStart) //Стартовая инициализация
	{
		isStart = true;
		Palka p1(91, SCREEN_HEIGHT - 40, 220, 20), p2(402, SCREEN_HEIGHT - 40, 220, 20), p3(713, SCREEN_HEIGHT - 40, 220, 20);
		palki.push_back(p1);
		palki.push_back(p2);
		palki.push_back(p3);
		for (int i = 0; i < 7; i++)
		{
			int r = rand() % 255;
			int g = rand() % 255;
			int b = rand() % 255;
			SDL_Color c;
			c.r = r;
			c.g = g;
			c.b = b;
			c.a = 255;
			Block tempBlock(p1.lowBlock.x + (10 * (i + 1)), p1.lowBlock.y - (20 * i) - p1.lowBlock.h, 200 - (20 * i), 20, c);
			blocks.push_back(tempBlock);
		}
		SDL_Color cT;
		cT.r = 255;
		cT.g = 0;
		cT.b = 0;
		cT.a = 255;
		/*blocks[2].color = cT;
		cT.r = 0;
		cT.g = 255;
		cT.b = 0;
		cT.a = 255;
		blocks[3].color = cT;*/
		//blocks.back().color = cT;
	}
	SDL_Rect lowBlock = { 0, SCREEN_HEIGHT - 20, SCREEN_WIDTH, 20 };

	if (isDown) //Падение блока
	{
		if (index != -1) //Если выбран какой-то блок
		{
			if (blocks[index].block.y > SCREEN_HEIGHT - 60) //Если упал на нижнюю границу
			{
				blocks[index].startx = blocks[index].block.x;
				blocks[index].starty = blocks[index].block.y;
				isDown = false;
			}
			for (int i = 0; i < blocks.size(); i++)
			{
				if (index == i) continue;
				if (RectCollision(blocks[index].block, blocks[i].block)) //Столкновения между обьектами
				{
					if (index < i)
					{
						blocks[index].setPosition(blocks[index].startx, blocks[index].starty);
					}
					else
					{
						blocks[index].startx = blocks[index].block.x;
						blocks[index].starty = blocks[index].block.y;
					}
					isDown = false;
					break;
				}
			}
			if (isDown)
			{
				for (int i = 0; i < palki.size(); i++)
				{
					if (RectCollision(blocks[index].block, palki[i].upBlock))
					{
						isCollidePalka = true;
						indexPalki = i;
					}
				}
				if (indexPalki != -1)
				{
					if (blocks[index].block.y + blocks[index].block.h >= palki[indexPalki].upBlock.y - 1 && blocks[index].block.y + blocks[index].block.h <= palki[indexPalki].upBlock.y + 1024)
					{
						blocks[index].block.x = palki[indexPalki].upBlock.x + palki[indexPalki].upBlock.w / 2 - blocks[index].block.w / 2;
						blocks[index].setPosition(blocks[index].block.x, blocks[index].block.y);
					}
				}
				blocks[index].block.y += 1;
				blocks[index].setPosition(blocks[index].block.x, blocks[index].block.y);
			}
			SDL_Delay(5);
		}
		if (!isDown) index = -1;
		if (index == -1) isDown = false;
	}
	else
	{
		if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && !isClick)
		{
			isClick = true;
			isDown = false;
			for (int i = 0; i < blocks.size(); i++)
			{
				if (MouseCollision(event.button.x, event.button.y, blocks[i].block))
				{
					if (!isDown)
					{
						index = i;
						xPress = event.button.x - blocks[index].block.x;
						yPress = event.button.y - blocks[index].block.y;
					}
				}
			}
		}
		if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT && isClick)
		{
			isClick = false;
			isDown = true;
		}
		if (index != -1 && isClick)
		{
			isCollide = false;
			isLower = false;
			isCollidePalka = false;
			isCollidePalkaCur = false;
			int xLast = blocks[index].block.x, yLast = blocks[index].block.y;
			for (int i = 0; i < palki.size(); i++)
			{
				if (RectCollision(blocks[index].block, palki[i].upBlock))
				{
					isCollidePalka = true;
					indexPalki = i;
				}
			}
			for (int i = 0; i < blocks.size(); i++)
			{
				if (index == i) continue;
				if (RectCollision(blocks[index].block, blocks[i].block))
				{
					if (abs((event.button.x - xPress) - xLast) <= 20 && abs((event.button.y - yPress) - yLast) <= 20)
					{
						if (isCollidePalka)
						{
							blocks[index].setPosition(xLast, event.button.y - yPress);
						}
						else
						{
							indexPalki = -1;
							blocks[index].setPosition(event.button.x - xPress, event.button.y - yPress);
						}
					}
					for (int j = 0; j < blocks.size(); j++)
					{
						if (index == j) continue;
						if (RectCollision(blocks[index].block, blocks[j].block))
						{
							blocks[index].setPosition(xLast, yLast);
							isCollide = true;
						}
					}
				}
			}
			if (blocks[index].block.y > SCREEN_HEIGHT - 60)
			{
				if (isCollidePalka)
				{
					blocks[index].setPosition(xLast, event.button.y - yPress);
				}
				else
				{
					indexPalki = -1;
					blocks[index].setPosition(event.button.x - xPress, event.button.y - yPress);
				}
				if (blocks[index].block.y > SCREEN_HEIGHT - 60)
				{
					blocks[index].setPosition(xLast, SCREEN_HEIGHT - 60 + blocks[index].block.h);
					isLower = true;
				}
			}
			if (!isCollide && !isLower)
			{
				if (isCollidePalka)
				{
					if (blocks[index].block.y + blocks[index].block.h >= palki[indexPalki].upBlock.y - 1 && blocks[index].block.y + blocks[index].block.h <= palki[indexPalki].upBlock.y + 1024)
					{
						blocks[index].block.x = palki[indexPalki].upBlock.x + palki[indexPalki].upBlock.w / 2 - blocks[index].block.w / 2;
						blocks[index].setPosition(blocks[index].block.x, event.button.y - yPress);
					}
					else
					{
						blocks[index].setPosition(xLast, event.button.y - yPress);
					}
				}
				else
				{
					indexPalki = -1;
					blocks[index].setPosition(event.button.x - xPress, event.button.y - yPress);
				}
			}
			else
			{
				blocks[index].setPosition(xLast, yLast);
			}
			for (int i = 0; i < palki.size(); i++)
			{
				if (RectCollision(blocks[index].block, palki[i].upBlock))
				{
					isCollidePalkaCur = true;
				}
			}
			if (isCollidePalkaCur && !isCollidePalka)
			{
				isDown = true;
			}
		}
	}

	SDL_RenderFillRect(renderer, &lowBlock);
	for (int i = 0; i < palki.size(); i++)
	{
		palki[i].Draw();
	}
	for (int i = 0; i < blocks.size(); i++)
	{
		blocks[i].Draw();
	}
}

int main(int args, char** argv)
{
	srand(time(NULL));
	init();
	while (!quit) //Игровой цикл
	{
		while (SDL_PollEvent(&event) || isDown) //Обработчик событий
		{
			if (event.type == SDL_QUIT) quit = true;
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //Цвет ручки
			SDL_RenderClear(renderer); //Заливка экрана
			switch (scene)
			{
			case 0:
			{
				choiceMenu();
				break;
			}
			case 1:
			{
				Game();
				break;
			}
			case 2:
			{
				quit = true;
				break;
			}
			}
			SDL_RenderPresent(renderer);
		}
	}

	return 1;
}