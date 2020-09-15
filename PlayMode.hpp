#include "PPU466.hpp"
#include "Mode.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----

	//input tracking:
	struct Button {
		//uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up, space;

	// robot attributes 	
	glm::vec2 robot_velocity = glm::vec2(0.0f, 0.0f);
	const float robot_speed = 70.0f;
	int orientation = 0; // 0 up, 1 down, 2 left, 3 right

	// enemy attributes
	glm::vec2 enemy1_velocity = glm::vec2(0.0f, 0.0f);
	glm::vec2 enemy2_velocity = glm::vec2(0.0f, 0.0f);
	glm::vec2 enemy3_velocity = glm::vec2(0.0f, 0.0f);
	const float enemy_speed = 50.0f;

	//player position:
	glm::vec2 player_at = glm::vec2(0.0f);
	glm::vec2 enemy1_at = glm::vec2(200, 200);
	glm::vec2 enemy2_at = glm::vec2(20, 200);
	glm::vec2 enemy3_at = glm::vec2(200, 20);

	// dimensions
	float court_width_height = 240;
	float robot_radius = 4;

	//game over
	bool game_over = false;

	//beam idx
	int beam_idx = 2;
	const int max_beams = 8;

	//----- drawing handled by PPU466 -----

	PPU466 ppu;

	// loading into ppu helper
	void insert_tile_table(std::vector< glm::u8vec4 > const* sprite_ptr, int idx);
};
