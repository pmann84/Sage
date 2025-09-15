#include <utility>
#include <optional>
#include <queue>
#include <mutex>
#include <condition_variable>

namespace sage::channel {
    template<typename T> 
    class channel;

    // Return here is producer and consumer respectively
    template<typename T>
    std::pair<channel<T>, channel<T>> make_channel() {
        auto shared_state = std::make_shared<typename channel<T>::state>();
        channel<T> producer{shared_state, true};
        channel<T> consumer{std::move(shared_state), false};
        return {std::move(producer), std::move(consumer)};
    }

    // Single producer-single consumer channel
    template<typename T>
    class channel {
    public:
        channel(channel const&) = delete;
        channel(channel&&) = default;
        channel& operator=(channel&) = delete;
        channel& operator=(channel&&) = default;

        ~channel() {
            close();
        }

        bool send(T item) {
            // If we arent the producer or we have no state we can't send.
            if (!is_producer || !m_shared_state) return false;

            std::lock_guard lock{m_shared_state->lock};
            if (m_shared_state->closed) return false;
            m_shared_state->queue.push(std::move(item));
            m_shared_state->condition.notify_one();
            return true;
        }

        std::optional<T> recv() {
            if (is_producer || !m_shared_state) return std::nullopt;

            std::unique_lock lock{m_shared_state->lock};
            m_shared_state->condition.wait(lock, [this](){
                return !m_shared_state->queue.empty() || m_shared_state->closed;
            });
            if (m_shared_state->queue.empty()) return std::nullopt;
            auto item = std::move(m_shared_state->queue.front());
            m_shared_state->queue.pop();
            return item;
        }

        // Close the channel - returns operation successful or not
        bool close() {
            if (!m_shared_state) return false;

            std::lock_guard lock{m_shared_state->lock};
            if (m_shared_state->closed) return false;

            m_shared_state->closed = true;
            m_shared_state->condition.notify_one();
            return true;
        }

    private:
        struct state {
            std::queue<T> queue;
            std::mutex lock;
            std::condition_variable condition;
            bool closed = false;
        };

        std::shared_ptr<state> m_shared_state;
        bool is_producer = false;

        explicit channel(std::shared_ptr<state> state, bool is_producer) : m_shared_state{std::move(state)}, is_producer{is_producer} {}

        friend std::pair<channel<T>, channel<T>> make_channel<T>();
    };
}