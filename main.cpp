#include "rp2040/application.h"

int main()
{
    stdio_init_all();

    Application app;

    return app.run();
}
