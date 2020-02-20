//
// Created by mauro on 4/17/19.
//

#include <spdlog/sinks/dist_sink.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/ansicolor_sink.h>

#include "logger.h"

namespace {
    std::once_flag g_loggingOnce;
    Logging *g_instance;
}

struct Logging::Impl {
    Impl() {
        restoreDefaultSink();
    }

    ::spdlog::logger &getLogger() {
        return *::spdlog::get(LOGGER_NAME);
    }

    static const char *makeLoc(const char *file, int line, const char *func) {
        static thread_local char buf[128];
        //sprintf(buf, "(%s:%d %s)", basename(file), line, func); // need to fix, macos problem
        sprintf(buf, "(%s:%d %s)", "/tmp/xx", line, func);
        return buf;
    }

    void setLevel(::spdlog::level::level_enum newLevel) {
        getLogger().set_level(newLevel);
    }

    void clearPattern() {
        getLogger().set_pattern("%v");
    }

    void replaceSink(::spdlog::sink_ptr p) {
        ::spdlog::drop(LOGGER_NAME);

        auto l = std::make_shared<::spdlog::logger>(LOGGER_NAME, p);

        register_logger(l);

        // set global logging level to debug
        ::spdlog::set_level(::spdlog::level::debug);
    }

    void restoreDefaultSink() {

        auto a = std::make_shared<::spdlog::sinks::ansicolor_stdout_sink_st>();
        a->set_color(spdlog::level::info, a->reset);
        replaceSink(a);
    }

};

Logging &Logging::instance() {
    call_once(g_loggingOnce, []() { g_instance = new Logging(); });
    return *g_instance;
}

Logging::Logging() {
    pImpl_ = new Impl();
}

Logging::~Logging() {
    delete pImpl_;
}

::spdlog::logger &
Logging::getLogger() {
    return pImpl_->getLogger();
}

const char *Logging::makeLoc(const char *file, int line, const char *func) {
    return Impl::makeLoc(file, line, func);
}

void Logging::setLevelInfo() {
    pImpl_->setLevel(spdlog::level::info);
}

void Logging::setLevelWarn() {
    pImpl_->setLevel(spdlog::level::warn);
}

void Logging::setLevelError() {
    pImpl_->setLevel(spdlog::level::err);
}

bool Logging::setLevel(std::string_view lvl) {
    int idx = 0;

    while (idx <= spdlog::level::off) {
        if (spdlog::level::level_string_views[idx] == lvl) {
            break;
        }
        ++idx;
    }

    if (idx <= spdlog::level::off) {
        pImpl_->setLevel(static_cast<spdlog::level::level_enum>(idx));

        return true;
    }

    return false;
}

void Logging::clearPattern() {
    pImpl_->clearPattern();
}

void Logging::replaceSink(::spdlog::sink_ptr p) {
    pImpl_->replaceSink(p);
}

void Logging::restoreDefaultSink() {
    pImpl_->restoreDefaultSink();
}

LineLogger LineLogger::log(LineLogger::LEVEL levelx) {
    return LineLogger(static_cast<::spdlog::level::level_enum>(levelx));
}

void LineLogger::log(LineLogger::LEVEL level, std::string_view msg) {
    Logging::instance().getLogger().log(static_cast<::spdlog::level::level_enum>(level), std::string(msg));
}

LineLogger::LineLogger(::spdlog::level::level_enum level)
        : level(level),
          logger(Logging::instance().getLogger()) {}

LineLogger::LineLogger(LineLogger &&rhs)
        : level(rhs.level),
          logger(rhs.logger) {
}

LineLogger::~LineLogger() {}