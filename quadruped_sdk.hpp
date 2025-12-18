#ifndef QUADRUPED_SDK_HPP
#define QUADRUPED_SDK_HPP

/**
 * 四足机器人SDK - 主头文件
 * Quadruped Robot SDK - Main Header
 *
 * 使用方法 / Usage:
 *   #include <quadruped_sdk.hpp>
 *
 * 版本 / Version: 1.0.0
 */

// 核心模块 Core
#include "core/types.hpp"            // 类型定义
#include "core/robot.hpp"            // 连接管理

// 运动控制 Motion Control
#include "motion/motion_control.hpp"
#include "motion/motion_state.hpp"

// 建图与定位 Mapping & Localization
#include "mapping/slam.hpp"
#include "mapping/map_manager.hpp"

// 工具 Utilities
#include "utils/error.hpp"

/**
 * SDK版本信息
 */
#define QUADRUPED_SDK_VERSION_MAJOR 1
#define QUADRUPED_SDK_VERSION_MINOR 0
#define QUADRUPED_SDK_VERSION_PATCH 0
#define QUADRUPED_SDK_VERSION "1.0.0"

namespace quadruped {
namespace sdk {

/**
 * 获取SDK版本字符串
 */
inline const char* getSDKVersion() {
    return QUADRUPED_SDK_VERSION;
}

/**
 * 获取SDK版本号
 */
inline void getSDKVersionNumber(int& major, int& minor, int& patch) {
    major = QUADRUPED_SDK_VERSION_MAJOR;
    minor = QUADRUPED_SDK_VERSION_MINOR;
    patch = QUADRUPED_SDK_VERSION_PATCH;
}

} // namespace sdk
} // namespace quadruped

#endif // QUADRUPED_SDK_HPP
