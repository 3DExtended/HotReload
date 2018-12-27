# HotReload

This library allows to look for file-changes (i.e. delete, modify or create actions) within specified path(s).
It is based off of the library [simplefilewatcher](https://github.com/apetrone/simplefilewatcher) by Adam Petrone (alias apetrone) and adds a thread to watch our for file changes. This way, the application won't consume as much CPU bandwidth as it updates the watches only every second.

# Cloning

Please make sure to clone the submodules used, too. For example using this git command:

> git clone --recurse-submodules https://github.com/3DExtended/HotReload.git

# Usage

There is an example located [here](https://github.com/3DExtended/HotReload/blob/master/example/Example.cc) but here is the basic things you can do:

## Creating a callback

A callback is the function that is called whenever the corresponding directory is modified (i.e. some file changes within the directory).
I used the following lambda function definiton for callbacks:
```cpp
std::function<void(const std::string& path, const std::string& filename, FW::Action action)> callMe = []
(const std::string& path,
    const std::string& filename,
    FW::Action action)
{
    std::cout << "DIR (" << path + ") FILE (" + filename + ") has event " << action << std::endl;
};
```

For convinience, I added a ```typedef``` called "callback" which allows you to write this instead:
```cpp
callback callMe = []
(const std::string& path,
    const std::string& filename,
    FW::Action action)
{
    std::cout << "DIR (" << path + ") FILE (" + filename + ") has event " << action << std::endl;
};
```

## Registering the callback

To register a new watch, do something like this:

```cpp
auto hr = HR::HotReload::instance();
hr->registerCallback("./changeDir", callMe);
```

This call will create a watch on the directory "changeDir" (relative paths start at the working directory of the application) which will call the function callMe when ever one of the following conditions are hit:

* A file was created (action=1)
* A file was deleted (action=2)
* A file was modified (action=4)

## Update

In order to call the callbacks from the main thread, you have to call ```hr->update()``` regularly. This function will call the callback function, whenever the watches saw an event. If there were no file changes, this function will do nothing (except handling addition and deletions of watches).

```cpp
while (true)
{
    // do something
    hr->update(); //callbacks might be called
    // do another something
}
```


## Remove a callback/watch

In order to delete a watch (and the corresponding callback function), call the ```hr->removeCallback(str)``` function like in this example:

```cpp
HR::HotReload::instance()->removeCallback("./changeDir");
```

__NOTE:__ If you wish to update a callback function for a given path, please make sure to call ```hr->update()``` after deleting the callback and before creating a new watch (Race-condition of multi-threaded watch support).

# Example

```cpp
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
        hr->update();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    return 0;
}
```

# License

[MIT License](https://github.com/3DExtended/HotReload/blob/master/LICENSE)
