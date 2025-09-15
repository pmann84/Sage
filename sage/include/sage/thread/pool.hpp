#pragma once

#include <functional>
#include <future>
#include <queue>

namespace sage::thread {
    class pool {
        explicit pool(const size_t num_threads = 0) {
            // Check the number of threads we can use
            const unsigned long hardware_threads = std::thread::hardware_concurrency();
            const unsigned int max_threads = num_threads == 0 ? (hardware_threads != 0 ? hardware_threads : 2) : num_threads;
            for (unsigned int i = 0; i < max_threads; ++i)
            {
                m_workers.push_back(std::thread(std::bind(&pool::main_thread_loop, this)));
            }
        }

        ~pool() {
            m_stop = true;
            m_condition.notify_all();
            for (auto& worker : m_workers)
            {
                worker.join();
            }
        }

        // For now we can only add void fns with no args, could expand to arbitrary functions - investigate std::result_of
        std::future<void> add_job(std::function<void()> task) {
            std::lock_guard<std::mutex> lock(m_queue_mutex);
            auto packaged_task = std::make_shared<std::packaged_task<void()>>(std::packaged_task<void()>(task));
            std::future<void> result = packaged_task->get_future();
            m_tasks.emplace([packaged_task]() { (*packaged_task)(); });
            m_condition.notify_all();
            return result;
        }

    private:
        void main_thread_loop() {
            while (true)
            {
                if (m_stop)
                {
                    return;
                }
                std::function<void()> task;
                {
                    // Grab the next job off the queue
                    std::unique_lock<std::mutex> lock(m_queue_mutex);
                    m_condition.wait(lock, [&]() { return m_stop || !m_tasks.empty(); });
                    if (m_stop && m_tasks.empty())
                    {
                        return;
                    }
                    task = std::move(m_tasks.front());
                    m_tasks.pop();
                }
                // Actually run it
                task();
            }
        }

        std::mutex m_queue_mutex;
        bool m_stop;
        std::queue<std::function<void()>> m_tasks;
        std::vector<std::thread> m_workers;
        std::condition_variable m_condition;
    };
};