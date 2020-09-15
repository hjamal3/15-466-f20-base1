#include "PlayMode.hpp"
#include "Sprite.hpp"
#include "Load.hpp"
#include "data_path.hpp"
#include "PPU466.hpp"

//for the GL_ERRORS() macro:
#include "gl_errors.hpp"

//for glm::value_ptr() :
#include <glm/gtc/type_ptr.hpp>

#include <random>
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

DummyData const* robot = nullptr;
DummyData const* box1 = nullptr;
DummyData const* box2 = nullptr;
std::vector< glm::u8vec4 > const * robot_up = nullptr;
std::vector< glm::u8vec4 > const * robot_down = nullptr;
std::vector< glm::u8vec4 > const * robot_left = nullptr;
std::vector< glm::u8vec4 > const * robot_right = nullptr;
std::vector< glm::u8vec4 > const * enemy = nullptr;
std::vector< glm::u8vec4 > const* goodie = nullptr;
std::vector< glm::u8vec4 > const* brick = nullptr;
std::vector< glm::u8vec4> const* block1, * block2, * block3 = nullptr;
std::vector< glm::u8vec4> const* beam_up, * beam_down, * beam_left, * beam_right = nullptr;

std::vector<PPU466::Palette> const * palettes;

// based off: https://github.com/15-466/15-466-f19-base1/blob/99434c0b859571b6cc0edb9e8e3781f54bd29d00/StoryMode.cpp
// Global function to load all sprites when main is called
Load<DummyAtlas> dummysprites(LoadTagEarly, []() -> DummyAtlas const* {

	// called dummy improperly
	DummyAtlas const* ret = new DummyAtlas(data_path("test1.kk")); // put it in dist

	// in game tiles
	robot_up = &(ret->robot_up);
	robot_down = &(ret->robot_down);
	robot_left = &(ret->robot_left);
	robot_right = &(ret->robot_right);
	goodie = &(ret->goodie);

	// background tiles
	brick = &(ret->brick);
	block1 = &(ret->block1);
	block2 = &(ret->block2);
	block3 = &(ret->block3);

	// beam tiles
	beam_up = &(ret->beam_up);
	beam_down = &(ret->beam_down);
	beam_left = &(ret->beam_left);
	beam_right = &(ret->beam_right);

	// enemy
	enemy = &(ret->enemy);

	// palettes
	palettes = &(ret->palettes);


	// these aren't being used 
	robot = &ret->dummy_in[0];
	box1 = &ret->dummy_in[1];
	box2 = &ret->dummy_in[2];

	return ret;
	}
);



void PlayMode::insert_tile_table(const std::vector<glm::u8vec4> * sprite_ptr, const int idx)
{
	// 10 is color, 11 is black
	// https://stackoverflow.com/questions/29123959/convert-a-char-array-of-0-and-1-to-bytes-in-c
	std::array<uint8_t, 8> arr1;
	std::array<uint8_t, 8> arr2;
	for (int row = 0; row < 8; row++)
	{
		uint8_t byte1 = 0;
		uint8_t byte2 = 0;
		for (int i = row * 8; i < (row + 1) * (8); i++)
		{
			glm::u8vec4 rgba = (*sprite_ptr)[i];
			// R or G or B. R = 01. G = 10. B = 11
			if (!((int)rgba.x && (int)rgba.y && (int)rgba.z))
			{
				if ((int)rgba.x) // R
				{
					byte1 |= 1 << (i - row * 8);
				}
				else if ((int)rgba.y) // G
				{
					byte2 |= 1 << (i - row * 8);
				}
				else if ((int)rgba.z) // B
				{
					byte1 |= 1 << (i - row * 8);
					byte2 |= 1 << (i - row * 8);

				}
			} 
		}
		arr1[7 - row] = byte1;
		arr2[7 - row] = byte2;
	}
	ppu.tile_table[idx].bit0 = arr1;
	ppu.tile_table[idx].bit1 = arr2;
}

