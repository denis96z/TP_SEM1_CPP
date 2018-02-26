#include <stdio.h>
#include <stdlib.h>

#define LINE_LENGTH_INCREMENT 10

size_t read_lines(char ***lines);
void print_lines(char **, size_t);

size_t filter_lines(char*** match_lines, char **lines, size_t num_lines);

char** realloc_lines_memory(char **, size_t, size_t);
char* realloc_line_memory(char *, size_t, size_t);
void free_lines_memory(char **, size_t);

#define PRINT_ERROR printf("%s", "[error]");

int main(void)
{
    char **lines = NULL;
    size_t num_lines = read_lines(&lines);
    if (!lines)
    {
        PRINT_ERROR;
        return 0;
    }

    char **matches = NULL;
    size_t num_matches = filter_lines(&matches, lines, num_lines);
    if (!matches)
    {
        PRINT_ERROR;
    }

    print_lines(matches, num_matches);

    free_lines_memory(lines, num_lines);
    free(matches);

    return 0;
}

size_t read_lines(char ***lines)
{
    size_t line_index = 0, line_length = 0, column_index = 0;

    char in_char;
    char **all_lines = NULL;
    while (scanf("%c", &in_char) == 1)
    {
        if (in_char == '\n')
        {
            if (column_index)
            {
                ++line_index;
                line_length = 0;
                column_index = 0;
            }
            else
            {
                break;
            }
        }
        else
        {
            if (!column_index)
            {
                all_lines = realloc_lines_memory(all_lines, line_index, line_index + 1);
                if (!all_lines)
                {
                    return 0;
                }
            }

            if (column_index == line_length)
            {
                all_lines[line_index] = realloc_line_memory(all_lines[line_index],
                                                            line_length, line_length + LINE_LENGTH_INCREMENT + 1);
                if (!all_lines[line_index])
                {
                    free_lines_memory(all_lines, line_index + 1);
                    return 0;
                }

                line_length += LINE_LENGTH_INCREMENT;
            }

            all_lines[line_index][column_index++] = in_char;
        }
    }

    if (column_index)
    {
        ++line_index;
    }

    *lines = all_lines;
    return line_index;
}

size_t filter_lines(char*** match_lines, char **lines, size_t num_lines)
{
    char **new_lines = malloc(num_lines * sizeof(char*));
    if (!new_lines)
    {
        return 0;
    }

    size_t num_new_lines = 0;
    for (size_t i = 0; i < num_lines; ++i)
    {
        int brackets_counter = 0;
        for (char *c = lines[i]; *c; ++c)
        {
            switch (*c) {
                case '(':
                    ++brackets_counter;
                    break;

                case ')':
                    --brackets_counter;
                    break;
            }
        }
        if (!brackets_counter)
        {
            new_lines[num_new_lines++] = lines[i];
        }
    }

    *match_lines = new_lines;
    return num_new_lines;
}

void print_lines(char **lines, size_t num_lines)
{
    for (size_t i = 0; i < num_lines; ++i)
    {
        printf("%s\n", lines[i]);
    }
}

char** realloc_lines_memory(char **lines, size_t current_size, size_t new_size)
{
    char **new_lines = (char**)realloc(lines, new_size * sizeof(char*));
    if (!new_lines)
    {
        free_lines_memory(lines, current_size);
    }

    for (size_t i = current_size; i < new_size; ++i)
    {
        new_lines[i] = NULL;
    }

    return new_lines;
}

char* realloc_line_memory(char *line, size_t current_size, size_t new_size)
{
    char *new_line = (char*)realloc(line, new_size * sizeof(char));
    if (!new_line)
    {
        free(line);
    }
    else
    {
        for (size_t i = current_size; i < new_size; ++i)
        {
            new_line[i] = '\0';
        }
    }
    return new_line;
}

void free_lines_memory(char **lines, size_t num_lines)
{
    for (size_t i = 0; i < num_lines; ++i)
    {
        free(lines[i]);
    }
    free(lines);
}
