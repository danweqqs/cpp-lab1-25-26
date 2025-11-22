#include <iostream>
#include <coroutine>
#include <random>
#include <cmath>

class Task3 {
public:
    struct promise_type;
    using handle = std::coroutine_handle<promise_type>;

    struct promise_type {
        int prev_value = 0;
        bool is_first_run = true;

        Task3 get_return_object() {
            return Task3{handle::from_promise(*this)};
        }
        std::suspend_always initial_suspend() {
            return {};
        }
        std::suspend_always final_suspend() noexcept {
            return {};
        }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }

        auto await_transform(int value) {
            return Awaiter{ *this, value };
        }
    };

    struct Awaiter {
        promise_type& p;
        int current_val;

        bool await_ready() const {
            return false;
        }

        void await_suspend(std::coroutine_handle<>) const {}

        bool await_resume() {
            if (p.is_first_run) {
                p.prev_value = current_val;
                p.is_first_run = false;
                return true;
            }
            int diff = std::abs(current_val - p.prev_value);
            p.prev_value = current_val;
            bool should_continue = (diff >= 16);
            if (!should_continue) {
                 std::cout << "Stop: difference " << diff << " < 16";
            }
            return should_continue;
        }
    };

    bool move_next() {
        if (coro && !coro.done()) {
            coro.resume();
            return !coro.done();
        }
        return false;
    }

    ~Task3() { if (coro) coro.destroy(); }

private:
    Task3(handle h) : coro(h) {}
    handle coro;
};

int random_num() {
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_int_distribution<int> dist(1, 256);
    return dist(rng);
}

Task3 task3_logic() {
    int val;
    do {
        val = random_num();
        std::cout << val << "\n";

    } while (co_await val);
}

void demo_task3() {
    std::cout << "Start working" << std::endl;

    auto generator = task3_logic();
    while (generator.move_next()) {}
    std::cout << "\nTask is done!" << std::endl;
}

int main() {
    demo_task3();
    return 0;
}