#include <bn_core.h>
#include <bn_math.h>
#include <bn_random.h>
#include <bn_keypad.h>
#include <bn_bg_palettes.h>
#include <bn_sprite_builder.h>
#include <bn_sprite_text_generator.h>

#include "bn_sprite_items_block_0.h"
#include "bn_sprite_items_block_2.h"
#include "bn_sprite_items_block_4.h"
#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

struct block {
	int x, y, n;
	bn::optional<bn::sprite_ptr> sprite;

	block() = default;

	block(int _x, int _y, int _n) : x(_x), y(_y) {
		change_n(_n);
	}

	void change_n(int _n) {
		n = _n;
		switch (n) {
			case 0:
				sprite = bn::sprite_items::block_0.create_sprite(x, y);
				break;
			case 2:
				sprite = bn::sprite_items::block_2.create_sprite(x, y);
				break;
			case 4:
				sprite = bn::sprite_items::block_4.create_sprite(x, y);
				break;
			default:
				break;
		}
	}
};

struct board {
    block blocks[4][4];
    bn::random random;

    board() {
		reset();
    }

	void reset() {
        constexpr int size = 32, space = 4;
        constexpr int total = 4 * size + 3 * space;
        constexpr int offset = -total / 2 + size / 2;

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                const int x = offset + j * (size + space);
                const int y = offset + i * (size + space);
                blocks[i][j] = block(x, y, 0);
            }
        }

		int i = random.get_int(4);
		int j = random.get_int(4);
		blocks[i][j].change_n(2);
	}

	void lost() {
		reset();
	}

	void spawn() {
		int k = 0;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
				k += blocks[i][j].n == 0;
			}
		}

		if (k == 0)
			return lost();

		int r = random.get_int(k);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
				if (blocks[i][j].n > 0)
					continue;

				if (r-- == 0) {
					blocks[i][j].change_n(2);
					return;
				}
			}
		}
	}

	void move_x(int dir) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
				int c = dir > 0 ? 3 - j : j;

				if (blocks[i][c].n == 0)
					continue;

                int k = c;
				while (k + dir >= 0 && k + dir < 4 && blocks[i][k + dir].n == 0) {
					blocks[i][k + dir].change_n(blocks[i][k].n);
					blocks[i][k].change_n(0);
					k += dir;
				}
			}
        }
		spawn();
	}

	void move_y(int dir) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
				int r = dir > 0 ? 3 - i : i;

				if (blocks[r][j].n == 0)
					continue;

                int k = r;
				while (k + dir >= 0 && k + dir < 4 && blocks[k + dir][j].n == 0) {
					blocks[k + dir][j].change_n(blocks[k][j].n);
					blocks[k][j].change_n(0);
					k += dir;
				}
			}
        }
		spawn();
	}

	void update() {
        if (bn::keypad::a_pressed())
			return reset();

        if (bn::keypad::up_pressed())
			return move_y(-1);

        if (bn::keypad::down_pressed())
			return move_y(1);

        if (bn::keypad::left_pressed())
			return move_x(-1);

        if (bn::keypad::right_pressed())
			return move_x(1);
	}
};

int main()
{
    bn::core::init();

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bn::bg_palettes::set_transparent_color(bn::color(31, 31, 31));

    board board;

    while(true)
    {
		board.update();
        bn::core::update();
    }
}
