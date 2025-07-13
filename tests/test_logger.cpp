#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <common/logger.hpp>

#include "common/macros.hpp"

class TestLogger : public ::testing::Test {};

TEST_F(TestLogger, StructHandler) {
    struct TestHandler {
        void operator()(const LogMessage& msg) { UNUSED(msg); }
    };

    LogManager::instance().addHandler(TestHandler{});

    TestHandler testHandler;
    LogManager::instance().addHandler(testHandler);

    LOG(INFO) << "Send a message";
}

TEST_F(TestLogger, LambdaHandler) {
    LogManager::instance().addHandler([](const LogMessage& msg) { UNUSED(msg); });

    auto handler = [](const LogMessage& msg) { UNUSED(msg); };

    LogManager::instance().addHandler(handler);

    LOG(INFO) << "Send a message";
}

namespace {
auto funcHandler(const LogMessage& msg) -> void { UNUSED(msg); }
}    // namespace

TEST_F(TestLogger, FunctionHandler) {
    LogManager::instance().addHandler(funcHandler);

    LOG(INFO) << "Send a message";
}

class MockLogHandler {
public:
    MOCK_METHOD(void, handle, (const LogMessage&), ());
};

TEST_F(TestLogger, SendsLogMessageToHandler) {
    MockLogHandler mockHandler;
    auto           handlerWrapper = [&mockHandler](const LogMessage& msg) -> void {
        mockHandler.handle(msg);
    };

    // Expect `handle` to be called exactly once with *any* LogMessage
    EXPECT_CALL(mockHandler, handle(::testing::_)).Times(1);

    // Register the mock handler
    LogManager::instance().addHandler(handlerWrapper);

    // Log something
    LOG(INFO) << "Test log message";

    // No need to manually flush—Logger's destructor submits automatically
}

TEST_F(TestLogger, LogMessageHasCorrectAttributes) {
    MockLogHandler mockHandler;

    EXPECT_CALL(mockHandler, handle(::testing::Truly([](const LogMessage& msg) {
                    // Check level
                    if (msg.level != LogLevel::WARNING) return false;

                    // Check message text
                    const std::string logText = msg.msg;
                    if (logText.find("Expected message") == std::string::npos) return false;

                    // Check file, line, func are non-empty (basic sanity)
                    if (msg.file.empty() || msg.func.empty() || msg.line <= 0) return false;

                    // Check timestamp is not default-initialized
                    if (msg.timestamp.time_since_epoch().count() == 0) return false;

                    return true;
                })))
        .Times(1);

    // Register
    LogManager::instance().addHandler(
        [&mockHandler](const LogMessage& msg) { mockHandler.handle(msg); });

    LOG(WARNING) << "Expected message";
}

TEST_F(TestLogger, MultipleHandlersAllReceiveMessage) {
    MockLogHandler mockHandler1;
    MockLogHandler mockHandler2;

    EXPECT_CALL(mockHandler1, handle(::testing::_)).Times(1);
    EXPECT_CALL(mockHandler2, handle(::testing::_)).Times(1);

    LogManager::instance().addHandler(
        [&mockHandler1](const LogMessage& msg) { mockHandler1.handle(msg); });
    LogManager::instance().addHandler(
        [&mockHandler2](const LogMessage& msg) { mockHandler2.handle(msg); });

    LOG(ERROR) << "Fan-out to multiple handlers";
}

TEST_F(TestLogger, HandlesConcurrentLoggingWithContention) {
    static constexpr int threadCount   = 8;
    static constexpr int logsPerThread = 100;

    MockLogHandler mockHandler;

    // Total expected calls = threads × logsPerThread
    EXPECT_CALL(mockHandler, handle(::testing::_))
        .Times(threadCount * logsPerThread)
        .WillRepeatedly([](const LogMessage& msg) {
            // Simulate a slow handler (1ms delay)
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            std::cout << "Handled: " << msg.msg << "\n";
        });

    // Register the mock handler (wrapped so no copies)
    LogManager::instance().addHandler(
        [&mockHandler](const LogMessage& msg) { mockHandler.handle(msg); });

    // Launch multiple threads, each sending logs
    std::vector<std::thread> threads;
    threads.reserve(threadCount);
    for (int t = 0; t < threadCount; ++t) {
        threads.emplace_back([t]() {
            for (int i = 0; i < logsPerThread; ++i) {
                LOG(INFO) << "Thread " << t << " log #" << i;
            }
        });
    }

    // Join all threads
    for (auto& thread : threads) { thread.join(); }
}
