#ifndef QUADRUPED_SDK_CHARGING_AUTO_CHARGE_HPP
#define QUADRUPED_SDK_CHARGING_AUTO_CHARGE_HPP

#include "../core/types.hpp"
#include <cstdint>
#include <string>

namespace robot {
namespace q25 {

/**
 * 充电状态枚举
 * 对应 CMS::AutoChargeStatus
 */
enum class ChargeStatus {
    IDLE = 0,                       // 空闲
    PATH_PLAN_FAIL = 1,             // 路径规划失败
    GUIDING_TO_POINT = 2,           // 引导至充电点
    GUIDING_TO_POINT_FAIL = 3,      // 引导至充电点失败
    REACHED_POINT = 4,              // 到达充电点
    GUIDING_TO_PILE = 5,            // 引导至充电桩
    GUIDING_TO_PILE_FAIL = 6,       // 引导至充电桩失败
    REACHED_PILE = 7,               // 到达充电桩
    SQUATTING = 8,                  // 下蹲中
    SQUAT_FAIL = 9,                 // 下蹲失败
    CHARGING = 10,                  // 充电中
    CHARGE_FULL = 11,               // 已充满
    EXITING_PILE = 12,              // 退桩中
    EXIT_PILE_SUCCESS = 13,         // 退桩成功
    EXIT_PILE_FAIL = 14,            // 退桩失败
    OVER_TEMP_PROTECTION = 15,      // 充电过温保护
    OVER_VOLTAGE_PROTECTION = 16,   // 过压保护
    OVER_CURRENT_PROTECTION = 17    // 过流保护
};

/**
 * 自主充电任务类型
 */
enum class ChargeTaskType {
    GUIDE_TO_POINT = 0,     // 引导至充电点
    GUIDE_TO_PILE = 1,      // 引导至充电桩
    EXIT_CHARGE = 2         // 退出充电
};

/**
 * 自主充电信息
 */
struct AutoChargeInfo {
    ChargeStatus status;        // 当前充电状态
    bool is_charging;           // 是否正在充电
    uint8_t battery_percent;    // 当前电量百分比
    double timestamp;           // 时间戳
};

/**
 * AutoCharge - 自主充电接口
 * 提供自主充电控制功能
 * 可独立使用，不依赖其他模块
 */
class AutoCharge {
public:
    /**
     * 构造函数
     */
    explicit AutoCharge();

    virtual ~AutoCharge() = default;

    // ============ 充电控制 ============

    /**
     * 开始自主充电
     * @return true表示任务启动成功
     */
    virtual bool startCharge() = 0;

    /**
     * 停止充电/取消充电任务
     * @return true表示停止成功
     */
    virtual bool stopCharge() = 0;

    // ============ 状态查询 ============

    /**
     * 获取充电状态
     * @return 当前充电状态
     */
    virtual ChargeStatus getChargeStatus() const = 0;

    /**
     * 检查是否正在充电
     * @return true表示正在充电中
     */
    virtual bool isCharging() const = 0;
};

} // namespace q25
} // namespace robot

#endif // QUADRUPED_SDK_CHARGING_AUTO_CHARGE_HPP

