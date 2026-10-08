from PIL import Image, ImageDraw, ImageFont, ImageColor
import json

FONT_PATH = "/usr/share/fonts/pressstart2p/PressStart2P-Regular.ttf"

def darken(color, factor):
    rgb = ImageColor.getrgb(color)
    return tuple(max(0, min(255, int(channel * factor))) for channel in rgb)

def make_block(bg):
    img = Image.new("RGB", (32, 32), color=bg)
    draw = ImageDraw.Draw(img)

    corners = [(0,0), (0,1), (1,0),
               (31,0), (30,0), (31,1),
               (0,31), (0, 30), (1, 31),
               (31,31), (30,31), (31,30)]

    draw.point(corners, fill=(0,0,0,0))

    shadow1 = darken(bg, 0.8)
    shadow2 = darken(bg, 0.7)

    draw.line([(2,31), (29,31)], width=1, fill=shadow1)

    draw.line([(31,2), (31,29)], width=1, fill=shadow2)
    draw.point([(30,30)], fill=shadow2)

    return img, draw


BLOCKS = [
        (0,     "#cdc1b4",      "#000000"),
        (2,     "#eee4da",      "#776e65"),
        (4,     "#ede0c8",      "#776e65"),
        (8,     "#f2b179",      "#f9f6f2"),
        (16,    "#f59563",      "#f9f6f2"),
        (32,    "#f67c5f",      "#f9f6f2"),
        (64,    "#f65e3b",      "#f9f6f2"),
        (128,   "#edcf72",      "#f9f6f2"),
        (256,   "#edcc61",      "#f9f6f2"),
        (512,   "#edc850",      "#f9f6f2"),
        (1024,  "#edc53f",      "#f9f6f2"),
        (2048,  "#edc22e",      "#f9f6f2"),
]

for n, bg, fg in BLOCKS:
    img, draw = make_block(bg)

    if n > 0:
        fsize = 16 if len(str(n)) <= 2 else 8
        font = ImageFont.truetype(FONT_PATH, size=fsize)
        draw.text((16, 16), str(n), fill=fg, font=font, anchor="mm")

    s_img = img.quantize(colors=16, method=Image.Quantize.MAXCOVERAGE)

    s_img.save(f"./block_{n}.bmp", format="BMP")

    sprite = {
        "type": "sprite",
        "height": 32
    }

    with open(f"./block_{n}.json", "w") as f:
        json.dump(sprite, f)
