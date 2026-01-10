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

    // ============ 场景管理 ============

    /**
     * 获取所有场景列表
     * @return 场景信息列表
     */
    std::vector<SceneInfo> getScenes();

    /**
     * 加载场景
     * @param scene_id 场景ID
     * @return true表示加载成功
     */
    bool loadScene(uint32_t scene_id);

    /**
     * 卸载当前场景
     * @return true表示卸载成功
     */
    bool unloadScene();

    /**
     * 获取当前加载的场景ID
     * @return 场景ID，如果未加载场景则返回0
     */
    uint32_t getCurrentSceneId() const;

    /**
     * 删除场景
     * @param scene_id 场景ID
     * @return true表示删除成功
     */
    bool deleteScene(uint32_t scene_id);

    // ============ 导航点管理 ============

    /**
     * 更新导航点位姿
     * @param point_id 导航点ID
     * @param pose 新的位姿
     * @return true表示更新成功
     */
    bool updateWayPoint(uint32_t point_id, const Pose& pose);

    // ============ 导航控制 ============

    /**
     * 导航到指定导航点
     * @param point_id 导航点ID
     * @return true表示导航任务启动成功
     */
    bool navigateToPoint(uint32_t point_id);

    /**
     * 导航到指定位姿
     * @param pose 目标位姿
     * @return true表示导航任务启动成功
     */
    bool navigateToPose(const Pose& pose);

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

    /**
     * 获取当前目标点ID
     * @return 目标点ID，无目标时返回0
     */
    uint32_t getCurrentTargetId() const;

    /**
     * 获取到目标点的距离
     * @return 距离 (米)，无目标时返回-1
     */
    float getDistanceToTarget() const;

private:
    struct Impl;
    Impl* impl_;
};

} // namespace q25
} // namespace robot

#endif // QUADRUPED_SDK_NAVIGATION_POINT_NAVIGATION_HPP
