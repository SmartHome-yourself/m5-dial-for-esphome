#!/usr/bin/env python3
"""
Generate a 70x70 fireplace icon in RGB565 as a C++ PROGMEM array.
VERSION 3:
- Thinner hearth base (half height)
- Multiple flames (3 flames - center largest, two smaller on sides)
- Flames don't touch top

Output: fireplace_icon.h
"""

import math

W, H = 70, 70

def rgb565(r, g, b):
    """Convert 8-bit RGB to 16-bit RGB565."""
    return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3)

# Colors (tweak as you like)
BLACK       = rgb565(0, 0, 0)
BG          = rgb565(10, 10, 10)       # background dark
WALL_DARK   = rgb565(40, 40, 40)
WALL_LIGHT  = rgb565(90, 90, 90)
HEARTH      = rgb565(110, 70, 40)
FIREBOX     = rgb565(5, 5, 5)

FLAME_OUTER = rgb565(255, 140, 0)
FLAME_INNER = rgb565(255, 90, 0)
FLAME_CORE  = rgb565(255, 230, 0)
EMBER       = rgb565(220, 80, 0)

# Create blank image
img = [[BG for _ in range(W)] for _ in range(H)]

# ---- Draw Fireplace ----

# Hearth base - THINNER (half of v2 - now 7 rows instead of 14)
for y in range(63, 70):  # bottom 7 rows (was 56-70)
    for x in range(0, W):
        img[y][x] = HEARTH

# Outer wall / arch - full width, adjusted for thinner hearth
for y in range(0, 63):  # from top to hearth (was 0 to 56)
    for x in range(0, W):
        img[y][x] = WALL_LIGHT

# Inner firebox rectangle - slightly adjusted
for y in range(14, 57):  # adjusted for new hearth position (was 14-50)
    for x in range(8, W - 8):
        img[y][x] = FIREBOX

# Add a slightly darker border around firebox to emphasize shape
for y in range(13, 58):  # adjusted (was 13-51)
    for x in range(7, W - 7):
        if (y == 13 or y == 57 or x == 7 or x == W - 8):
            img[y][x] = WALL_DARK

# ---- Draw THREE Flames (center largest, two smaller on sides) ----

def draw_flame(cx, cy, width_scale, height_scale, min_y):
    """Draw a single flame at given center with given scale"""
    for y in range(int(cy - height_scale * 14), int(cy + height_scale * 4)):
        if y < min_y or y >= 57:  # don't go above min_y or into hearth
            continue
        for x in range(int(cx - width_scale * 10), int(cx + width_scale * 10)):
            if x < 8 or x >= W - 8:  # stay inside firebox
                continue

            dx = (x - cx) / (width_scale * 10.0)
            dy = (y - cy) / (height_scale * 14.0)
            r2 = dx * dx + dy * dy

            # elliptical flame region
            if r2 < 1.0:
                if r2 < 0.20:
                    col = FLAME_CORE
                elif r2 < 0.50:
                    col = FLAME_INNER
                else:
                    col = FLAME_OUTER
                img[y][x] = col

# Center flame (largest) - doesn't touch top
draw_flame(W // 2, 38, 1.0, 1.0, 20)  # center at y=38, don't go above y=20

# Left flame (smaller)
draw_flame(W // 2 - 12, 42, 0.6, 0.7, 28)  # smaller, lower, don't go above y=28

# Right flame (smaller)
draw_flame(W // 2 + 12, 42, 0.6, 0.7, 28)  # smaller, lower, don't go above y=28

# Embers at bottom of firebox
cx = W // 2
for y in range(52, 57):  # adjusted position (was 46-50)
    for x in range(cx - 15, cx + 15):  # wider ember area
        if x < 8 or x >= W - 8:  # stay inside firebox
            continue
        # a bit random-looking but deterministic: stripe pattern
        if (x + y) % 3 != 0:
            img[y][x] = EMBER

# ---- Write C++ header in global_image_data.h format ----

# Flatten the 2D image array to 1D
pixels = []
for y in range(H):
    for x in range(W):
        pixels.append(img[y][x])

with open("fireplace_icon.h", "w") as f:
    f.write("// Icons: 70x70px - Fireplace icon (V3 - thin hearth, 3 flames)\n\n")
    f.write("#ifndef GLOBAL_FIREPLACE_IMG_H\n")
    f.write("#define GLOBAL_FIREPLACE_IMG_H\n")
    f.write("    static const unsigned short FIREPLACE_IMG[0x1324] PROGMEM = {\n")

    # Write 16 pixels per line with proper formatting
    for i in range(0, len(pixels), 16):
        chunk = pixels[i:i+16]
        hex_values = ", ".join(f"0x{val:04X}" for val in chunk)
        hex_offset = i + 16
        comment = f"// 0x{hex_offset:04X} ({hex_offset})"

        # Handle last line (may have fewer than 16 values)
        if i + 16 >= len(pixels):
            # Last line
            f.write(f"        {hex_values}\n")
        else:
            f.write(f"        {hex_values},   {comment}\n")

    f.write("    };\n")
    f.write("#endif\n")

print("Generated fireplace_icon.h (V3 - thinner hearth, 3 shorter flames)")
