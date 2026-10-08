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
#include "bn_sprite_items_block_8.h"
#include "bn_sprite_items_block_16.h"
#include "bn_sprite_items_block_32.h"
#include "bn_sprite_items_block_64.h"
#include "bn_sprite_items_block_128.h"
#include "bn_sprite_items_block_256.h"
#include "bn_sprite_items_block_512.h"
#include "bn_sprite_items_block_1024.h"
#include "bn_sprite_items_block_2048.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

#define BLOCK_SPRITE(n) \
    bn::sprite_items::block_ ## n

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
            /* n=0 means empty tile */
            case 0:
                sprite = BLOCK_SPRITE(0).create_sprite(x, y);
                break;
            case 2:
                sprite = BLOCK_SPRITE(2).create_sprite(x, y);
                break;
            case 4:
                sprite = BLOCK_SPRITE(4).create_sprite(x, y);
                break;
            case 8:
                sprite = BLOCK_SPRITE(8).create_sprite(x, y);
                break;
            case 16:
                sprite = BLOCK_SPRITE(16).create_sprite(x, y);
                break;
            case 32:
                sprite = BLOCK_SPRITE(32).create_sprite(x, y);
                break;
            case 64:
                sprite = BLOCK_SPRITE(64).create_sprite(x, y);
                break;
            case 128:
                sprite = BLOCK_SPRITE(128).create_sprite(x, y);
                break;
            case 256:
                sprite = BLOCK_SPRITE(256).create_sprite(x, y);
                break;
            case 512:
                sprite = BLOCK_SPRITE(512).create_sprite(x, y);
                break;
            case 1024:
                sprite = BLOCK_SPRITE(1024).create_sprite(x, y);
                break;
            case 2048:
                sprite = BLOCK_SPRITE(2048).create_sprite(x, y);
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

        spawn(16);
        spawn(15);
    }

    void lost() {
        // TODO: Add a loss text
        reset();
    }

    void maybe_lost() {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (i > 0 && blocks[i][j].n == blocks[i-1][j].n)
                    return;

                if (j > 0 && blocks[i][j].n == blocks[i][j-1].n)
                    return;
            }
        }

        lost();
    }

    void spawn() {
        int k = 0;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                k += blocks[i][j].n == 0;
            }
        }

        spawn(k);

        if (k == 1)
            maybe_lost();
    }

    void spawn(int k) {
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
        bool action = false;
        bool merged[4][4] = {false};

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                int c = dir > 0 ? 3 - j : j;

                if (blocks[i][c].n == 0)
                    continue;

                int k = c;
                int n = blocks[i][k].n;

                while (k + dir >= 0 && k + dir < 4 && blocks[i][k + dir].n == 0) {
                    blocks[i][k + dir].change_n(n);
                    blocks[i][k].change_n(0);
                    k += dir;

                    merged[i][k + dir] = merged[i][k];
                    action = true;
                }

                if (k + dir >= 0 && k + dir < 4 && blocks[i][k + dir].n == n) {
                    if (!merged[i][k + dir]) {
                        blocks[i][k + dir].change_n(2*n);
                        blocks[i][k].change_n(0);

                        merged[i][k + dir] = true;
                        action = true;
                    }
                }
            }
        }

        if (action)
            spawn();
    }

    void move_y(int dir) {
        bool action = false;
        bool merged[4][4] = {false};

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                int r = dir > 0 ? 3 - i : i;

                if (blocks[r][j].n == 0)
                    continue;

                int k = r;
                int n = blocks[k][j].n;

                while (k + dir >= 0 && k + dir < 4 && blocks[k + dir][j].n == 0) {
                    blocks[k + dir][j].change_n(n);
                    blocks[k][j].change_n(0);
                    k += dir;

                    merged[k + dir][j] = merged[k][j];
                    action = true;
                }

                if (k + dir >= 0 && k + dir < 4 && blocks[k + dir][j].n == n) {
                    if (!merged[k + dir][j]) {
                        blocks[k + dir][j].change_n(2*n);
                        blocks[k][j].change_n(0);

                        merged[k + dir][j] = true;
                        action = true;
                    }
                }
            }
        }

        if (action)
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
    text_generator.set_center_alignment();

    bn::bg_palettes::set_transparent_color(bn::color(31, 31, 31));

    board board;

    while(true)
    {
        board.update();
        bn::core::update();
    }
}
