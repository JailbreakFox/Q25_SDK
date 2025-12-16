#ifndef QUADRUPED_SDK_UTILS_ERROR_HPP
#define QUADRUPED_SDK_UTILS_ERROR_HPP

#include "../core/types.hpp"
#include <string>
#include <exception>

namespace quadruped {
namespace sdk {

/**
 * SDK异常基类
 */
class SDKException : public std::exception {
public:
    explicit SDKException(ErrorCode code, const std::string& message)
        : code_(code), message_(message) {}

    ErrorCode getErrorCode() const { return code_; }
    const char* what() const noexcept override { return message_.c_str(); }

private:
    ErrorCode code_;
    std::string message_;
};

/**
 * 连接异常
 */
class ConnectionException : public SDKException {
public:
    explicit ConnectionException(const std::string& message)
        : SDKException(ErrorCode::UNKNOWN_ERROR, message) {}
};

/**
 * 导航异常
 */
class NavigationException : public SDKException {
public:
    NavigationException(ErrorCode code, const std::string& message)
        : SDKException(code, message) {}
};

/**
 * 超时异常
 */
class TimeoutException : public SDKException {
public:
    explicit TimeoutException(const std::string& message)
        : SDKException(ErrorCode::TIMEOUT, message) {}
};

/**
 * 错误信息
 */
struct ErrorInfo {
    ErrorCode code;
    std::string message;
    double timestamp;
};

/**
 * ErrorHandler - 错误处理接口
 */
class ErrorHandler {
public:
    virtual ~ErrorHandler() = default;

    /**
     * 获取最后一个错误
     * @return 错误信息
     */
    virtual ErrorInfo getLastError() const = 0;

    /**
     * 清除最后一个错误
     */
    virtual void clearLastError() = 0;

    /**
     * 检查是否有错误
     * @return true表示有错误
     */
    virtual bool hasError() const = 0;

    /**
     * 获取错误码的字符串描述
     * @param code 错误码
     * @return 错误描述
     */
    static std::string getErrorString(ErrorCode code);
};

} // namespace sdk
} // namespace quadruped

#endif // QUADRUPED_SDK_UTILS_ERROR_HPP
