#include "rp2040/console.h"

// Console::Console()
//     : mDone(false)
// {
//
// }
char console_input[CONSOLE_INPUT_LENGTH] = {0};
int console_input_count = 0;

void console_flush_input()
{
    memset(console_input, '\0', CONSOLE_INPUT_LENGTH);
    console_input_count = 0;
}

void console_evaluate(char *input, const size_t &length, const char delimiter = ' ')
{
    LOG_TRACE("Evaluate Line %s\n", console_input);

    size_t start = 0;

    int32_t wordCount = 0;
    const char *word = nullptr;

    bool done = false;

    for(size_t i = 0; (i < length) && !done; i++) {
        if(input[i] == '\0') {
            // We've reached the end of the line
            word = &input[start];
            LOG_TRACE("Word (%d): %s\n", wordCount, word);
            LOG_TRACE("EOL\n");
            done = true;
        } else if(input[i] == delimiter) {
            // We've reached the end of our word
            input[i] = '\0';
            word = &input[start];
            LOG_TRACE("Word (%d): %s\n", wordCount, word);
            start = i + 1;
            wordCount++;
        }
    }

}

void console_run()
{
    console_flush_input();

    while(true) {
        int input = getchar_timeout_us(50);
        if(input == PICO_ERROR_TIMEOUT) {
            // This is okay, just loop back around
        } else if(input == '\r') {
            // Evaluate the input string
            console_input[console_input_count] = 0;
            console_evaluate(console_input, sizeof(console_input));
            // Once the input has been handled, flush the container
            console_flush_input();
        } else {
            LOG_TRACE("Received: 0x%02X\n", input);
            console_input[console_input_count] = input;
            console_input_count++;
        }
    }
}
