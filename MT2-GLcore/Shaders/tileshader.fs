#version 330

out vec4 fragColor;

uniform sampler2D u_image;
uniform sampler2D u_palette;

uniform int atlasWidth = 2048;
uniform int atlasHeight = 1024;
uniform int tileByteSize = 0x200;
uniform int rowByteStride = 16;
uniform int tilePixelSize = 32;

in vec2 texturePosition;
in vec2 pixelPosition;

flat in int tileIdentifier;
flat in int tileAttribute;

const int bitsPerByte = 8;
const int bitsPerNibble = 4;

const vec2 flips[] = vec2[] (
    vec2(1.0, 1.0),
    vec2(-1.0, 1.0),
    vec2(1.0, -1.0),
    vec2(-1.0, -1.0)
);


void main() {
    int palette = tileAttribute & 0x1f;
    int flipIndex = (tileAttribute & 0x60) >> 5;
    
    vec2 flippedCoord = (pixelPosition * flips[flipIndex] + 0.5) * tilePixelSize;
    
    ivec2 pixel = ivec2(
        min(int(flippedCoord.x), tilePixelSize - 1),
        min(int(flippedCoord.y), tilePixelSize - 1 ));

//    fragColor = vec4(pixel.x / 31.0, 1.0, pixel.y / 31.0, 1.0);
//    return;

    
    // we need to get the pixel color index, which requires reading four bytes from
    // the `index` texture
    
    vec2 pixelStride = vec2(1.0 / float(atlasWidth), 1.0 / float(atlasHeight));
    vec2 halfStride = pixelStride / 2.0;
            
    int pixelId =
    tileByteSize * tileIdentifier + // byte size of a tile
        (pixel.x / bitsPerByte) * bitsPerNibble + // number of bits per byte, bit per nibble
    rowByteStride * pixel.y; // bytes per row
    
    int xOff = pixelId % atlasWidth;
    int yOff = pixelId / atlasWidth;
    
    int bit = 7 - (pixel.x % 8);
    
    vec2 tileCoord1 = vec2(pixelStride.x * (xOff + 0) + halfStride.x, pixelStride.y * yOff + halfStride.y);
    vec2 tileCoord2 = vec2(pixelStride.x * (xOff + 1) + halfStride.x, pixelStride.y * yOff + halfStride.y);
    vec2 tileCoord4 = vec2(pixelStride.x * (xOff + 2) + halfStride.x, pixelStride.y * yOff + halfStride.y);
    vec2 tileCoord8 = vec2(pixelStride.x * (xOff + 3) + halfStride.x, pixelStride.y * yOff + halfStride.y);

    int b1 = (int(texture(u_image, tileCoord1).r * 255) & (1 << bit));
    int b2 = (int(texture(u_image, tileCoord2).r * 255) & (1 << bit));
    int b4 = (int(texture(u_image, tileCoord4).r * 255) & (1 << bit));
    int b8 = (int(texture(u_image, tileCoord8).r * 255) & (1 << bit));
    
    int idx =
        (b1 != 0 ? 1 : 0) |
        (b2 != 0 ? 2 : 0) |
        (b4 != 0 ? 4 : 0) |
        (b8 != 0 ? 8 : 0);

    if (idx == 15)
        discard;
    
    // the CPS palette is packed in ARGB format, so shift it
    vec4 cpsColor = texture(u_palette, vec2(idx / 15.0 , palette / 31.0));
    
    vec3 rawColor = cpsColor.gba;
    vec3 brightness = vec3(cpsColor.r);
    
    fragColor = vec4(rawColor * brightness, 1.0);
}
