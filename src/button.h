#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"

typedef struct Button {
    Rectangle bounds;
    Color normalColor;
    Color hoverColor;
    Color pressedColor;
    bool isHovered;
    bool isPressed;
} Button;

// Initialize a new button with given position, size, and colors
Button CreateButton(float x, float y, float width, float height, Color normalColor, Color hoverColor, Color pressedColor);

// Update button state based on mouse input
void UpdateButton(Button *button);

// Draw the button
void DrawButton(Button *button, const char *text);

#endif // BUTTON_H