#ifndef QUADRUPED_SDK_MAPPING_SLAM_HPP
#define QUADRUPED_SDK_MAPPING_SLAM_HPP

#include "../core/types.hpp"
#include <string>

namespace quadruped {
namespace sdk {

/**
 * 建图状态
 */
enum class MappingStatus {
    IDLE = 0,        // 空闲
    RUNNING = 1,     // 建图中
    PAUSED = 2,      // 已暂停
    SAVING = 3,      // 保存中
    COMPLETED = 4,   // 已完成
    FAILED = 5       // 失败
};

/**
 * 建图统计信息
 */
struct MappingStatistics {
    float mapped_area;          // 已建图区域面积 (m²)
    uint32_t trajectory_length; // 轨迹长度 (m)
    uint32_t duration;          // 建图时长 (秒)
    float loop_closure_count;   // 回环检测次数
};

/**
 * SLAM - 建图与定位接口
 * 提供SLAM建图和实时定位功能
 * 可独立使用，不依赖其他模块
 */
class SLAM {
public:
    /**
     * 构造函数
     */
    explicit SLAM();

    virtual ~SLAM() = default;

    // ============ 建图控制 ============

    /**
     * 开始建图
     * @param scene_name 场景名称
     * @return true表示建图任务启动成功
     */
    virtual bool startMapping(const std::string& scene_name) = 0;

    /**
     * 暂停建图
     * @return true表示暂停成功
     */
    virtual bool pauseMapping() = 0;

    /**
     * 恢复建图
     * @return true表示恢复成功
     */
    virtual bool resumeMapping() = 0;

    /**
     * 停止建图并保存
     * @return true表示保存成功，返回场景ID
     */
    virtual uint32_t stopMappingAndSave() = 0;

    /**
     * 取消建图 (不保存)
     * @return true表示取消成功
     */
    virtual bool cancelMapping() = 0;

    // ============ 建图状态 ============

    /**
     * 获取建图状态
     * @return 建图状态
     */
    virtual MappingStatus getMappingStatus() const = 0;

    /**
     * 获取建图统计信息
     * @return 统计信息
     */
    virtual MappingStatistics getMappingStatistics() const = 0;

    /**
     * 检查是否正在建图
     * @return true表示正在建图
     */
    virtual bool isMapping() const = 0;

    // ============ 定位功能 ============

    /**
     * 获取当前位姿 (在已加载场景中)
     * @return 当前位姿
     */
    virtual Pose getCurrentPose() const = 0;

    /**
     * 获取定位置信度
     * @return 置信度 (0.0-1.0)
     */
    virtual float getLocalizationConfidence() const = 0;

    /**
     * 设置初始位姿 (用于定位初始化)
     * @param pose 初始位姿
     * @return true表示设置成功
     */
    virtual bool setInitialPose(const Pose& pose) = 0;

    /**
     * 检查定位是否正常
     * @return true表示定位正常
     */
    virtual bool isLocalized() const = 0;
};

} // namespace sdk
} // namespace quadruped

#endif // QUADRUPED_SDK_MAPPING_SLAM_HPP
