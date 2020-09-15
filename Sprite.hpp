#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <array>

// heavily based off: https://github.com/15-466/15-466-f19-base1/blob/master/Sprite.hpp
struct DummyData {
	glm::uvec2 size = glm::uvec2(0); //size of sprite, in pixels

	int a;
	int b;
	int c;
	//std::string name = ""; //name for in-game lookup
	//glm::vec2 anchor = glm::vec2(0.0f); //position of anchor in sprite -- pixel coordinates, upper-left origin
};

//struct Sprite
//{
//
//};
//
//struct SpriteAtlas
//{
//	SpriteAtlas(std::string const & fname);
//	//~SpriteAtlas();
//
//};
struct DummyAtlas
{
	std::vector<DummyData> dummy_in;
	std::vector< glm::u8vec4 > tex_data;
	glm::uvec2 tex_size = glm::uvec2(0); // in 

	// four pngs based on where robot is moving
	std::vector<glm::u8vec4> robot_up;
	const std::string robot_up_str = "sprites/robot_up.png";
	std::vector<glm::u8vec4> robot_down;
	const std::string robot_down_str = "sprites/robot_down.png";
	std::vector<glm::u8vec4> robot_left;
	const std::string robot_left_str = "sprites/robot_left.png";
	std::vector<glm::u8vec4> robot_right;
	const std::string robot_right_str = "sprites/robot_right.png";

	// goodie object
	std::vector<glm::u8vec4> goodie;
	const std::string goodie_str = "sprites/goodie.png";

	// brick object
	std::vector<glm::u8vec4> brick;
	const std::string brick_str = "sprites/background.png";

	// blocks for background
	std::vector<glm::u8vec4> block1, block2, block3;
	const std::string block1_str = "sprites/block1.png";
	const std::string block2_str = "sprites/block2.png";
	const std::string block3_str = "sprites/block3.png";

	std::vector<glm::u8vec4> beam_up, beam_down, beam_left, beam_right;
	const std::string beam_up_str = "sprites/beam_up.png";
	const std::string beam_down_str = "sprites/beam_down.png";
	const std::string beam_left_str = "sprites/beam_left.png";
	const std::string beam_right_str = "sprites/beam_right.png";

	std::vector<glm::u8vec4> enemy;
	const std::string enemy_str = "sprites/enemy.png";

	typedef std::array< glm::u8vec4, 4 > Palette;
	std::vector<Palette> palettes;

	DummyAtlas(std::string const& fname);
	~DummyAtlas();
};
