#include <iostream>
#include <ofia/application.hpp>

using namespace ofia;

int main()
{
    try
    {
        Application app;
        app.Run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "[FATAL] Exception: " << e.what() << std::endl;
        return -1;
    }
    catch (...)
    {
        std::cerr << "[FATAL] Unknown exception occurred!" << std::endl;
        return -2;
    }

    return 0;
}
