#include <stdio.h>

#define BUFFER_SIZE 32768

typedef struct formatter_state {
    int level;
    int last;
    int escaped;
    int in_string;
} formatter_state;

int buffer_offset = 0;
char output_buffer[BUFFER_SIZE];

void flush_buffer()
{
    write(1, output_buffer, buffer_offset);
    buffer_offset = 0;
}

void print(char* str, int len)
{
    if ((buffer_offset + len) >= BUFFER_SIZE) {
        flush_buffer();
    }

    memcpy(output_buffer + buffer_offset, str, len);
    buffer_offset += len;
}

void padLine(int level)
{
    print("\n", 1);
    for (int i = 0; i < level; i++) {
        print("    ", 4);
    }
}

void format_buffer(char* buffer, formatter_state* state)
{
    int c;

    for (int i = 0; buffer[i] != '\0' && i < BUFFER_SIZE; i++) {
        c = buffer[i];

        if (state->in_string && c == '\\') { /* escape */
            state->escaped = !state->escaped;

        } else if (state->escaped) {
            state->escaped = 0;

            if ('"' == c) {
                print("\"", 1);
                state->last = '1';
                continue;
            }

        } else {
            if (state->last == '"')
                state->in_string = !state->in_string; /* quote */

            if (!state->in_string && state->last == ':') {
                print(" ", 1);
            }

            if (!state->in_string) {
                if (state->last == '{' || state->last == '[') {
                    padLine(++state->level);
                } else if (state->last == ',') {
                    padLine(state->level);
                }

                if (c == '}' || c == ']') {
                    if (state->level > 0) {
                        state->level--;
                    }
                    padLine(state->level);
                } else if (c == '{' || c == '[') {
                    if (state->last != ',' && state->last != '[' && state->last != '{') {
                        padLine(state->level);
                    }
                }
            }
        }

        state->last = c;
        if (state->in_string || !(' ' == c || '\t' == c || '\r' == c || '\n' == c)) {
            print(&c, 1);
        }
    }
}

int process_fp(FILE* fp)
{
    // Must have space for a potential ending \0, thus +1
    char buffer[BUFFER_SIZE + 1];

    formatter_state state;
    state.level = 0;
    state.last = -1;
    state.in_string = 0;
    state.escaped = 0;

    while (1) {
        char* result = fgets(buffer, BUFFER_SIZE, fp);
        if (result == NULL && feof(fp)) {
            break;
        } else if (result == NULL && ferror(fp)) {
            fprintf(stderr, "Error reading from file");
            break;
        }

        format_buffer(buffer, &state);
    }

    flush_buffer();
}

int main(int argc, char* argv[])
{
    FILE* fp = stdin;

    if (argc == 2) {
        fp = fopen(argv[1], "r");
        if (!fp) {
            fprintf(stderr, "Unable to open file: %s\n", argv[1]);
            return 1;
        }
    }

    process_fp(fp);

    if (fp != stdin) {
        fclose(fp);
    }
}
