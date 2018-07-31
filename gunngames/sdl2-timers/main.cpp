#include <SDL.h>
#include <vector>
#include <cstdint>
#include <iostream>
#include <functional>
#include <unordered_map>

struct block_t {
    int32_t x, y;
    int32_t w, h;
    SDL_Color color;
    int32_t dx = 4, dy = -4;
};

class timer_pool;
struct timer_t;

using timer_expired_callable = std::function<bool (timer_pool*, timer_t*)>;

struct timer_t {
    uint16_t id;
    bool enabled;
    uint32_t last_time;
    uint32_t expiry_time;
    uint32_t duration;
    int32_t data1;
    int32_t data2;
    timer_expired_callable callback {};
};

using timer_map_t = std::unordered_map<uint16_t, timer_t*>;
using timer_list_t = std::vector<timer_t*>;

class timer_pool {
public:
    timer_pool() = default;

    ~timer_pool() {
        for (auto timer : _timers)
            delete timer;
        _id_to_timer.clear();
    }

    void update() {
        for (auto timer : _to_remove)
            remove_timer(timer);
        _to_remove.clear();

        auto now = SDL_GetTicks();
        for (auto timer : _timers) {
            if (!timer->enabled)
                continue;

            if (timer->expiry_time < now) {
                if (!timer->callback(this, timer))
                    timer->enabled = false;
                else {
                    timer->last_time = timer->expiry_time;
                    timer->expiry_time = SDL_GetTicks() + timer->duration;
                }
            }
        }
    }

    timer_t* start_timer(
            uint32_t duration,
            const timer_expired_callable& callable) {
        auto timer = new timer_t;
        timer->id = s_id++;
        timer->enabled = true;
        timer->callback = callable;
        timer->duration = duration;
        timer->last_time = SDL_GetTicks();
        timer->expiry_time = timer->last_time + timer->duration;
        _timers.emplace_back(timer);
        _id_to_timer.insert(std::make_pair(timer->id, timer));
        return timer;
    }

    timer_t* find_by_id(uint16_t id) {
        const auto it = _id_to_timer.find(id);
        if (it == _id_to_timer.end())
            return nullptr;
        return it->second;
    }

    const timer_list_t& timers() const {
        return _timers;
    }

    bool remove_timer(timer_t* timer) {
        auto it = _timers.erase(std::find_if(
            _timers.begin(),
            _timers.end(),
            [timer](timer_t* each) {
                return each == timer;
            }));
        _id_to_timer.erase(timer->id);
        delete timer;
        return it != _timers.end();
    }

    void mark_for_remove(timer_t* timer) {
        _to_remove.emplace_back(timer);
    }

private:
    static inline uint16_t s_id = 1;

    timer_list_t _timers {};
    timer_list_t _to_remove {};
    timer_map_t _id_to_timer {};
};

static uint32_t sdl_timer_callback(uint32_t interval, void* param) {
    SDL_Event event;
    SDL_UserEvent user_event;

    user_event.type = SDL_USEREVENT;
    user_event.code = 0;
    user_event.data1 = param;
    user_event.data2 = nullptr;

    event.type = SDL_USEREVENT;
    event.user = user_event;
    SDL_PushEvent(&event);

    return interval;
}

int main(int argc, char** argv) {
    timer_pool pool;
    block_t first {
        .x = 100,
        .y = 100,
        .w = 32,
        .h = 32,
        .color = {
            .a = 0xff,
            .r = 0x9a,
            .g = 0x38,
            .b = 0xfe
        }
    };

    timer_t* color_magic_timer;
    color_magic_timer = pool.start_timer(
        125,
        [&](timer_pool* timer_pool1, timer_t* timer) {
            if (timer->data1 == 0)
                return true;

            if (timer->data1 == 1) {
                timer->data1 = 0;

                auto reset_and_bounce_timer = timer_pool1->start_timer(
                    1000,
                    [&](timer_pool* timer_pool2, timer_t* reset_timer) {
                        color_magic_timer->data1 = 32;
                        first.color.r = 0x9a;
                        first.color.g = 0x38;
                        first.color.b = 0xfe;
                        first.dx = -first.dx;
                        first.dy = -first.dy;
                        SDL_RemoveTimer(reset_timer->data2);
                        timer_pool2->mark_for_remove(reset_timer);
                        return false;
                    });
                reset_and_bounce_timer->enabled = false;
                reset_and_bounce_timer->data2 = SDL_AddTimer(
                    reset_and_bounce_timer->duration,
                    sdl_timer_callback,
                    reset_and_bounce_timer);
            } else {
                first.color.r += 2;
                if (first.color.r > 250)
                    first.color.r = 5;

                first.color.g += 3;
                if (first.color.g > 250)
                    first.color.g = 5;

                if (first.color.b > 10)
                    first.color.b -= 4;
                else
                    first.color.b = 0xff;

                timer->data1--;
            }
            return true;
        });
    color_magic_timer->enabled = false;
    color_magic_timer->data1 = 32;
    SDL_AddTimer(
        color_magic_timer->duration,
        sdl_timer_callback,
        color_magic_timer);

    auto window = SDL_CreateWindow(
        "Timer Test",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1024,
        768,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    auto renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    auto running = true;
    while (running) {
        pool.update();

        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
                case SDL_QUIT: {
                    running = false;
                    break;
                }
                case SDL_KEYDOWN: {
                    switch (e.key.keysym.sym) {
                        case SDLK_ESCAPE: {
                            running = false;
                        }
                        default: {
                            break;
                        }
                    }
                    break;
                }
                case SDL_USEREVENT: {
                    auto timer = reinterpret_cast<timer_t*>(e.user.data1);
                    if (timer != nullptr)
                        timer->callback(&pool, timer);
                    break;
                }
                default:
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 64, 41, 89, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(
            renderer,
            first.color.r,
            first.color.g,
            first.color.b,
            first.color.a);
        SDL_Rect block_rect {
            .x = first.x,
            .y = first.y,
            .w = first.w,
            .h = first.h
        };
        SDL_RenderFillRect(renderer, &block_rect);

        first.x += first.dx;
        first.y += first.dy;

        if (first.x < 32 || first.x > 992)
            first.dx = -first.dx;
        if (first.y < 32 || first.y > 736)
            first.dy = -first.dy;

        SDL_RenderPresent(renderer);
    }

    return 0;
}