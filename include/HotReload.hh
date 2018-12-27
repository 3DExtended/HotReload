#include <map>
#include <memory>
#include <functional>
#include <chrono>
#include <thread>
#include <string>
#include <vector>
#include <FileWatcher/FileWatcher.h>

namespace HR
{
struct CallbackParameter
{
    FW::WatchID watchID;
    std::string dir;
    std::string filename;
    FW::Action action;
};

class HotReload
{
  public:
      static HotReload* instance() {
          static HotReload* instance = new HotReload();
          return instance;
      }

      // Register a new watch by specifing the path to the file and a callback which 
      // should get called at a latter point.
      void registerCallback(
          std::string path, 
          std::function<void(const std::string &, const std::string &, FW::Action)> callback)
      {
          // add a watch to the system
          auto gWatchID = gFileWatcher->addWatch(path,this->queueCallbackListener,true);
          this->watches[path] = gWatchID;
          this->callbacks[gWatchID] = callback;
      }

      // Removes a watch by specifing the path to the file the watch was listening to.
      void removeCallback(
          std::string path)
      {
          this->watches.erase(path);
          this->gFileWatcher->removeWatch(path);
      }

      // Calls the callbacks when they were queued by the filewatcher.
      void update()
      {
          this->fetchFileChanges();
          for each (auto callbackParams in queuedCallbacks)
          {
              callbacks[callbackParams.watchID](
                  callbackParams.dir,
                  callbackParams.filename,
                  callbackParams.action);
          }
          queuedCallbacks.clear();
      }

      ~HotReload() {
          fetchNewChanges = false;
      }
  
  protected:
    // Class needed due to FileWatch implementation 
    class QueueCallback : public FW::FileWatchListener
    {
      public:
        QueueCallback() {}
        void handleFileAction(
            FW::WatchID watchID, 
            const std::string &dir, 
            const std::string &filename, 
            FW::Action action) override
        {
            // this is actually a bug. Sometimes the event is received before the system is
            // finished writing the change and you will get file contention errors. So, wait
            // for the write to finish.
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            CallbackParameter params = { watchID, dir, filename, action };
            HotReload::instance()->queueCallback(params);
        }
    };

    QueueCallback* queueCallbackListener = new QueueCallback();

    // The fileWatcher element is checking for file updates. gFileWatcher->update() 
    // gets called by the updateFileChanges thread.
    std::shared_ptr<FW::FileWatcher> gFileWatcher = std::make_shared<FW::FileWatcher>();

    // List of active watches
    std::map<std::string, FW::WatchID> watches;

    // List of active callbacks
    std::map<FW::WatchID, std::function<void(const std::string &, const std::string &, FW::Action)>> callbacks;

    // List of queued callbacks
    std::vector<CallbackParameter> queuedCallbacks;

  private:
    // thread for async checks for file changes
    std::thread *updateFileChanges;
    // thread stopper
    bool fetchNewChanges = true;
    // function for thread
    // loops, until fetchNewChanges gets set to false
    void fetchFileChanges() {
        //while (this->fetchNewChanges) {
            this->gFileWatcher->update();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        //}
    }

    // queue file change callbacks so the main thread can execute them
    friend class QueueCallback;
    void queueCallback(const CallbackParameter &param)
    {
        this->queuedCallbacks.push_back(param);
    }

    // singleton pattern
    HotReload() {
        //updateFileChanges = new std::thread(&HR::HotReload::fetchFileChanges, this);
    }
};
} // namespace HotReload
