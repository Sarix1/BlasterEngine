int getTextSize(const char* text, int* columns, int* rows)
{
    int num_chars   = 0;
    int num_rows    = 1;
    int current_row = 0;
    int longest_row = 0;
    char* c;

    for (c = text; *c != '\0'; c++)
    {
        current_row++;

        if (*c == '\n')
        {
            num_rows++;
            current_row = 0;
        }
        else
        {
            num_chars++;

            if (current_row > longest_row)
                longest_row = current_row;
        }
    }

    *rows = num_rows;
    *columns = longest_row;

    return num_chars;
}

int getTextPositionCentered(const char* text, int font_w, int font_h, int area_w, int area_h, int* x, int* y)
{
    int chars, columns, rows;

    chars = getTextSize(text, &columns, &rows);
    *x = ((area_w / font_w) - columns) / 2;
    *y = ((area_h / font_h) - rows) / 2;

    return chars;
}
