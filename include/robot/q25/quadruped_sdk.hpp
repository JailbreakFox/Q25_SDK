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
#include "common/types.hpp"            // 类型定义
#include "common/robot.hpp"            // 连接管理

// 运动控制 Motion Control
#include "motion/motion_control.hpp"
#include "motion/motion_state.hpp"

// 导航 Navigation
#include "navigation/point_navigation.hpp"
#include "navigation/track_navigation.hpp"

// 建图与定位 Mapping & Localization
#include "mapping/slam.hpp"
#include "mapping/map_manager.hpp"

// 传感器 Sensors
#include "sensor/imu.hpp"
#include "sensor/lidar.hpp"
#include "sensor/battery.hpp"
#include "sensor/joint.hpp"
#include "sensor/camera.hpp"

// 安全监控 Safety
#include "safety/safety_monitor.hpp"

// 自主充电 Charging
#include "charging/auto_charge.hpp"

// 系统信息 System
#include "system/system_info.hpp"

// 工具 Utilities
#include "utils/error.hpp"

/**
 * SDK版本信息
 */
#define QUADRUPED_SDK_VERSION_MAJOR 1
#define QUADRUPED_SDK_VERSION_MINOR 0
#define QUADRUPED_SDK_VERSION_PATCH 0
#define QUADRUPED_SDK_VERSION "1.0.0"

namespace robot {
namespace q25 {

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

} // namespace q25
} // namespace robot

#endif // QUADRUPED_SDK_HPP
