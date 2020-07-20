#include "display.hpp"
#include "interrupts.hpp"

Display::Display() {
    cycles = 0;
    LX = 0;
    LY = 0;
}

Display::~Display() {

}

void Display::clock() {
    cycles++;

    switch (LCDCSTAT & 0x03) {
        case 0x00:  // HBlank
            OAMAccess = true;
            if (cycles >= 204) {
                cycles = 0;
                LX = 0;
                LY++;
                if (LYC == LY) {
                    setLYCInterrupt();
                    // TODO: interrupt stuff
                }
                if (LY == 144) {
                    setVBlankInterrupt();
                    // TODO: interrupt stuff
                    LCDCSTAT |= 0x01;
                    LCDCSTAT &= 0xFD;
                }
                else {
                    LCDCSTAT |= 0x02;
                    LCDCSTAT &= 0xFE;
                }
            }
            break;
        case 0x01:  // VBlank
            OAMAccess = true;
            if (cycles % 456 == 0) {
                LY++;
                if (LYC == LY) {
                    setLYCInterrupt();
                    // TODO: interrupt stuff
                }
            }
            if (cycles >= 4560) {
                cycles = 0;
                LY = 0;
                if (LYC == LY) {
                    setLYCInterrupt();
                    // TODO: interrupt stuff
                }
                LX = 0;
                LCDCSTAT |= 0x02;
                LCDCSTAT &= 0xFE;
                setOAMInterrupt();
                // TODO: interrupt stuff
            }
            break;
        case 0x02:
            OAMAccess = false;
            if (cycles >= 80) {
                spriteSelect();
                cycles = 0;
                LCDCSTAT |= 0x03;
            }
            break;
        case 0x03:
            OAMAccess = false;
            if (cycles >= 172) {
                if ((LCDC & (1 << 7)) >> 7 == 1) {
                    drawScanline();
                }
                LCDCSTAT &= 0xFC;
                cycles = 0;
                setHBlankInterrupt();
            }
            
    }
}

void Display::spriteSelect() {
    for (int item : visibleSprites) {
        item = -1;  // reset sprites
    }

    int index = 0;

    for (int i = 0; i < 0xA0; i += 4) {
        if (index < 10) {
            int y = OAM[i];
            if (y > 0 && y < 160) {
                if (y - 8 >= LY && y - 16 <= LY) {
                    int x = OAM[i + 1];
                    if (x > 0 && x < 168) {
                        visibleSprites[index] = i;
                    }
                    else {
                        visibleSprites[index] = -1;  // not drawn
                    }
                    index++;
                }
            }
        }
    }
}

