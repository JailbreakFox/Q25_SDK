#ifndef QUADRUPED_SDK_SENSOR_BATTERY_HPP
#define QUADRUPED_SDK_SENSOR_BATTERY_HPP

#include "../common/types.hpp"
#include <string>

namespace robot {
namespace q25 {

/**
 * 详细电池信息
 */
struct BatteryInfo {
    uint8_t percentage;        // 电量百分比 (0-100%)
    bool is_charging;          // 是否正在充电
    float voltage;             // 电压 (V)
    float current;             // 电流 (A)
    float temperature;         // 温度 (℃)
    uint32_t remaining_time;   // 剩余使用时间 (秒)，充电时为充满时间
    uint32_t cycle_count;      // 充放电循环次数
};

/**
 * BatterySensor - 电池传感器接口
 * 提供电池状态信息访问功能
 * 可独立使用，不依赖其他模块
 */
class BatterySensor {
public:
    BatterySensor();
    ~BatterySensor();

    // 禁用复制
    BatterySensor(const BatterySensor&) = delete;
    BatterySensor& operator=(const BatterySensor&) = delete;

    // ============ 数据访问 ============

    /**
     * 获取电池状态
     * @return 电池状态
     */
    BatteryState getBatteryState() const;

    /**
     * 获取详细电池信息
     * @return 电池信息
     */
    BatteryInfo getBatteryInfo() const;

    /**
     * 获取电量百分比
     * @return 电量 (0-100%)
     */
    uint8_t getBatteryPercentage() const;

    /**
     * 检查是否正在充电
     * @return true表示正在充电
     */
    bool isCharging() const;

    /**
     * 检查电量是否低 (<20%)
     * @return true表示电量低
     */
    bool isBatteryLow() const;

private:
    struct Impl;
    Impl* impl_;
};

} // namespace q25
} // namespace robot

#endif // QUADRUPED_SDK_SENSOR_BATTERY_HPP
