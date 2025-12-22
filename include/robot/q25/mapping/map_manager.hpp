#ifndef QUADRUPED_SDK_MAPPING_MAP_MANAGER_HPP
#define QUADRUPED_SDK_MAPPING_MAP_MANAGER_HPP

#include "../core/types.hpp"
#include <string>
#include <vector>

namespace robot {
namespace q25 {

/**
 * 地图元数据
 */
struct MapMetadata {
    uint32_t width;           // 宽度 (像素)
    uint32_t height;          // 高度 (像素)
    float resolution;         // 分辨率 (m/pixel)
    Pose origin;              // 地图原点位姿
    double creation_time;     // 创建时间
    double modification_time; // 修改时间
};

/**
 * MapManager - 地图管理接口
 * 提供场景和地图的管理功能
 * 可独立使用，不依赖其他模块
 */
class MapManager {
public:
    /**
     * 构造函数
     */
    explicit MapManager();

    virtual ~MapManager() = default;

    // ============ 场景管理 ============

    /**
     * 获取所有场景列表
     * @return 场景信息列表
     */
    virtual std::vector<SceneInfo> getScenes() = 0;

    /**
     * 获取场景详情
     * @param scene_id 场景ID
     * @return 场景信息
     */
    virtual SceneInfo getScene(uint32_t scene_id) = 0;

    /**
     * 删除场景
     * @param scene_id 场景ID
     * @return true表示删除成功
     */
    virtual bool deleteScene(uint32_t scene_id) = 0;

    /**
     * 重命名场景
     * @param scene_id 场景ID
     * @param new_name 新场景名称
     * @return true表示重命名成功
     */
    virtual bool renameScene(uint32_t scene_id, const std::string& new_name) = 0;

    /**
     * 导出场景
     * @param scene_id 场景ID
     * @param export_path 导出路径
     * @return true表示导出成功
     */
    virtual bool exportScene(uint32_t scene_id, const std::string& export_path) = 0;

    /**
     * 导入场景
     * @param import_path 导入路径
     * @param scene_name 场景名称
     * @return 场景ID，失败返回0
     */
    virtual uint32_t importScene(const std::string& import_path, const std::string& scene_name) = 0;

    // ============ 地图管理 ============

    /**
     * 获取地图元数据
     * @param scene_id 场景ID
     * @return 地图元数据
     */
    virtual MapMetadata getMapMetadata(uint32_t scene_id) = 0;

    /**
     * 获取地图图像数据 (PGM格式)
     * @param scene_id 场景ID
     * @param output_buffer [out] 输出缓冲区
     * @param buffer_size [in/out] 缓冲区大小
     * @return true表示获取成功
     */
    virtual bool getMapImage(uint32_t scene_id, uint8_t* output_buffer, size_t& buffer_size) = 0;

    /**
     * 保存地图到文件
     * @param scene_id 场景ID
     * @param file_path 文件路径
     * @return true表示保存成功
     */
    virtual bool saveMapToFile(uint32_t scene_id, const std::string& file_path) = 0;

    // ============ 场景状态 ============

    /**
     * 获取当前加载的场景ID
     * @return 场景ID，未加载返回0
     */
    virtual uint32_t getCurrentSceneId() const = 0;

    /**
     * 检查场景是否存在
     * @param scene_id 场景ID
     * @return true表示场景存在
     */
    virtual bool sceneExists(uint32_t scene_id) const = 0;
};

} // namespace q25
} // namespace robot

#endif // QUADRUPED_SDK_MAPPING_MAP_MANAGER_HPP
