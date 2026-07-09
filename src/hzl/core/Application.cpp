#include "hzl/core/Application.h"

#include <iostream>

namespace hzl
{
    Application::Application()
    {
        std::cout << "Application created.\n";
    }

    void Application::run()
    {
        std::cout << "HZL Atom Simulation running...\n";
    }
}
