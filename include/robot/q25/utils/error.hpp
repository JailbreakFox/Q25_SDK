#ifndef QUADRUPED_SDK_UTILS_ERROR_HPP
#define QUADRUPED_SDK_UTILS_ERROR_HPP

#include "../common/types.hpp"
#include <string>
#include <exception>

namespace robot {
namespace q25 {

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
 * 建图与定位异常
 */
class MappingException : public SDKException {
public:
    MappingException(ErrorCode code, const std::string& message)
        : SDKException(code, message) {}
};

/**
 * 运动异常
 */
class MotionException : public SDKException {
public:
    explicit MotionException(const std::string& message)
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
    ErrorHandler();
    ~ErrorHandler();

    // 禁用复制
    ErrorHandler(const ErrorHandler&) = delete;
    ErrorHandler& operator=(const ErrorHandler&) = delete;

    /**
     * 获取最后一个错误
     * @return 错误信息
     */
    ErrorInfo getLastError() const;

    /**
     * 清除最后一个错误
     */
    void clearLastError();

    /**
     * 检查是否有错误
     * @return true表示有错误
     */
    bool hasError() const;

    /**
     * 获取错误码的字符串描述
     * @param code 错误码
     * @return 错误描述
     */
    static std::string getErrorString(ErrorCode code);

private:
    struct Impl;
    Impl* impl_;
};

} // namespace q25
} // namespace robot

#endif // QUADRUPED_SDK_UTILS_ERROR_HPP
