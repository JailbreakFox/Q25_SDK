#ifndef QUADRUPED_SDK_NAVIGATION_POINT_NAVIGATION_HPP
#define QUADRUPED_SDK_NAVIGATION_POINT_NAVIGATION_HPP

#include "../common/types.hpp"
#include <vector>
#include <string>

namespace robot {
namespace q25 {

/**
 * PointNavigation - 定点导航接口
 * 提供基于场景和导航点的定点导航功能
 * 可独立使用，不依赖其他模块
 */
class PointNavigation {
public:
    PointNavigation();
    ~PointNavigation();

    // 禁用复制
    PointNavigation(const PointNavigation&) = delete;
    PointNavigation& operator=(const PointNavigation&) = delete;

    // ============ 导航控制 ============

    /**
     * 导航到指定位姿
     * @param pose 目标位姿
     * @return true表示导航任务启动成功
     */
    bool navigateToPose(const std::string& scene,const Pose& pose);

    /**
     * 取消当前导航
     * @return true表示取消成功
     */
    bool cancelNavigation();

    /**
     * 暂停导航
     * @return true表示暂停成功
     */
    bool pauseNavigation();

    /**
     * 恢复导航
     * @return true表示恢复成功
     */
    bool resumeNavigation();

    // ============ 状态查询 ============

    /**
     * 获取导航状态
     * @return 导航状态
     */
    NavigationStatus getNavigationStatus() const;

private:
    struct Impl;
    Impl* impl_;
};

} // namespace q25
} // namespace robot

#endif // QUADRUPED_SDK_NAVIGATION_POINT_NAVIGATION_HPP
