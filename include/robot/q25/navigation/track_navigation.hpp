#ifndef QUADRUPED_SDK_NAVIGATION_TRACK_NAVIGATION_HPP
#define QUADRUPED_SDK_NAVIGATION_TRACK_NAVIGATION_HPP

#include "../core/types.hpp"
#include <vector>
#include <string>

namespace robot {
namespace q25 {

/**
 * TrackNavigation - 循迹导航接口
 * 提供基于路径的循迹导航功能
 * 可独立使用，不依赖其他模块
 */
class TrackNavigation {
public:
    /**
     * 构造函数
     */
    explicit TrackNavigation();

    virtual ~TrackNavigation() = default;

    // ============ 路径管理 ============

    /**
     * 获取所有路径列表
     * @return 导航路径列表
     */
    virtual std::vector<NavigationPath> getPaths() = 0;

    /**
     * 获取指定路径详情
     * @param path_id 路径ID
     * @return 路径信息，失败返回空路径
     */
    virtual NavigationPath getPath(uint32_t path_id) = 0;

    /**
     * 删除路径
     * @param path_id 路径ID
     * @return true表示删除成功
     */
    virtual bool deletePath(uint32_t path_id) = 0;

    /**
     * 重命名路径
     * @param path_id 路径ID
     * @param new_name 新路径名称
     * @return true表示重命名成功
     */
    virtual bool renamePath(uint32_t path_id, const std::string& new_name) = 0;

    /**
     * 设置路径为单向/双向
     * @param path_id 路径ID
     * @param bidirectional true=双向, false=单向
     * @return true表示设置成功
     */
    virtual bool setPathBidirectional(uint32_t path_id, bool bidirectional) = 0;

    // ============ 路径录制 ============

    /**
     * 开始录制路径
     * @param path_name 路径名称
     * @param bidirectional 是否双向路径
     * @return true表示开始录制成功
     */
    virtual bool startRecording(const std::string& path_name, bool bidirectional = true) = 0;

    /**
     * 停止录制并保存路径
     * @return true表示保存成功，返回路径ID
     */
    virtual uint32_t stopRecording() = 0;

    /**
     * 取消录制 (不保存)
     * @return true表示取消成功
     */
    virtual bool cancelRecording() = 0;

    /**
     * 检查是否正在录制
     * @return true表示正在录制
     */
    virtual bool isRecording() const = 0;

    /**
     * 获取当前录制的路径点数量
     * @return 路径点数量
     */
    virtual uint32_t getRecordedWayPointCount() const = 0;

    // ============ 循迹导航 ============

    /**
     * 开始循迹导航
     * @param path_id 路径ID
     * @param forward 是否正向行走 (true=正向, false=反向)
     * @param loop 是否循环导航
     * @return true表示导航任务启动成功
     */
    virtual bool startTrackFollowing(uint32_t path_id, bool forward = true, bool loop = false) = 0;

    /**
     * 停止循迹导航
     * @return true表示停止成功
     */
    virtual bool stopTrackFollowing() = 0;

    /**
     * 暂停循迹导航
     * @return true表示暂停成功
     */
    virtual bool pauseTrackFollowing() = 0;

    /**
     * 恢复循迹导航
     * @return true表示恢复成功
     */
    virtual bool resumeTrackFollowing() = 0;

    // ============ 状态查询 ============

    /**
     * 获取导航状态
     * @return 导航状态
     */
    virtual NavigationStatus getNavigationStatus() const = 0;

    /**
     * 获取当前正在执行的路径ID
     * @return 路径ID，无路径时返回0
     */
    virtual uint32_t getCurrentPathId() const = 0;

    /**
     * 获取当前路径进度
     * @param current_waypoint [out] 当前路径点索引
     * @param total_waypoints [out] 总路径点数量
     * @return true表示获取成功
     */
    virtual bool getProgress(uint32_t& current_waypoint, uint32_t& total_waypoints) const = 0;

    /**
     * 检查是否为循环模式
     * @return true表示循环模式
     */
    virtual bool isLoopMode() const = 0;

    /**
     * 检查是否为正向行走
     * @return true表示正向，false表示反向
     */
    virtual bool isForwardDirection() const = 0;
};

} // namespace q25
} // namespace robot

#endif // QUADRUPED_SDK_NAVIGATION_TRACK_NAVIGATION_HPP
