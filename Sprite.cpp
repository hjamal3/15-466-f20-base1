#include "Sprite.hpp"
#include "read_write_chunk.hpp"
#include "load_save_png.hpp"

#include <fstream>
#include <iostream>
#include <vector>

//SpriteAtlas::SpriteAtlas(std::string const& fname)
//{
//}

DummyAtlas::DummyAtlas(std::string const& fname)
{
	//std::cout << fname << std::endl;

	// load png
	//std::string str = "sprites/test1.png";
	//load_png(str, &tex_size, &tex_data, UpperLeftOrigin);

	// load robot tiles
	load_png(robot_up_str, &tex_size, &robot_up, UpperLeftOrigin);
	load_png(robot_down_str, &tex_size, &robot_down, UpperLeftOrigin);
	load_png(robot_left_str, &tex_size, &robot_left, UpperLeftOrigin);
	load_png(robot_right_str, &tex_size, &robot_right, UpperLeftOrigin);

	// load goodie tile
	load_png(goodie_str, &tex_size, &goodie, UpperLeftOrigin);

	// load brick tile
	load_png(brick_str, &tex_size, &brick, UpperLeftOrigin);

	// load blocks
	load_png(block1_str, &tex_size, &block1, UpperLeftOrigin);
	load_png(block2_str, &tex_size, &block2, UpperLeftOrigin);
	load_png(block3_str, &tex_size, &block3, UpperLeftOrigin);

	// load beams
	load_png(beam_up_str, &tex_size, &beam_up, UpperLeftOrigin);
	load_png(beam_down_str, &tex_size, &beam_down, UpperLeftOrigin);
	load_png(beam_left_str, &tex_size, &beam_left, UpperLeftOrigin);
	load_png(beam_right_str, &tex_size, &beam_right, UpperLeftOrigin);

	// load enemy 
	load_png(enemy_str, &tex_size, &enemy, UpperLeftOrigin);

	// load palettes
	std::ifstream in_palettes("palettes.asset", std::ios::binary);
	if (in_palettes)
	{
		// change to something variable.
		read_chunk(in_palettes, "plt0", &palettes);
	}
	else
	{
		std::cout << "Failed to construct palettes" << std::endl;
	}
	in_palettes.close();

	// load test data. unused.
	std::ifstream in(fname, std::ios::binary);
	if (in)
	{
		// change to something variable.
		read_chunk(in, "spr0", &dummy_in);
	}
	else
	{
		std::cout << "Failed to construct DummyAtlas" << std::endl;
	}
	in.close();

}

DummyAtlas::~DummyAtlas()
{

}
