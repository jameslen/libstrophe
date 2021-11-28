export module strophe.context;

import <chrono>;
import <memory>;
import <memory_resource>;
import <string>;

namespace xmpp
{
    export enum class log_level
    { 
        debug,
        info,
        warn,
        error
    };

    enum class loop_status
    {
        not_started,
        running,
        quit
    };

    export using log_handler = void (*)(
        void *userdata,
        log_level level,
        std::string_view area,
        std::string_view msg
    );

    export struct logger
    {
      log_handler handler;
      void *userdata;
    };

    class connection;

    export class context
    {
    public:
        context(
            logger* logger,
            std::pmr::memory_resource* allocator
        ) noexcept;

        context(const context&) = delete;
        context(context&&) = delete;

        context &operator=(const context&) = delete;
        context &operator-(context&&) = delete;

        ~context() = default;

        void set_verbosity(log_level level) noexcept;

        std::shared_ptr<connection> create_connection();

        void run_once(std::chrono::milliseconds timeout);
        void run();
        void stop();

        void set_timeout(std::chrono::milliseconds timeout);
    private:
        struct handlist_t {
          /* common members */
          int user_handler;
          int (*handler)();
          void *userdata;
          int enabled; /* handlers are added disabled and enabled after the
                        * handler chain is processed to prevent stanzas from
                        * getting processed by newly added handlers */
          handlist_t *next;

          union {
            /* timed handlers */
            struct {
              unsigned long period;
              uint64_t last_stamp;
            };
            /* id handlers */
            struct {
              char *id;
            };
            /* normal handlers */
            struct {
              char *ns;
              char *name;
              char *type;
            };
          } u;
        };

        struct connlist_t {
          connection *conn;
          struct connlist_t *next;
        };
    private:
        static constexpr size_t c_default_loop_timeout = 1000;

        std::pmr::memory_resource *mem{};
        const logger *log{};
        int verbosity;

        struct xmpp_rand_t *rand{};
        loop_status loop_status = loop_status::not_started;
        connlist_t *connlist{};
        handlist_t *timed_handlers{};

        unsigned long timeout = c_default_loop_timeout;

        friend class connection;
    };
}