#include "HotReload.hh"

using namespace HR;

inline std::shared_ptr<HotReload> HR::HotReload::getHotReload()
{
    return std::make_shared<MakeSharedEnabler>();
}

inline void HR::HotReload::registerCallback(std::string path, std::function<void(const std::string&, const std::string&, FW::Action)> callback)
{
    // add a watch to the system
    auto gWatchID = gFileWatcher->addWatch(path, new QueueCallback());
    this->watches[path] = gWatchID;
    this->callbacks[gWatchID] = callback;
}

void HR::HotReload::removeCallback(std::string path)
{
    this->watches.erase(path);
    this->gFileWatcher->removeWatch(path);
}

void HR::HotReload::update()
{
    for each (auto callbackParams in queuedCallbacks)
    {
        callbacks[callbackParams.watchID](
            callbackParams.dir,
            callbackParams.filename,
            callbackParams.action);
    }
    queuedCallbacks.clear();
}

inline HR::HotReload::~HotReload() { fetchNewChanges = false; }

inline HR::HotReload::HotReload() {
    updateFileChanges = new std::thread(&HR::HotReload::fetchFileChanges, this);
}

inline void HR::HotReload::queueCallback(const CallbackParameter & param)
{
    this->queuedCallbacks.push_back(param);
}

inline void HR::HotReload::fetchFileChanges() {
    while (this->fetchNewChanges) {
        this->gFileWatcher->update();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

inline void HR::HotReload::QueueCallback::handleFileAction(FW::WatchID watchID, const std::string & dir, const std::string & filename, FW::Action action)
{
    // this is actually a bug. Sometimes the event is received before the system is
    // finished writing the change and you will get file contention errors. So, wait
    // for the write to finish.
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    CallbackParameter params = { watchID, std::string(dir), std::string(filename), action };
    HotReload::getHotReload()
        ->queueCallback(params);
}
