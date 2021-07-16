#ifndef TEXT_H
#define TEXT_H

int getTextSize             (const char* text, int* columns, int* rows);
int getTextPositionCentered (const char* text, int font_w, int font_h, int area_w, int area_h, int* x, int* y);

#endif
