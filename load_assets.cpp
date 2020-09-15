#include "load_save_png.hpp"
#include "read_write_chunk.hpp"
#include "Sprite.hpp"

#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include <array>

// https://github.com/15-466/15-466-f19-base1/blob/master/pack-sprites.cpp
//struct DummyData {
//	glm::uvec2 size = glm::uvec2(0); //size of sprite, in pixels
//	//std::vector< glm::u8vec4 > data; //pixel data for sprite
//	int a;
//	int b;
//	int c;
//	//std::string name = ""; //name for in-game lookup
//	//glm::vec2 anchor = glm::vec2(0.0f); //position of anchor in sprite -- pixel coordinates, upper-left origin
//};

void write_palettes()
{
	typedef std::array< glm::u8vec4, 4 > Palette;

	Palette p0 = { glm::u8vec4(0x00, 0x00, 0x00, 0xff),  // background
		glm::u8vec4(0xff, 0xcc, 0x99, 0xff),
		glm::u8vec4(0xff, 0xb2, 0x66, 0xff),
		glm::u8vec4(0xff, 0x66, 0x66, 0xff),
	};
	Palette p1 = { glm::u8vec4(0xff, 0xff, 0xff, 0xff), // goodie
		glm::u8vec4(0xff, 0x00, 0x00, 0xff),
		glm::u8vec4(0x00, 0xff, 0x00, 0xff),
		glm::u8vec4(0x00, 0x00, 0xff, 0xff),
	};

	Palette p2345 = { glm::u8vec4(0x00, 0x00, 0x00, 0x00), // blocks
		glm::u8vec4(0x80, 0x80, 0x80, 0xff),
		glm::u8vec4(0x60, 0x60, 0x60, 0xff),
		glm::u8vec4(0x40, 0x40, 0x40, 0xff),
	};
	Palette p6 = { glm::u8vec4(0x00, 0x00, 0x00, 0x00), // enemies
		glm::u8vec4(0x88, 0x88, 0xff, 0xff),
		glm::u8vec4(0x00, 0x00, 0x00, 0xff),
		glm::u8vec4(0x00, 0x00, 0x00, 0x00),
	};
	Palette p7 = { glm::u8vec4(0x00, 0x00, 0x00, 0x00), //robot
		glm::u8vec4(0xff, 0x00, 0x00, 0xff),
		glm::u8vec4(0x00, 0xff, 0x00, 0xff),
		glm::u8vec4(0x00, 0x00, 0xff, 0xff),
	};

	std::vector<Palette> palettes;
	palettes.emplace_back(p0);
	palettes.emplace_back(p1);
	palettes.emplace_back(p2345);
	palettes.emplace_back(p2345);
	palettes.emplace_back(p2345);
	palettes.emplace_back(p2345);
	palettes.emplace_back(p6);
	palettes.emplace_back(p7);

	std::ofstream out("sprites\\palettes.asset", std::ios::binary);
	write_chunk("plt0", palettes, &out);
}


void write()
{
	//std::string str = "sprites/test1.png";
	//load_png(str, &t.size, &t.data, UpperLeftOrigin);

	std::vector<DummyData> tiles_out;

	for (int i = 0; i < 10; i++)
	{
		DummyData t;
		t.a = i;
		t.b = i;
		t.c = i;
		tiles_out.push_back(t);

	}
	
	//std::cout << t.size.x << "," << t.size.y << std::endl;
	// write to file
	std::ofstream out("test1.kk", std::ios::binary);
	write_chunk("spr0", tiles_out, &out);
	out.close(); // or put in {}

	std::cout << std::endl;
}

void read()
{
	// read file
	std::vector<DummyData> tiles_in;
	std::ifstream in("test1.kk", std::ios::binary);
	if (in)
	{
		read_chunk(in, "spr0", &tiles_in);
		for (uint32_t i = 0; i < tiles_in.size(); i++)
		{
			std::cout << tiles_in[i].a << std::endl;
		}
		std::cout << std::endl;
	}
	else
	{
		std::cout << "Failed to open." << std::endl;
	}
	//in.close();
}
int main()
{
	//write();
	//read();
	write_palettes();
	return 0;


}