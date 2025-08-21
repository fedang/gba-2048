#include <bn_core.h>
#include <bn_math.h>
#include <bn_keypad.h>
#include <bn_bg_palettes.h>
#include <bn_sprite_builder.h>
#include <bn_sprite_text_generator.h>

#include "bn_sprite_items_block_2.h"
#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

struct board {
    bn::optional<bn::sprite_ptr> blocks[4][4];

    board() {
        constexpr int size = 32, space = 4;
        constexpr int total = 4 * size + 3 * space;
        constexpr int offset = -total / 2 + size / 2;

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                const int x = offset + i * (size + space);
                const int y = offset + j * (size + space);
                blocks[i][j] = bn::sprite_items::block_2.create_sprite(x, y);
            }
        }
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
        bn::core::update();
    }
}
