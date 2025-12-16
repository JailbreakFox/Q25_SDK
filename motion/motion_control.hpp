#ifndef QUADRUPED_SDK_MOTION_CONTROL_HPP
#define QUADRUPED_SDK_MOTION_CONTROL_HPP

#include "../core/types.hpp"

namespace quadruped {
namespace sdk {

/**
 * MotionController - 运动控制接口
 * 提供精细的运动控制功能
 * 可独立使用，不依赖其他模块
 */
class MotionController {
public:
    /**
     * 构造函数
     */
    explicit MotionController();

    virtual ~MotionController() = default;

    // ============ 基础控制 ============

    /**
     * 站立
     */
    virtual bool stand() = 0;

    /**
     * 趴下
     */
    virtual bool lie() = 0;

    /**
     * 切换站立/趴下
     */
    virtual bool toggleStand() = 0;

    /**
     * 急停
     */
    virtual bool emergencyStop() = 0;

    /**
     * 释放急停 (恢复控制)
     */
    virtual bool releaseEmergencyStop() = 0;

    // ============ 运动模式 ============

    /**
     * 设置运动模式
     * @param mode 运动模式 (MANUAL/NAVIGATION/ASSISTED)
     */
    virtual bool setMotionMode(MotionMode mode) = 0;

    /**
     * 获取当前运动模式
     */
    virtual MotionMode getMotionMode() const = 0;

    // ============ 速度控制 (手动模式) ============

    /**
     * 发送速度控制指令
     * @param velocity 速度指令
     */
    virtual bool setVelocity(const Velocity& velocity) = 0;

    /**
     * 停止运动 (速度设为0)
     */
    virtual bool stop() = 0;

    // ============ 步态控制 ============

    /**
     * 设置步态类型
     * @param gait 步态类型 (WALK/RUN)
     */
    virtual bool setGait(GaitType gait) = 0;

    /**
     * 获取当前步态类型
     */
    virtual GaitType getGait() const = 0;

    /**
     * 设置速度档位
     * @param level 速度档位 (LOW/HIGH)
     */
    virtual bool setSpeedLevel(SpeedLevel level) = 0;

    /**
     * 获取当前速度档位
     */
    virtual SpeedLevel getSpeedLevel() const = 0;

    // ============ 机身姿态控制 ============

    /**
     * 设置机身高度
     * @param height 高度 (米)
     */
    virtual bool setBodyHeight(float height) = 0;

    /**
     * 设置机身姿态 (roll, pitch, yaw)
     * @param roll 横滚角 (度)
     * @param pitch 俯仰角 (度)
     * @param yaw 偏航角 (度)
     */
    virtual bool setBodyPose(float roll, float pitch, float yaw) = 0;

    /**
     * 重置机身姿态 (归零)
     */
    virtual bool resetBodyPose() = 0;

    // ============ 高级控制 ============

    /**
     * 设置最大线速度限制
     * @param max_linear_velocity 最大线速度 (m/s)
     */
    virtual bool setMaxLinearVelocity(float max_linear_velocity) = 0;

    /**
     * 设置最大角速度限制
     * @param max_angular_velocity 最大角速度 (rad/s)
     */
    virtual bool setMaxAngularVelocity(float max_angular_velocity) = 0;
};

} // namespace sdk
} // namespace quadruped

#endif // QUADRUPED_SDK_MOTION_CONTROL_HPP
