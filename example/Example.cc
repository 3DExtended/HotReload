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
    //HR::CallbackParameter temp = { 111, std::string("Huhu"),std::string("Huhu") ,FW::Actions::Add};

    while (true)
    {
        std::cout << "Running..." << std::endl;
        hr->update();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    return 0;
}//*/

/*
#include <FileWatcher/FileWatcher.h>
#include <iostream>
#include <stdio.h>

/// Processes a file action
class UpdateListener : public FW::FileWatchListener
{
public:
    UpdateListener() {}
    void handleFileAction(FW::WatchID watchid, const FW::String& dir, const FW::String& filename,
        FW::Action action)
    {
        std::cout << "DIR (" << dir + ") FILE (" + filename + ") has event " << action << std::endl;
    }
};


int main(int argc, char **argv)
{
    try
    {
        // create the listener (before the file watcher - so it gets destroyed after the file watcher)
        UpdateListener listener;

        // create the file watcher object
        FW::FileWatcher fileWatcher;

        // add a watch to the system
        // the file watcher doesn't manage the pointer to the listener - so make sure you don't just
        // allocate a listener here and expect the file watcher to manage it - there will be a leak!
        FW::WatchID watchID = fileWatcher.addWatch("./changeDir", &listener, true);
        FW::WatchID watchID2 = fileWatcher.addWatch("./changeDir2", &listener, true);

        std::cout << "Press ^C to exit demo" << std::endl;

        // loop until a key is pressed
        while (1)
        {
            fileWatcher.update();
        }
    }
    catch (std::exception& e)
    {
        fprintf(stderr, "An exception has occurred: %s\n", e.what());
    }

    return 0;
}
//*/