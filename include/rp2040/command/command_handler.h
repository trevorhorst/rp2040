#ifndef FAWKES_COMMAND_HANDLER_H
#define FAWKES_COMMAND_HANDLER_H

#include <vector>

#include "rp2040/json/cjson.h"
#include "rp2040/command/command.h"
#include "rp2040/control/control_template.h"

#define COMMAND_NAME_CMD        "cmd"
#define COMMAND_NAME_PARAMS     "params"
#define COMMAND_NAME_RESULTS    "results"
#define COMMAND_NAME_SUCCESS    "success"

class CommandHandler
        : public ControlTemplate< CommandHandler >
{
public:
    CommandHandler();

    void addCommand( Command *command );
    int32_t process( const char *data, char **response );

    const Types::CharHashMap< Command* > *map();
    const Types::CharMap< Command* > sortedMap();

private:
    Types::CharHashMap< Command* > mCommandMap;
};

#endif // FAWKES_COMMAND_HANDLER_H
