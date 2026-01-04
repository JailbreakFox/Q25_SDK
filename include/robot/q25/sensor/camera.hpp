#ifndef QUADRUPED_SDK_SENSOR_CAMERA_HPP
#define QUADRUPED_SDK_SENSOR_CAMERA_HPP

#include "../common/types.hpp"
#include <cstdint>
#include <string>

namespace robot {
namespace q25 {

/**
 * 相机位置枚举
 * 对应 ROBOT_CAMERA_LOCATION
 */
enum class CameraLocation {
    MAIN = 0,           // 主视相机
    FRONT_ROUND = 1,    // 前环视相机
    BACK_ROUND = 2,     // 后环视相机
    LEFT_ROUND = 3,     // 左环视相机
    RIGHT_ROUND = 4,    // 右环视相机
    ROUND = 5           // 全景环视
};

/**
 * 相机开关控制类型
 */
enum class CameraSwitchType {
    ALL_CLOSE = 0,          // 全部关闭
    OPEN_ONLY_MAIN = 1,     // 仅开主视相机
    OPEN_ONLY_ROUND = 2,    // 仅开环视相机
    ALL_OPEN = 3            // 全部打开
};

/**
 * 相机信息
 */
struct CameraInfo {
    CameraLocation location;    // 相机位置
    bool is_enabled;            // 是否启用
    std::string rtsp_url;       // RTSP地址
};

/**
 * CameraSensor - 相机传感器接口
 * 提供相机控制和信息访问功能
 * 可独立使用，不依赖其他模块
 */
class CameraSensor {
public:
    /**
     * 构造函数
     */
    explicit CameraSensor();

    virtual ~CameraSensor() = default;

    // ============ 相机控制 ============

    /**
     * 设置相机开关
     * @param type 开关类型
     * @return true表示操作成功
     */
    virtual bool setCameraEnabled(CameraSwitchType type) = 0;

    // ============ 信息查询 ============

    /**
     * 获取相机RTSP地址
     * @param location 相机位置
     * @return RTSP地址字符串，失败返回空字符串
     */
    virtual std::string getRtspUrl(CameraLocation location) const = 0;
};

} // namespace q25
} // namespace robot

#endif // QUADRUPED_SDK_SENSOR_CAMERA_HPP

