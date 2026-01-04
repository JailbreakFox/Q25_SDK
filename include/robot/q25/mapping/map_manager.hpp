#ifndef QUADRUPED_SDK_MAPPING_MAP_MANAGER_HPP
#define QUADRUPED_SDK_MAPPING_MAP_MANAGER_HPP

#include "../common/types.hpp"
#include <string>
#include <vector>

namespace robot {
namespace q25 {

/**
 * MapManager - 地图与导航轨迹管理接口
 * 提供场景、地图、导航轨迹的管理功能
 */
class MapManager {
public:
    explicit MapManager();
    virtual ~MapManager() = default;

    // ============ 场景管理 ============

	/**
	 * 请求获取最新场景
	 */
    virtual void refreshScenes() = 0;
	
	/**
	 * 订阅场景上报事件
	 * 由refreshScenes()请求后上报
	 */
	virtual void subscribeSceneUpdate(SceneUpdateCallback callback) = 0;

    /**
     * 获取所有场景列表
	 * refreshScenes()刷新完成后，通过此方法获取最新数据
     * @return 场景名称列表
     */
    virtual std::vector<std::string> getScenes() = 0;

    /**
     * 获取所有场景详情
	 * refreshScenes()刷新完成后，通过此方法获取最新数据
     * @return 场景详情列表（包含所有场景及其子场景信息）
     */
    virtual std::vector<SceneDetail> getScenesDetails() = 0;

    /**
     * 获取指定场景详情
     * @param scene_name 场景名称
     * @return 场景详情（包含子场景列表）
     */
    virtual SceneDetail getScenesDetail(const std::string& scene_name) = 0;

    /**
     * 删除场景
     * @param scene_name 场景名称
     */
    virtual void deleteScene(const std::string& scene_name) = 0;

    /**
     * 删除所有场景
     */
    virtual void deleteAllScenes() = 0;

    // ============ 地图管理 ============

    /**
     * 下载地图文件到本地
     * SDK内部通过FTP下载YAM和PGM文件到指定目录
     * @param scene_name 场景名称
     * @param sub_scene_id 子场景ID
     * @param save_dir 保存目录（绝对路径）
     * @param callback 下载完成回调（参数表示成功/失败）
     */
    virtual void downloadMap(const std::string& scene_name,
                            uint32_t sub_scene_id,
                            const std::string& save_dir,
                            std::function<void(bool)> callback) = 0;

    // ============ 导航轨迹管理 ============

	/**
	 * 请求获取最新导航轨迹
	 */
    virtual void refreshTrajectories() = 0;

    /**
     * 获取所有场景的导航轨迹组详情
     * @param scene_name 场景名称
     * @return 所有场景的导航轨迹组列表
     */
    virtual std::vector<NavigationTrajectory> getNavigationTrajectories() = 0;

    /**
     * 获取指定场景的导航轨迹组详情
     * @param scene_name 场景名称
     * @return 某场景的导航轨迹组
     */
    virtual NavigationTrajectory getNavigationTrajectory(const std::string& scene_name) = 0;

    /**
     * 获取指定场景-导航轨迹组中的某条轨迹详情
     * @param scene_name 场景名称
     * @param path_name 路径名称
     * @return 某场景-导航轨迹组的某条轨迹详情
     */
    virtual NavigationPath getNavigationPath(const std::string& scene_name,
                                             const std::string& path_name) = 0;

	/**
     * 删除某场景的导航轨迹组
     * @param scene_name 场景名称
     */
    virtual void deleteNavigationTrajectory(const std::string& scene_name) = 0;

    /**
     * 删除某场景-导航轨迹组中的某条轨迹
     * @param scene_name 场景名称
     * @param path_name 轨迹名称
     */
    virtual void deleteNavigationPath(const std::string& scene_name,
                                      const std::string& path_name) = 0;

    /**
     * 重命名某场景-导航轨迹组中的某条轨迹
     * @param scene_name 场景名称
     * @param old_path_name 旧轨迹名称
     * @param new_path_name 新轨迹名称
     */
    virtual void renameNavigationTrajectory(const std::string& scene_name,
                                            const std::string& old_path_name,
                                            const std::string& new_path_name) = 0;
};

} // namespace q25
} // namespace robot

#endif // QUADRUPED_SDK_MAPPING_MAP_MANAGER_HPP