PlayMode::PlayMode() {

	/**************** My code *****************/

	// insert robot sprites
	insert_tile_table(robot_up, 32);
	insert_tile_table(robot_down, 33);
	insert_tile_table(robot_left, 34);
	insert_tile_table(robot_right, 35);
	ppu.sprites[0].index = 32; // start robot upward facing
	ppu.sprites[0].attributes = 7; 

	// create enemy sprite
	insert_tile_table(enemy, 36);
	ppu.sprites[11].index = 36;
	ppu.sprites[11].attributes = 1;
	ppu.sprites[11].x = (int)enemy1_at.x;
	ppu.sprites[11].y = (int)enemy1_at.y;
	// second enemy
	insert_tile_table(enemy, 37);
	ppu.sprites[12].index = 36;
	ppu.sprites[12].attributes = 1;
	ppu.sprites[12].x = (int)enemy2_at.x;
	ppu.sprites[12].y = (int)enemy2_at.y;
	// third enemy
	insert_tile_table(enemy, 37);
	ppu.sprites[13].index = 36;
	ppu.sprites[13].attributes = 1;
	ppu.sprites[13].x = (int)enemy3_at.x;
	ppu.sprites[13].y = (int)enemy3_at.y;

	// insert goal sprite
	insert_tile_table(goodie, 0);
	ppu.sprites[1].index = 0;
	// http://www.cplusplus.com/reference/cstdlib/rand/
	srand((int)time(NULL));	
	ppu.sprites[1].x = (int)rand()%150+50;
	ppu.sprites[1].y = (int)rand()%150+50;
	ppu.sprites[1].attributes = 1;

	// insert bricks into tile
	insert_tile_table(brick, 1);

	// insert blocks into tile
	insert_tile_table(block1, 2);
	insert_tile_table(block2, 4);
	insert_tile_table(block3, 3);

	// create a beam
	insert_tile_table(beam_up, 5);
	insert_tile_table(beam_down, 6);
	insert_tile_table(beam_left, 7);
	insert_tile_table(beam_right, 8);

	// create the beams
	for (uint32_t i = 2; i < 2+max_beams; i++)
	{
		ppu.sprites[i].x = 255;
		ppu.sprites[i].y = 255;
		ppu.sprites[i].attributes = 7;
	}

	// insert palettes
	for (uint32_t i = 0; i < palettes->size(); i++)
	{
		ppu.palette_table[i] = (*palettes)[i];
	}
	std::cout << "Make it to the base. Run my friend. Run." << std::endl;

	/**************** End *****************/

}

