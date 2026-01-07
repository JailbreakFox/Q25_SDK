#ifndef QUADRUPED_SDK_MAPPING_SLAM_HPP
#define QUADRUPED_SDK_MAPPING_SLAM_HPP

#include "../common/types.hpp"
#include <string>
#include <vector>

namespace robot {
namespace q25 {

/**
 * SLAM - 建图与定位接口
 * 提供SLAM建图、定位、轨迹录制功能
 */
class SLAM {
public:
    SLAM();
    ~SLAM();

    // 禁用复制
    SLAM(const SLAM&) = delete;
    SLAM& operator=(const SLAM&) = delete;

    // ============ 建图功能 ============

    /**
     * 开始建图
     * @param scene_name 场景名称
     * @param scene_type 场景类型（室内/室外普通/室外空旷）
     */
    void startMapping(const std::string& scene_name, MappingSceneType scene_type);

    /**
     * 结束建图并保存（默认使用上一次的开始建图场景）
     * @param scene_name 场景名称
     */
    void finishMapping();

    /**
     * 获取SLAM工作模式
     * @return 当前工作模式（0=空闲, 1=建图, 2=保存, 3=重定位, 4=定位）
     */
    SLAMWorkMode getWorkMode() const;

    /**
     * 获取SLAM错误码
     * @return 错误码（0=正常, 1-12=各种错误）
     */
    SLAMErrorCode getErrorCode() const;

    /**
     * 获取建图路径点
	 * 注意：必须在建图模式下且定位正常时才能获取路径点
     * @return 建图路径点列表（实时更新）
     */
    std::vector<MappingPathPoint> getMappingPathPoints() const;

    /**
     * 检查是否正在建图
     * @return true表示正在建图
     */
    bool isMapping() const;

    // ============ 定位功能 ============

    /**
     * 开启定位
     * @param scene_name 场景名称
     */
    void startLocalization(const std::string& scene_name);

    /**
     * 关闭定位
     * @param scene_name 场景名称
     */
    void stopLocalization(const std::string& scene_name);

    /**
     * 获取当前定位信息
     * @return 定位信息
     */
    LocalizationInfo getLocalizationInfo() const;

    /**
     * 检查是否正在定位中
     */
    bool isLocalized() const;

    // ============ 轨迹录制功能 ============

    /**
     * 开始轨迹录制（默认双向）
     * 注意：必须在定位模式下且定位正常时才能开始录制
     */
    void startRecording();

    /**
     * 在当前位置添加路径点（关键点）
     */
    void addPathPoint();

    /**
     * 结束轨迹录制并保存
     */
    void finishRecording();

    /**
     * 订阅录制事件
     * 轨迹录制过程中触发回调
     * @param callback 事件回调函数
     */
    void subscribeRecordingEvent(RecordingEventCallback callback);

private:
    struct Impl;
    Impl* impl_;
};

} // namespace q25
} // namespace robot

#endif // QUADRUPED_SDK_MAPPING_SLAM_HPP
