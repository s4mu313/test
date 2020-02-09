#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <thread>
#include <mutex>
#include <vector>

class Thread_pool {

private:

    struct guarded_thread : std::thread {
        using std::thread::thread;
        ~guarded_thread()
        { if (joinable()) join(); }
    };

public:

    Thread_pool() = default;
    ~Thread_pool() = default;

    Thread_pool(Thread_pool&&) = delete;
    Thread_pool(const Thread_pool&) = delete;
    Thread_pool& operator=(Thread_pool&&) = delete;
    Thread_pool& operator=(const Thread_pool&) = delete;

    /**
     * There is a list of free positions to check when it needs a new thread.
     *
     * If there aren't free position, this may be for 2 reasons:
     *     * all threads are running and no one has finished yet,
     *     * no threads have been executed so the pool is empty,
     * anyway in this case add the thread in the pool.
     *
     * If there is a free position this means that one thread has finished so
     * overwrite it with a new thread and delete the free pos.
     *
     * When a thread finishes its work it set its pool's position in the vec
     * so that a new thread can take its place.
     *
     * When a thread overwrite another thread, the destructor of the object
     * (guarded_thread) is automatically called, then the thread will be
     * automatically joined.
     */
    template<typename F, typename... Args>
    void
    exec(F&& f, Args&&... args)
    {
        std::unique_lock<std::mutex> lck(mtx_);

        if (pos_.size() == 0) {
            thread_list_.emplace_back(std::make_unique<guarded_thread>([&](std::size_t id, F& f, Args&... args) {
                                          f(args...);
                                          std::unique_lock<std::mutex> lck(mtx_);
                                          pos_.push_back(id);
                                      },  thread_list_.size(), std::ref(f), std::ref(args)...));
            return;
        }

        thread_list_[pos_[0]] = std::make_unique<guarded_thread>([&](std::size_t id, F& f, Args&... args) {
                f(args...);
                std::unique_lock<std::mutex> lck(mtx_);
                pos_.push_back(id);
    }, pos_[0], std::ref(f), std::ref(args)...);
        pos_.erase(pos_.begin());
    }


private:
    std::mutex mtx_;
    std::vector<std::unique_ptr<guarded_thread>> thread_list_;
    std::vector<std::size_t> pos_; /// Free pos in vec.

};

#endif // THREAD_POOL_H
