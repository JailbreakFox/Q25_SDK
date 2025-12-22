#ifndef QUADRUPED_SDK_CORE_TYPES_HPP
#define QUADRUPED_SDK_CORE_TYPES_HPP

#include <cstdint>
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

// 场景信息
struct SceneInfo {
    uint32_t scene_id;
    std::string scene_name;
    std::vector<std::string> map_files;  // yam, pgm files
};

// 路径点
struct WayPoint {
    uint32_t point_id;
    Pose pose;
};

// 导航路径
struct NavigationPath {
    uint32_t path_id;
    std::string path_name;
    std::vector<WayPoint> waypoints;
    bool bidirectional;  // 单向/双向
};

// 导航模式
enum class NavigationMode {
    NONE = 0,
    POINT_TO_POINT = 1,  // 定点导航
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

} // namespace q25
} // namespace robot

#endif // QUADRUPED_SDK_CORE_TYPES_HPP
