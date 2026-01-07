#ifndef QUADRUPED_SDK_SENSOR_IMU_HPP
#define QUADRUPED_SDK_SENSOR_IMU_HPP

#include "../common/types.hpp"
#include <string>

namespace robot {
namespace q25 {

/**
 * IMUSensor - IMU传感器接口
 * 提供IMU数据访问功能
 * 可独立使用，不依赖其他模块
 */
class IMUSensor {
public:
    IMUSensor();
    ~IMUSensor();

    // 禁用复制
    IMUSensor(const IMUSensor&) = delete;
    IMUSensor& operator=(const IMUSensor&) = delete;

    // ============ 数据访问 ============

    /**
     * 获取当前IMU数据
     * @return IMU数据
     */
    IMUData getData() const;

    /**
     * 获取欧拉角 (度)
     * @param roll [out] 横滚角
     * @param pitch [out] 俯仰角
     * @param yaw [out] 偏航角
     */
    void getOrientation(float& roll, float& pitch, float& yaw) const;

    /**
     * 获取角速度 (rad/s)
     * @param omega_x [out] X轴角速度
     * @param omega_y [out] Y轴角速度
     * @param omega_z [out] Z轴角速度
     */
    void getAngularVelocity(float& omega_x, float& omega_y, float& omega_z) const;

    /**
     * 获取线加速度 (m/s²)
     * @param acc_x [out] X轴加速度
     * @param acc_y [out] Y轴加速度
     * @param acc_z [out] Z轴加速度
     */
    void getLinearAcceleration(float& acc_x, float& acc_y, float& acc_z) const;

private:
    struct Impl;
    Impl* impl_;
};

} // namespace q25
} // namespace robot

#endif // QUADRUPED_SDK_SENSOR_IMU_HPP