PlayMode::~PlayMode() {
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {
	// keyboard, based on https://www.libsdl.org/release/SDL-1.2.15/docs/html/guideinputkeyboard.html
	switch (evt.type)
	{
	case SDL_KEYDOWN:
		switch (evt.key.keysym.sym)
		{
		case (SDLK_SPACE):
			if (!space.pressed)
			{
				switch (orientation)
				{
					case (0):
						ppu.sprites[beam_idx].index = 5;
						ppu.sprites[beam_idx].x = (int)player_at.x;
						ppu.sprites[beam_idx].y = (int)player_at.y + 16;
					break;
					case (1):
						ppu.sprites[beam_idx].index = 6;
						ppu.sprites[beam_idx].x = (int)player_at.x;
						ppu.sprites[beam_idx].y = (int)player_at.y - 16;
					break;
					case (2):
						ppu.sprites[beam_idx].index = 7;
						ppu.sprites[beam_idx].x = (int)player_at.x - 16;
						ppu.sprites[beam_idx].y = (int)player_at.y;
					break;
					case (3):
						ppu.sprites[beam_idx].index = 8;
						ppu.sprites[beam_idx].x = (int)player_at.x + 16;
						ppu.sprites[beam_idx].y = (int)player_at.y;
					break;
				}
				beam_idx++;
				if (beam_idx == 2+ max_beams)
				{
					beam_idx = 2;
				}
				space.pressed = true;
			}
			break;
		case (SDLK_LEFT):
			robot_velocity.x = -1;
			left.pressed = true;
			break;
		case (SDLK_RIGHT):
			robot_velocity.x = 1;
			right.pressed = true;
			break;
		case (SDLK_UP):
			robot_velocity.y = 1;
			up.pressed = true;
			break;
		case (SDLK_DOWN):
			robot_velocity.y = -1;
			down.pressed = true;
			break;
		}
		break;
	case SDL_KEYUP:
		switch (evt.key.keysym.sym)
		{
		case (SDLK_SPACE):
			space.pressed = false;
			break;
		case (SDLK_LEFT):
			if (robot_velocity.x < 0)
			{
				robot_velocity.x = 0;
			}
			left.pressed = false;
			break;
		case (SDLK_RIGHT):
			if (robot_velocity.x > 0)
			{
				robot_velocity.x = 0;
			}
			right.pressed = false;
			break;
		case (SDLK_UP):
			if (robot_velocity.y > 0)
			{
				robot_velocity.y = 0;
			}
			up.pressed = false;
			break;
		case (SDLK_DOWN):
			if (robot_velocity.y < 0)
			{
				robot_velocity.y = 0;
			}
			down.pressed = false;
			break;
		}
		break;
	}
	// normalize diag
	if (robot_velocity.x != 0 && robot_velocity.y != 0)
	{
		robot_velocity.x /= abs(robot_velocity.x) * sqrt(2.0f);
		robot_velocity.y /= abs(robot_velocity.y) * sqrt(2.0f);
	}
	return false;
}

void PlayMode::update(float elapsed) {
	if (!game_over)
	{
		player_at += elapsed * robot_velocity * robot_speed;

		// robot leaving map
		if (player_at.y > court_width_height -  robot_radius*4) {
			player_at.y = court_width_height - robot_radius*4;
		}
		if (player_at.y < robot_radius*2) {
			player_at.y = robot_radius*2;
		}
		if (player_at.x > court_width_height) {
			player_at.x = court_width_height;
		}
		if (player_at.x < robot_radius*2) {
			player_at.x = robot_radius*2;
		}

		// update enemy positions
		enemy1_at += elapsed * enemy1_velocity * enemy_speed;
		enemy2_at += elapsed * enemy2_velocity * enemy_speed;
		enemy3_at += elapsed * enemy3_velocity * enemy_speed;
		
		// check if enemy close to player
		float dist_enemy1_player = sqrt((player_at.y - enemy1_at.y) * (player_at.y - enemy1_at.y) + (player_at.x - enemy1_at.x) * (player_at.x - enemy1_at.x));
		float dist_enemy2_player = sqrt((player_at.y - enemy2_at.y) * (player_at.y - enemy2_at.y) + (player_at.x - enemy2_at.x) * (player_at.x - enemy2_at.x));
		float dist_enemy3_player = sqrt((player_at.y - enemy3_at.y) * (player_at.y - enemy3_at.y) + (player_at.x - enemy3_at.x) * (player_at.x - enemy3_at.x));

		if (dist_enemy1_player > 2.0f && dist_enemy2_player > 2.0f && dist_enemy3_player > 2.0f) {
			// from Artificial Intelligence for Games by Ian Millington and John Funge
			enemy1_velocity = (player_at - enemy1_at) / dist_enemy1_player;
			enemy2_velocity = (player_at - enemy2_at) / dist_enemy2_player;
			enemy3_velocity = (player_at - enemy3_at) / dist_enemy3_player;
		}
		else {
			std::cout << "GAME OVER. YOU LOSE." << std::endl;
			game_over = true;
		}

		// check if player close to goodie
		float dist_goodie_player = sqrt((player_at.y - ppu.sprites[1].y) * (player_at.y - ppu.sprites[1].y) + (player_at.x - ppu.sprites[1].x) * (player_at.x - ppu.sprites[1].x));
		if (dist_goodie_player < 3.0f)
		{
			std::cout << "YOU MADE IT. YOU WIN." << std::endl;
			game_over = true;
		}

		// check if enemy hit beam
		for (int i = 2; i < 2+max_beams; i++)
		{
			float dist_beam_enemy1 = sqrt((enemy1_at.y - ppu.sprites[i].y) * (enemy1_at.y - ppu.sprites[i].y) + (enemy1_at.x - ppu.sprites[i].x) * (enemy1_at.x - ppu.sprites[i].x));
			float dist_beam_enemy2 = sqrt((enemy2_at.y - ppu.sprites[i].y) * (enemy2_at.y - ppu.sprites[i].y) + (enemy2_at.x - ppu.sprites[i].x) * (enemy2_at.x - ppu.sprites[i].x));
			float dist_beam_enemy3 = sqrt((enemy3_at.y - ppu.sprites[i].y) * (enemy3_at.y - ppu.sprites[i].y) + (enemy3_at.x - ppu.sprites[i].x) * (enemy3_at.x - ppu.sprites[i].x));
			if (dist_beam_enemy1 < 8)
			{
				std::cout << "Enemy 1 goes BOOM!" << std::endl;
				ppu.sprites[i].x = 240;
				ppu.sprites[i].y = 240;
				enemy1_at -= elapsed * enemy1_velocity * (float)enemy_speed * 100.0f;
			}	
			else if (dist_beam_enemy2 < 8)
			{
				std::cout << "Enemy 2 goes BOOM!" << std::endl;
				ppu.sprites[i].x = 240;
				ppu.sprites[i].y = 240;
				enemy2_at -= elapsed * enemy2_velocity * (float)enemy_speed * 100.0f;
			} 
			else if (dist_beam_enemy3 < 8)
			{
				std::cout << "Enemy 3 goes BOOM!" << std::endl;
				ppu.sprites[i].x = 240;
				ppu.sprites[i].y = 240;
				enemy3_at -= elapsed * enemy3_velocity * (float)enemy_speed * 100.0f;
			}
		}
	}
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	//--- set ppu state based on game state ---

	//background color will be some hsv-like fade:
	ppu.background_color = glm::u8vec4(
		0,0,0,
		0
	);

	// create background grid pattern.
	for (uint32_t y = 0; y < PPU466::BackgroundHeight/2; ++y) {
		for (uint32_t x = 0; x < PPU466::BackgroundWidth/2; ++x) {
			ppu.background[2 * x + PPU466::BackgroundWidth*(2*y)] = 2 << 8 | ((x+y) % 2 + 2);
			ppu.background[2 * x + 1 + PPU466::BackgroundWidth * (2*y)] = 2 << 8 | ((x+y) % 2 + 2);
			ppu.background[2 * x + PPU466::BackgroundWidth * (2*y+1)] = 2 << 8 | ((x+y) % 2 + 2);
			ppu.background[2 * x + 1 + PPU466::BackgroundWidth * (2*y+1)] = 2 << 8 | ((x+y) % 2 + 2);
		}
	}
	for (uint32_t y = 0; y < PPU466::BackgroundHeight; ++y)
	{
		ppu.background[PPU466::BackgroundWidth * y] =  1;
		ppu.background[31+PPU466::BackgroundWidth * y] = 1;
		ppu.background[y] = 1;
		ppu.background[y + PPU466::BackgroundWidth * 29] = 1;
	}

	//player sprite:
	ppu.sprites[0].x = int32_t(player_at.x);
	ppu.sprites[0].y = int32_t(player_at.y);

	// enemy sprite:
	ppu.sprites[11].x = int32_t(enemy1_at.x);
	ppu.sprites[11].y = int32_t(enemy1_at.y);
	ppu.sprites[12].x = int32_t(enemy2_at.x);
	ppu.sprites[12].y = int32_t(enemy2_at.y);
	ppu.sprites[13].x = int32_t(enemy3_at.x);
	ppu.sprites[13].y = int32_t(enemy3_at.y);

	// depending on which direction, different drawn for robot
	if (up.pressed)
	{
		ppu.sprites[0].index = 32;
		orientation = 0;
	}
	else if (down.pressed)
	{
		ppu.sprites[0].index = 33;
		orientation = 1;
	}
	else if (left.pressed)
	{
		ppu.sprites[0].index = 34;
		orientation = 2;
	}
	else if (right.pressed)
	{
		ppu.sprites[0].index = 35;
		orientation = 3;
	} 

	//--- actually draw ---
	ppu.draw(drawable_size);
}
