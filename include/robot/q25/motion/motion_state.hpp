#ifndef QUADRUPED_SDK_MOTION_STATE_HPP
#define QUADRUPED_SDK_MOTION_STATE_HPP

#include "../common/types.hpp"
#include <string>

namespace robot {
namespace q25 {

/**
 * 运动状态信息
 */
struct MotionState {
    RobotBasicState basic_state;   // 基本状态
    MotionMode motion_mode;        // 运动模式
    GaitType gait;                 // 步态
    SpeedLevel speed_level;        // 速度档位
    Velocity velocity;             // 当前速度
    Pose pose;                     // 当前位姿 (里程计)
    float body_height;             // 机身高度
    float body_roll;               // 机身横滚角
    float body_pitch;              // 机身俯仰角
    double timestamp;              // 时间戳
};

/**
 * MotionStateMonitor - 运动状态监控接口
 * 用于查询机器人运动状态
 * 可独立使用，不依赖其他模块
 */
class MotionStateMonitor {
public:
    MotionStateMonitor();
    ~MotionStateMonitor();

    // 禁用复制
    MotionStateMonitor(const MotionStateMonitor&) = delete;
    MotionStateMonitor& operator=(const MotionStateMonitor&) = delete;

    // ============ 状态查询 ============

    /**
     * 获取完整运动状态
     */
    MotionState getMotionState() const;

    /**
     * 获取机器人基本状态
     */
    RobotBasicState getRobotState() const;

    /**
     * 获取当前速度
     */
    Velocity getCurrentVelocity() const;

    /**
     * 获取当前位姿 (里程计)
     */
    Pose getCurrentPose() const;

    /**
     * 检查是否站立状态
     */
    bool isStanding() const;

    /**
     * 检查是否在运动中
     */
    bool isMoving() const;

    /**
     * 检查是否处于急停状态
     */
    bool isEmergencyStopped() const;

private:
    struct Impl;
    Impl* impl_;
};

} // namespace q25
} // namespace robot

#endif // QUADRUPED_SDK_MOTION_STATE_HPP
