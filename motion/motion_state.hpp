#ifndef QUADRUPED_SDK_MOTION_STATE_HPP
#define QUADRUPED_SDK_MOTION_STATE_HPP

#include "../core/types.hpp"
#include <string>

namespace quadruped {
namespace sdk {

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
    /**
     * 构造函数
     */
    explicit MotionStateMonitor();

    virtual ~MotionStateMonitor() = default;

    // ============ 状态查询 ============

    /**
     * 获取完整运动状态
     */
    virtual MotionState getMotionState() const = 0;

    /**
     * 获取机器人基本状态
     */
    virtual RobotBasicState getRobotState() const = 0;

    /**
     * 获取当前速度
     */
    virtual Velocity getCurrentVelocity() const = 0;

    /**
     * 获取当前位姿 (里程计)
     */
    virtual Pose getCurrentPose() const = 0;

    /**
     * 检查是否站立状态
     */
    virtual bool isStanding() const = 0;

    /**
     * 检查是否在运动中
     */
    virtual bool isMoving() const = 0;

    /**
     * 检查是否处于急停状态
     */
    virtual bool isEmergencyStopped() const = 0;
};

} // namespace sdk
} // namespace quadruped

#endif // QUADRUPED_SDK_MOTION_STATE_HPP
