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
    std::string &dir;
    std::string &filename;
    FW::Action action;
};

class HotReload
{
  public:
      // Singleton getter
      static std::shared_ptr<HotReload> getHotReload();

      // Register a new watch by specifing the path to the file and a callback which 
      // should get called at a latter point.
      void registerCallback(
          std::string path, 
          std::function<void(const std::string &, const std::string &, FW::Action)>);

      // Removes a watch by specifing the path to the file the watch was listening to.
      void removeCallback(
          std::string path);

      // Calls the callbacks when they were queued by the filewatcher.
      void update();

      ~HotReload();
  
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
            FW::Action action) override;
    };

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
    // singleton pattern
    HotReload();
    struct MakeSharedEnabler;
    static std::shared_ptr<HotReload> instance;

    // thread for async checks for file changes
    std::thread *updateFileChanges;
    // thread stopper
    bool fetchNewChanges = true;
    // function for thread
    // loops, until fetchNewChanges gets set to false
    void fetchFileChanges();

    // queue file change callbacks so the main thread can execute them
    friend class QueueCallback;
    void queueCallback(const CallbackParameter &param);
};

struct HR::HotReload::MakeSharedEnabler : public HR::HotReload {
    MakeSharedEnabler() : HR::HotReload() {
    }
};
} // namespace HotReload
