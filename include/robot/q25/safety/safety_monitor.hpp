#ifndef QUADRUPED_SDK_SAFETY_MONITOR_HPP
#define QUADRUPED_SDK_SAFETY_MONITOR_HPP

#include "../common/types.hpp"
#include <cstdint>
#include <vector>

namespace robot {
namespace q25 {

/**
 * 过热类型
 */
enum class OverheatType {
    JOINT_MOTOR = 0,    // 关节电机过热
    DRIVER = 1,         // 驱动器过热
    BATTERY = 2         // 电池过热
};

/**
 * 告警状态
 */
enum class WarningState {
    NORMAL = 0,         // 正常
    WARNING = 1,        // 告警中
    CLEARED = 2         // 已清除
};

/**
 * 过热告警信息
 */
struct ThermalWarning {
    OverheatType type;          // 过热类型
    WarningState state;         // 告警状态
    uint32_t joint_id;          // 关节ID (仅关节/驱动器过热时有效)
    float temperature;          // 当前温度 (℃)
};

/**
 * 电池告警类型
 */
enum class BatteryWarningType {
    NONE = 0,           // 无告警
    OVER_TEMP = 1,      // 过温
    LOW_VOLTAGE = 2,    // 低电压
    LOW_SOC = 3,        // 低电量
    CRITICAL_SOC = 4    // 严重低电量
};

/**
 * 电池告警信息
 */
struct BatteryWarning {
    BatteryWarningType type;    // 告警类型
    WarningState state;         // 告警状态
    float value;                // 相关数值 (温度/电压/电量)
};

/**
 * 过载保护状态
 */
struct OverloadStatus {
    bool is_overload;           // 是否过载
    uint32_t joint_faults;      // 关节故障位图 (bit0-11: 各关节)
};

/**
 * 综合安全状态
 */
struct SafetyStatus {
    bool fall_protection;       // 摔倒保护激活
    bool emergency_stop;        // 急停状态
    bool overload_protection;   // 过载保护激活
    bool thermal_warning;       // 存在过热告警
    bool battery_warning;       // 存在电池告警
    uint32_t error_flags;       // 综合错误标志
};

/**
 * SafetyMonitor - 安全监控接口
 * 提供安全状态监控功能
 * 可独立使用，不依赖其他模块
 */
class SafetyMonitor {
public:
    SafetyMonitor();
    ~SafetyMonitor();

    // 禁用复制
    SafetyMonitor(const SafetyMonitor&) = delete;
    SafetyMonitor& operator=(const SafetyMonitor&) = delete;

    // ============ 状态查询 ============

    /**
     * 获取综合安全状态
     * @return 安全状态汇总
     */
    SafetyStatus getSafetyStatus() const;

    /**
     * 检查摔倒保护是否激活
     * @return true表示摔倒保护已激活
     */
    bool isFallProtectionActive() const;

    /**
     * 获取过热告警列表
     * @return 过热告警信息列表
     */
    std::vector<ThermalWarning> getThermalWarnings() const;

    /**
     * 获取电池告警
     * @return 电池告警信息
     */
    BatteryWarning getBatteryWarnings() const;

    /**
     * 获取过载保护状态
     * @return 过载状态信息
     */
    OverloadStatus getOverloadStatus() const;

private:
    struct Impl;
    Impl* impl_;
};

} // namespace q25
} // namespace robot

#endif // QUADRUPED_SDK_SAFETY_MONITOR_HPP

