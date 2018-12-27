#pragma once
#include <chrono>
#include <thread>
#include <HotReload.hh>
#include <iostream>

int main(int argc, char const *argv[])
{

    std::function<void(const std::string& path, const std::string& filename, FW::Action action)> callback = []
    (const std::string& path, 
        const std::string& filename, 
        FW::Action action) 
    {
        std::cout << "DIR (" << path + ") FILE (" + filename + ") has event " << action << std::endl;
    };

    auto hr = HR::HotReload::instance();
    hr->registerCallback("./changeDir", callback);

    while (true)
    {
        std::cout << "Try changing a file within ./changeDir..." << std::endl;
        hr->update(); // call me regulary (it does nothing if there was no file change)
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    return 0;
}