void Display::drawScanline() {
    int offsetY = (int) (((float) LY + (float) SCY) / 8);
    int offsetX = (int) (((float) SCX + (float) LX) / 8);
    uint16_t tileOffset = ((SCY + LY) % 8) * 2;
    uint16_t startAddress = getBackgroundMapAddress() + (offsetY * 32) + offsetX;
    uint16_t windowStartAddress = getWindowMapAddress() + (WY * 4) + WX;

    for (int i = 0; i < 20; i++) {
        uint16_t start = (getBackgroundStartAddress() + VRAM[(windowStartAddress + i - 0x8000) & 0x1FFF] * 16) - 0x8000;
        if (LY >= WY && LX >= WX - 7 && windowEnable()) {
            tileOffset = (LY % 8) * 2;
            if (getBackgroundStartAddress() == 0x8000) {
                drawBackgroundLine(VRAM[start + tileOffset], VRAM[start + tileOffset + 1]);
            }
            else {
                uint8_t tileNumber = VRAM[(windowStartAddress + i - 0x8000) & 0x1FFF];
                if (tileNumber <= 127) {
                    start = (getBackgroundStartAddress() + VRAM[(windowStartAddress + i - 0x8000) & 0x1FFF] * 16) - 0x8000;
                }
                else {
                    start = tileNumber * 16;
                }
                drawBackgroundLine(VRAM[start + tileOffset], VRAM[start + tileOffset + 1]);
            }
        }
        else {
            start = (getBackgroundStartAddress() + VRAM[(startAddress + i - 0x8000) & 0x1FFF] * 16) - 0x8000;
            if (getBackgroundStartAddress() == 0x8000) {
                drawBackgroundLine(VRAM[start + tileOffset], VRAM[start + tileOffset + 1]);
            }
            else {
                uint8_t tileNumber = VRAM[(startAddress + i - 0x8000) & 0x1FFF];
                if (tileNumber <= 127) {
                    start = (getBackgroundStartAddress() + VRAM[(startAddress + i - 0x8000) & 0x1FFF] * 16) - 0x8000;
                }
                else {
                    start = tileNumber * 16;
                }
                drawBackgroundLine(VRAM[start + tileOffset], VRAM[start + tileOffset + 1]);
            }
        }

        for (int j = 0; j < 10; j++) {
            if ((LCDC & (1 << 1)) >> 1 == 0x1 && visibleSprites[j] >= 0) {
                if (OAM[visibleSprites[j] + 1] - 8 >= LX && OAM[visibleSprites[j] + 1] <= LX + 8) {
                    start = OAM[visibleSprites[j] + 2] * 16;
                    tileOffset = (LY % 8) * 2;
                    drawSpriteLine((OAM[visibleSprites[j] + 3] & (1 << 4)) >> 4, VRAM[start + tileOffset], VRAM[start + tileOffset + 1]);
                }
            }
        }
    }
}

void Display::drawSpriteLine(uint8_t attr, uint8_t left, uint8_t right) {
    for (int i = 0; i < 8; i++) {
        uint8_t selection = ((((1 << (7 - i)) & left)) >> (7 - i)) << 1 | ((1 << (7 - i)) & right) >> (7 - i);
        uint8_t color = 0;
        if (spritePalette(attr) == 1) {
            color = (OBP1 & (0x03 << (selection * 2))) >> (selection * 2);
        }
        else {
            color = (OBP0 & (0x03 << (selection * 2))) >> (selection * 2);
        }

        if (color != 0) {
            frameBuffer[LY][LX + i] = color;
        }
    }
}

void Display::drawBackgroundLine(uint8_t left, uint8_t right) {
    for (int i = 0; i < 8; i++) {
        uint8_t selection = ((((1 << (7 - i)) & left)) >> (7 - i)) << 1 | ((1 << (7 - i)) & right) >> (7 - i);
        uint8_t color = (BGP & (0x03 << (selection * 2))) >> (selection * 2);
        frameBuffer[LY][LX + i] = color;
    }
}

uint16_t Display::getBackgroundStartAddress() {
    if (LCDC & 0x10 >> 4 == 1) {
        return 0x8000;
    }
    else {
        return 0x9000;
    }
}

uint16_t Display::getBackgroundMapAddress() {
    if (LCDC & 0x08 >> 3 == 1) {
        return 0x9C00;
    }
    else {
        return 0x9800;
    }
}

uint16_t Display::getWindowMapAddress() {
    if (LCDC & 0x40 >> 6 == 1) {
        return 0x9C00;
    }
    else {
        return 0x9800;
    }
}

void Display::setLYCInterrupt() {
    LCDCSTAT |= 1 << 6;
}

void Display::setOAMInterrupt() {
    LCDCSTAT |= 1 << 5;
}

void Display::setVBlankInterrupt() {
    LCDCSTAT |= 1 << 4;
}

void Display::setHBlankInterrupt() {
    LCDCSTAT |= 1 << 3;
}

uint8_t Display::spritePalette(uint8_t attr) {
    return (attr & 0x10) >> 4;
}

bool Display::windowEnable() {
    return ((LCDC & (1 << 5)) >> 5) == 1;
}

std::array<std::array<uint8_t, 160>, 144> Display::getFrameBuffer() {
    return frameBuffer;
}