#ifndef QUADRUPED_SDK_CORE_TYPES_HPP
#define QUADRUPED_SDK_CORE_TYPES_HPP

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace robot {
namespace q25 {

// 基础数据类型
struct Point2D {
    float x;
    float y;
};

struct Point3D {
    float x;
    float y;
    float z;
};

struct Quaternion {
    float x;
    float y;
    float z;
    float w;
};

struct Pose {
    Point3D position;
    Quaternion orientation;
};

// 速度信息
struct Velocity {
    float linear_x;   // 线速度 x (m/s)
    float linear_y;   // 线速度 y (m/s)
    float angular_z;  // 角速度 z (rad/s)
};

// 轴类型 (对应摇杆轴)
enum class AxisType {
    LEFT_Y = 0,     // 左摇杆Y轴 (前后) 死区: ±6553
    LEFT_X = 1,     // 左摇杆X轴 (左右) 死区: ±24576
    RIGHT_X = 2     // 右摇杆X轴 (旋转) 死区: ±28212
};

// 关节ID枚举 (Q25 12关节)
enum class JointId {
    FL_HIP_X = 0,   // 左前侧摆
    FL_HIP_Y = 1,   // 左前髋
    FL_KNEE = 2,    // 左前膝
    FR_HIP_X = 3,   // 右前侧摆
    FR_HIP_Y = 4,   // 右前髋
    FR_KNEE = 5,    // 右前膝
    HL_HIP_X = 6,   // 左后侧摆
    HL_HIP_Y = 7,   // 左后髋
    HL_KNEE = 8,    // 左后膝
    HR_HIP_X = 9,   // 右后侧摆
    HR_HIP_Y = 10,  // 右后髋
    HR_KNEE = 11    // 右后膝
};

// 导航模式
enum class NavigationMode {
    NONE = 0,
    POINT_TO_POINT = 1,   // 定点导航
    TRACK_FOLLOWING = 2   // 循迹导航
};

// 运动模式
enum class MotionMode {
    MANUAL = 0,      // 手动模式
    NAVIGATION = 1,  // 导航模式
    ASSISTED = 2     // 辅助模式
};

// 步态类型
enum class GaitType {
    WALK = 0,
    RUN = 1
};

// 速度档位
enum class SpeedLevel {
    LOW = 0,
    HIGH = 1
};

// 机器人基本状态
enum class RobotBasicState {
    LYING = 0,
    STANDING_UP = 1,
    STANDING = 2,
    FORCE_STANDING = 3,
    STEPPING = 4,
    LYING_DOWN = 5,
    EMERGENCY_STOP = 6
};

// 导航状态
enum class NavigationStatus {
    IDLE = 0,
    RUNNING = 1,
    FINISHED = 2,
    CANCELLED = 3,
    FAILED = 4
};

// 错误代码
enum class ErrorCode {
    SUCCESS = 0,
    UNKNOWN_ERROR = 1,
    OBSTACLE_DETECTED = 2,
    LOCALIZATION_LOST = 3,
    PATH_BLOCKED = 4,
    TIMEOUT = 5
};

// IMU 数据
struct IMUData {
    double timestamp;
    float roll;     // 度
    float pitch;    // 度
    float yaw;      // 度
    float omega_x;  // rad/s
    float omega_y;  // rad/s
    float omega_z;  // rad/s
    float acc_x;    // m/s²
    float acc_y;    // m/s²
    float acc_z;    // m/s²
};

// 电池状态
struct BatteryState {
    uint8_t percentage;  // 0-100%
    bool is_charging;
};

// ============ SLAM 建图与定位相关类型 ============

// SLAM工作模式（对齐实际系统）
enum class SLAMWorkMode {
    IDLE = 0,         // 空闲模式
    MAPPING = 1,      // 建图模式
    SAVING = 2,       // 保存地图模式
    RELOCATING = 3,   // 重定位模式
    LOCALIZING = 4    // 定位模式
};

// SLAM错误码（对齐实际系统）
enum class SLAMErrorCode {
    NORMAL = 0,           // 无异常
    UNABLE_START = 1,     // 无法启动定位
    NO_IMU = 2,           // 无IMU数据
    NO_LIDAR = 3,         // 无雷达数据
    LIDAR_INCOMPLETE = 4, // 雷达被遮挡
    MAP_NOFOUND = 5,      // 无法找到3D地图
    LOCALIZE_ERROR = 6,   // 定位失败
    SLAM_COLLAPSE = 7,    // SLAM崩溃
    MAPPING_FAIL = 8,     // 建图失败
    LIDAR_ERROR = 9,      // 雷达异常
    IMU_ERROR = 10,       // IMU异常
    GNSS_ERROR = 11,      // GNSS异常
    SYNC_ERROR = 12       // 传感器同步异常
};

// 建图场景类型
enum class MappingSceneType {
    INDOOR = 1,        // 室内建图
    OUTDOOR_NORMAL = 2,  // 室外普通建图
    OUTDOOR_OPEN = 3     // 室外空旷建图
};

// 建图命令
enum class MappingCommand {
    START = 1,    // 开始建图
    FINISH = 2    // 结束建图
};

// 建图路径点
struct MappingPathPoint {
    double x;
    double y;
    double z;
};

// 定位信息
struct LocalizationInfo {
    float position_x;
    float position_y;
    float position_z;
    float orientation_w;
    float orientation_x;
    float orientation_y;
    float orientation_z;
    float laser_quality;  // 激光定位质量
};

// ============ 轨迹录制相关类型 ============

// 轨迹录制命令
enum class RecordCommand {
    START = 0,  // 开始录制
    ADD = 1,    // 添加点位
    END = 2     // 结束录制
};

// 轨迹录制结果
enum class RecordResult {
    POINT_ADDED = 0,  // 添加点位成功
    SUCCESS = 1,      // 完成录制
    FAIL = 2          // 录制失败
};

// 子场景信息
struct SceneInfo {
    uint32_t sub_scene_id;       // 子场景ID
    std::string yam_filename;    // YAM文件路径
    std::string pgm_filename;    // PGM文件路径
};

// 场景详情
struct SceneDetail {
    std::string scene_name;
    std::vector<SceneInfo> sub_scenes;  // 子场景列表
};

// 导航点
struct NavigationPoint {
    int32_t point_id;     // 点ID
    int32_t sub_scene_id; // 子场景ID
    Pose pose;            // 位姿
};

// 导航路径
struct NavigationPath {
    int32_t path_id;              // 路径ID
    std::string path_name;        // 路径名称
    std::vector<Pose> points;     // 路径点列表（仅包含位姿信息）
};

// 导航轨迹
struct NavigationTrajectory {
    int32_t trajectory_id;                     // 轨迹ID
    std::string scene_name;                    // 场景名称
    std::vector<NavigationPoint> waypoints;    // 导航点信息
    std::vector<NavigationPath> paths;         // 导航路径信息
};

// 轨迹录制事件回调类型
using RecordingEventCallback = std::function<void(RecordResult)>;

// 场景更新事件回调类型
using SceneUpdateCallback = std::function<void(std::vector<SceneDetail>)>;

// 导航轨迹更新事件回调类型
using NavigationTrajectoryUpdateCallback = std::function<void(std::vector<NavigationTrajectory>)>;

} // namespace q25
} // namespace robot

#endif // QUADRUPED_SDK_CORE_TYPES_HPP
