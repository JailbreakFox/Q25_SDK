#ifndef QUADRUPED_SDK_NAVIGATION_POINT_NAVIGATION_HPP
#define QUADRUPED_SDK_NAVIGATION_POINT_NAVIGATION_HPP

#include "../core/types.hpp"
#include <vector>
#include <string>

namespace quadruped {
namespace sdk {

/**
 * PointNavigation - 定点导航接口
 * 提供基于场景和导航点的定点导航功能
 * 可独立使用，不依赖其他模块
 */
class PointNavigation {
public:
    /**
     * 构造函数
     */
    explicit PointNavigation();

    virtual ~PointNavigation() = default;

    // ============ 场景管理 ============

    /**
     * 获取所有场景列表
     * @return 场景信息列表
     */
    virtual std::vector<SceneInfo> getScenes() = 0;

    /**
     * 加载场景
     * @param scene_id 场景ID
     * @return true表示加载成功
     */
    virtual bool loadScene(uint32_t scene_id) = 0;

    /**
     * 卸载当前场景
     * @return true表示卸载成功
     */
    virtual bool unloadScene() = 0;

    /**
     * 获取当前加载的场景ID
     * @return 场景ID，如果未加载场景则返回0
     */
    virtual uint32_t getCurrentSceneId() const = 0;

    /**
     * 删除场景
     * @param scene_id 场景ID
     * @return true表示删除成功
     */
    virtual bool deleteScene(uint32_t scene_id) = 0;

    // ============ 导航点管理 ============

    /**
     * 获取当前场景的所有导航点
     * @return 导航点列表
     */
    virtual std::vector<WayPoint> getWayPoints() = 0;

    /**
     * 添加导航点 (在当前位置)
     * @param point_name 导航点名称
     * @return 导航点ID，失败返回0
     */
    virtual uint32_t addWayPoint(const std::string& point_name) = 0;

    /**
     * 删除导航点
     * @param point_id 导航点ID
     * @return true表示删除成功
     */
    virtual bool deleteWayPoint(uint32_t point_id) = 0;

    /**
     * 更新导航点位姿
     * @param point_id 导航点ID
     * @param pose 新的位姿
     * @return true表示更新成功
     */
    virtual bool updateWayPoint(uint32_t point_id, const Pose& pose) = 0;

    // ============ 导航控制 ============

    /**
     * 导航到指定导航点
     * @param point_id 导航点ID
     * @return true表示导航任务启动成功
     */
    virtual bool navigateToPoint(uint32_t point_id) = 0;

    /**
     * 导航到指定位姿
     * @param pose 目标位姿
     * @return true表示导航任务启动成功
     */
    virtual bool navigateToPose(const Pose& pose) = 0;

    /**
     * 取消当前导航
     * @return true表示取消成功
     */
    virtual bool cancelNavigation() = 0;

    /**
     * 暂停导航
     * @return true表示暂停成功
     */
    virtual bool pauseNavigation() = 0;

    /**
     * 恢复导航
     * @return true表示恢复成功
     */
    virtual bool resumeNavigation() = 0;

    // ============ 状态查询 ============

    /**
     * 获取导航状态
     * @return 导航状态
     */
    virtual NavigationStatus getNavigationStatus() const = 0;

    /**
     * 获取当前目标点ID
     * @return 目标点ID，无目标时返回0
     */
    virtual uint32_t getCurrentTargetId() const = 0;

    /**
     * 获取到目标点的距离
     * @return 距离 (米)，无目标时返回-1
     */
    virtual float getDistanceToTarget() const = 0;
};

} // namespace sdk
} // namespace quadruped

#endif // QUADRUPED_SDK_NAVIGATION_POINT_NAVIGATION_HPP
