#include "button.h"

Button CreateButton(float x, float y, float width, float height, Color normalColor, Color hoverColor, Color pressedColor)
{
    Button button;
    button.bounds = (Rectangle){ x, y, width, height };
    button.normalColor = normalColor;
    button.hoverColor = hoverColor;
    button.pressedColor = pressedColor;
    button.isHovered = false;
    button.isPressed = false;
    return button;
}

void UpdateButton(Button *button) {
    Vector2 mousePoint = GetMousePosition();
    button->isHovered = CheckCollisionPointRec(mousePoint, button->bounds);

    if (button->isHovered) 
    {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            button->isPressed = true;
        } 
        else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && button->isPressed) {
            button->isPressed = false;
        }

    } else {
        button->isPressed = false;
    }
}

void DrawButton(Button *button, const char *text) {
    Color color = button->normalColor;
    if (button->isHovered) {
        color = button->hoverColor;
    }
    if (button->isPressed) {
        color = button->pressedColor;
    }

    DrawRectangleRec(button->bounds, color);
    int textWidth = MeasureText(text, 20);
    int textX = button->bounds.x + (button->bounds.width - textWidth) / 2;
    int textY = button->bounds.y + (button->bounds.height - 20) / 2;
    DrawText(text, textX, textY, 20, BLACK);
}