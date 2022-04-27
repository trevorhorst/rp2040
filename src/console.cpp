#include "rp2040/console.h"

// Console::Console()
//     : mDone(false)
// {
//
// }

static CommandHandler *cmd_handler = nullptr;

char console_input[CONSOLE_INPUT_LENGTH] = {0};
int console_input_count = 0;

void console_set_command_handler(CommandHandler *handler)
{
    cmd_handler = handler;
}

void console_flush_input()
{
    memset(console_input, '\0', CONSOLE_INPUT_LENGTH);
    console_input_count = 0;
}

// void console_evaluate(char *input, const size_t &length, const char delimiter = ' ')
// {
//     LOG_TRACE("Evaluate Line %s\n", console_input);
//
//     size_t start = 0;
//
//     int32_t wordCount = 0;
//     const char *word = nullptr;
//
//     bool done = false;
//
//     for(size_t i = 0; (i < length) && !done; i++) {
//         if(input[i] == '\0') {
//             // We've reached the end of the line
//             word = &input[start];
//             LOG_TRACE("Word (%d): %s\n", wordCount, word);
//             LOG_TRACE("EOL\n");
//             done = true;
//         } else if(input[i] == delimiter) {
//             // We've reached the end of our word
//             input[i] = '\0';
//             word = &input[start];
//             LOG_TRACE("Word (%d): %s\n", wordCount, word);
//             start = i + 1;
//             wordCount++;
//         }
//     }
//
// }
std::vector<std::string> console_tokenize(char *input, const char *delimiter)
{
    std::string str( input );
    std::string token;
    std::vector< std::string > tokenized;
    size_t last = 0;
    size_t next = 0;

    // Split the input into segments by the delimeter
    while( ( next = str.find( delimiter, last ) ) != std::string::npos ) {
        if( next - last != 0 ) {
            // There is data in the string
            token = str.substr( last, next - last );
            tokenized.push_back( token );
        } else {
            // If the next and last values are the same, the string is empty
            // and we can ignore it
        }
        last = next + 1;
    }


    if( !str.substr( last ).empty() ) {
        // Need to place the the final substring in the list, unless it's empty
        tokenized.push_back( str.substr( last ) );
    }

    return tokenized;

}

void console_evaluate(char *input, const size_t &length)
{
    // Tokenize the input
    std::vector< std::string > tokenized = console_tokenize(input, " ");

    if( tokenized.empty() ) {
        // The input is empty
        LOG_TRACE("\n");
    } else if( tokenized.at( 0 ) == "quit" ) {
        // The command is to quit, so lets quit. Nothing fancy here.
        // console->quit();
    } else {

        // Add the cmd string to the object
        cJSON *msg    = cJSON_CreateObject();
        cJSON *params = cJSON_CreateObject();
        cJSON *cmd    = nullptr;

        for( auto it = tokenized.begin(); it != tokenized.end(); it++ ) {
            if( *it == tokenized.at( 0 ) ) {
                // Add the command parameter
                cmd = cJSON_CreateString( (*it).c_str() );
            } else {
                auto t = it;
                it++;
                if( it == tokenized.end() ) {
                    LOG_WARN( "Parameter mismatch" );
                    return;
                } else {
                    // Parse the parameter
                    cJSON *param = cJSON_Parse( (*it).c_str() );
                    // Add the item to the parameter list
                    cJSON_AddItemToObject( params, (*t).c_str(), param );
                }
            }
        }

        cJSON_AddItemToObject( msg, "cmd", cmd );
        cJSON_AddItemToObject( msg, "params", params );

        char *msgStr = cJSON_PrintUnformatted( msg );
        // char msgRsp[ 1024 ] = { 0 };
        char *msgRsp = new char[1024];

        cmd_handler->process(msgStr, &msgRsp);
        printf("%s\n", msgRsp);
        // if( console->client() ) {
        //     console->client()->send( msgStr, msgRsp, 1024 );
        //     cJSON *rsp = cJSON_Parse( msgRsp );
        //     if( rsp ) {
        //         char *str = cJSON_Print( rsp );
        //         printf( "%s\n", str );
        //         cJSON_free( str );
        //         // cJSON *results = cJSON_GetObjectItem( rsp, "results" );
        //         // if( results ) {
        //         //     char *str = cJSON_Print( results );
        //         //     printf( "%s\n", str );
        //         //     cJSON_free( str );
        //         // } else {
        //         //     cJSON *error = cJSON_GetObjectItem( rsp, "error" );
        //         //     if( error ) {
        //         //         char *str = cJSON_Print( error );
        //         //         printf( "%s\n", str );
        //         //         cJSON_free( str );
        //         //     }
        //         // }
        //     }
        //     cJSON_Delete( rsp );

        // } else {
        //     LOG_INFO( "No client available" );
        // }

        delete[] msgRsp;

        cJSON_free( msgStr );

        // Clean up the message, this should delete all the components
        cJSON_Delete( msg );

    }


}

void console_run()
{
    console_flush_input();

    printf(">");

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
            printf(">");
        } else if(input == '\b') {
            // LOG_TRACE("Received: 0x%02X\n", input);
            if(console_input_count > 0) {
                console_input_count--;
                console_input[console_input_count] = '\0';
            }
        } else {
            // LOG_TRACE("Received: 0x%02X\n", input);
            console_input[console_input_count] = input;
            console_input_count++;
        }
    }
}
