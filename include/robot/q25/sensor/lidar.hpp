#ifndef QUADRUPED_SDK_SENSOR_LIDAR_HPP
#define QUADRUPED_SDK_SENSOR_LIDAR_HPP

#include "../common/types.hpp"
#include <vector>
#include <cstdint>
#include <string>

namespace robot {
namespace q25 {

/**
 * LiDAR点云数据点
 */
struct LiDARPoint {
    float x;          // X坐标 (m)
    float y;          // Y坐标 (m)
    float z;          // Z坐标 (m)
    float intensity;  // 强度
};

/**
 * LiDAR扫描数据
 */
struct LiDARScan {
    double timestamp;               // 时间戳
    std::vector<LiDARPoint> points; // 点云数据
    uint32_t lidar_id;              // LiDAR ID (多LiDAR系统)
};

/**
 * LiDARSensor - LiDAR传感器接口
 * 提供LiDAR点云数据访问功能
 * 可独立使用，不依赖其他模块
 */
class LiDARSensor {
public:
    LiDARSensor();
    ~LiDARSensor();

    // 禁用复制
    LiDARSensor(const LiDARSensor&) = delete;
    LiDARSensor& operator=(const LiDARSensor&) = delete;

    // ============ 数据访问 ============

    /**
     * 获取最新的LiDAR扫描数据
     * @param lidar_id LiDAR ID (0表示默认/主LiDAR)
     * @return LiDAR扫描数据
     */
    LiDARScan getLatestScan(uint32_t lidar_id = 0) const;

    /**
     * 获取可用的LiDAR数量
     * @return LiDAR数量
     */
    uint32_t getLiDARCount() const;

    // ============ 控制功能 ============

    /**
     * 启用/禁用LiDAR
     * @param lidar_id LiDAR ID
     * @param enable true=启用, false=禁用
     * @return true表示操作成功
     */
    bool setLiDAREnabled(uint32_t lidar_id, bool enable);

    /**
     * 检查LiDAR是否启用
     * @param lidar_id LiDAR ID
     * @return true表示已启用
     */
    bool isLiDAREnabled(uint32_t lidar_id) const;

private:
    struct Impl;
    Impl* impl_;
};

} // namespace q25
} // namespace robot

#endif // QUADRUPED_SDK_SENSOR_LIDAR_HPP
