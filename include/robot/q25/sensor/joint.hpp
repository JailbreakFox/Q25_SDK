#ifndef QUADRUPED_SDK_SENSOR_JOINT_HPP
#define QUADRUPED_SDK_SENSOR_JOINT_HPP

#include "../core/types.hpp"
#include <array>
#include <cstdint>

namespace robot {
namespace q25 {

/**
 * 关节数量定义
 */
constexpr uint32_t JOINT_COUNT = 12;

/**
 * 单关节数据
 */
struct JointData {
    float position;         // 关节位置 (rad)
    float velocity;         // 关节速度 (rad/s)
    float torque;           // 关节力矩 (Nm)
    float motor_temp;       // 电机温度 (℃)
    float driver_temp;      // 驱动器温度 (℃)
    int32_t error_code;     // 错误码
};

/**
 * 所有关节数据
 */
struct AllJointsData {
    double timestamp;                           // 时间戳
    std::array<JointData, JOINT_COUNT> joints;  // 12个关节数据
};

/**
 * JointSensor - 关节传感器接口
 * 提供关节数据访问功能
 * 可独立使用，不依赖其他模块
 */
class JointSensor {
public:
    /**
     * 构造函数
     */
    explicit JointSensor();

    virtual ~JointSensor() = default;

    // ============ 数据访问 ============

    /**
     * 获取所有关节数据
     * @return 12个关节的完整数据
     */
    virtual AllJointsData getAllJointsData() const = 0;

    /**
     * 获取驱动器温度数组
     * @return 12个驱动器的温度 (℃)
     */
    virtual std::array<float, JOINT_COUNT> getDriverTemperatures() const = 0;

    /**
     * 获取电机温度数组
     * @return 12个电机的温度 (℃)
     */
    virtual std::array<float, JOINT_COUNT> getMotorTemperatures() const = 0;
};

} // namespace q25
} // namespace robot

#endif // QUADRUPED_SDK_SENSOR_JOINT_HPP

