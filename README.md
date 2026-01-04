# 天狼 Q25 四足机器人 SDK

## 概述

Q25_SDK 是天狼 Q25 Ultra 四足机器人的 C++ 软件开发套件，提供了机器人控制、导航、传感器访问、安全监控等功能的统一接口。

**版本**: 1.0.0

## 特性

- 模块化设计，各模块可独立使用
- 纯头文件实现，易于集成
- 完整的类型定义和接口抽象
- 支持运动控制、导航、SLAM、传感器、安全监控等功能

## 环境要求

- C++11 或更高版本
- 支持 POSIX 的操作系统（Linux）

## 使用方法

### 包含头文件

将 `include` 目录添加到编译器的包含路径中，然后：

```cpp
// 包含主头文件（包含所有模块）
#include <robot/q25/quadruped_sdk.hpp>

// 或单独包含需要的模块
#include <robot/q25/common/types.hpp>
#include <robot/q25/motion/motion_control.hpp>
#include <robot/q25/sensor/joint.hpp>
```

### 使用命名空间

```cpp
using namespace robot::q25;

// 或使用完整命名空间
robot::q25::MotionController* controller;
robot::q25::Point2D point;
```

## 目录结构

```
Q25_SDK/
├── include/
│   └── robot/
│       └── q25/
│           ├── quadruped_sdk.hpp       # 主头文件（包含所有模块）
│           ├── core/                   # 核心模块
│           │   ├── types.hpp           # 类型定义
│           │   └── robot.hpp           # 机器人连接管理
│           ├── motion/                 # 运动控制
│           │   ├── motion_control.hpp  # 运动控制接口
│           │   └── motion_state.hpp    # 运动状态查询
│           ├── navigation/             # 导航
│           │   ├── point_navigation.hpp # 定点导航
│           │   └── track_navigation.hpp # 循迹导航
│           ├── mapping/                # 建图与定位
│           │   ├── slam.hpp            # SLAM接口
│           │   └── map_manager.hpp     # 地图管理
│           ├── sensor/                 # 传感器
│           │   ├── imu.hpp             # IMU传感器
│           │   ├── lidar.hpp           # 激光雷达
│           │   ├── battery.hpp         # 电池状态
│           │   ├── joint.hpp           # 关节数据
│           │   └── camera.hpp          # 摄像头
│           ├── safety/                 # 安全监控
│           │   └── safety_monitor.hpp  # 安全状态监控
│           ├── charging/               # 充电
│           │   └── auto_charge.hpp     # 自主充电
│           ├── system/                 # 系统信息
│           │   └── system_info.hpp     # 系统信息查询
│           └── utils/                  # 工具
│               └── error.hpp           # 错误处理
└── README.md
```

## 模块说明

### Core - 核心模块

| 文件 | 类/结构体 | 功能 |
|------|-----------|------|
| `types.hpp` | `Point2D`, `Point3D`, `Pose` | 基础几何类型 |
| | `AxisType`, `JointId` | 轴类型和关节ID枚举 |
| | `MotionMode`, `GaitType`, `SpeedLevel` | 运动相关枚举 |
| | `NavigationMode`, `NavigationStatus` | 导航相关枚举 |
| | `RobotBasicState`, `ErrorCode` | 状态和错误码 |
| | `IMUData`, `BatteryState` | 传感器数据结构 |
| | `SceneInfo`, `WayPoint`, `NavigationPath` | 导航数据结构 |
| `robot.hpp` | `Robot` | 机器人连接管理 |

### Motion - 运动控制

| 文件 | 类 | 功能 |
|------|-----|------|
| `motion_control.hpp` | `MotionController` | 运动控制 |
| `motion_state.hpp` | `MotionState` | 运动状态查询 |

**MotionController 接口**:

| 方法 | 说明 |
|------|------|
| `stand()` / `lie()` | 站立/趴下 |
| `emergencyStop()` | 急停 |
| `setMotionMode(mode)` | 设置运动模式 |
| `setAxisValue(axis, value)` | 轴控制（底层移动接口） |
| `setGait(gait)` | 设置步态 |
| `setSpeedLevel(level)` | 设置速度档位 |
| `setBodyHeight(height)` | 设置机身高度 |
| `setBodyPose(roll, pitch, yaw)` | 设置机身姿态 |

**轴控制说明**:

| 轴类型 | 功能 | 死区范围 |
|--------|------|----------|
| `LEFT_Y` | 前进/后退 | ±6553 |
| `LEFT_X` | 左移/右移 | ±24576 |
| `RIGHT_X` | 左转/右转 | ±28212 |

### Navigation - 导航

| 文件 | 类 | 功能 |
|------|-----|------|
| `point_navigation.hpp` | `PointNavigation` | 定点导航 |
| `track_navigation.hpp` | `TrackNavigation` | 循迹导航 |

**PointNavigation 接口**:

| 方法 | 说明 |
|------|------|
| `getScenes()` / `loadScene()` | 场景管理 |
| `getWayPoints()` / `addWayPoint()` | 导航点管理 |
| `navigateToPoint(id)` | 导航到指定点 |
| `cancelNavigation()` | 取消导航 |
| `getNavigationStatus()` | 获取导航状态 |

**TrackNavigation 接口**:

| 方法 | 说明 |
|------|------|
| `startRecording()` / `stopRecording()` | 路径录制 |
| `startFollowing()` / `stopFollowing()` | 路径跟踪 |
| `getPaths()` | 获取路径列表 |

### Mapping - 建图与定位

| 文件 | 类 | 功能 |
|------|-----|------|
| `slam.hpp` | `SLAM` | 建图与定位 |
| `map_manager.hpp` | `MapManager` | 地图管理 |

**SLAM 接口**:

| 方法 | 说明 |
|------|------|
| `startMapping()` / `stopMapping()` | 建图控制 |
| `startLocalization()` | 启动定位 |
| `getCurrentPose()` | 获取当前位姿 |
| `relocalize()` | 重定位 |

### Sensor - 传感器

| 文件 | 类 | 功能 |
|------|-----|------|
| `imu.hpp` | `IMUSensor` | IMU数据 |
| `lidar.hpp` | `LidarSensor` | 激光雷达数据 |
| `battery.hpp` | `BatterySensor` | 电池状态 |
| `joint.hpp` | `JointSensor` | 关节数据（12关节） |
| `camera.hpp` | `CameraSensor` | 摄像头控制 |

**JointSensor 接口**:

| 方法 | 说明 |
|------|------|
| `getAllJointsData()` | 获取所有关节数据 |
| `getDriverTemperatures()` | 获取驱动器温度 |
| `getMotorTemperatures()` | 获取电机温度 |

**关节ID定义** (Q25 共12个关节):

| 关节ID | 说明 | 关节ID | 说明 |
|--------|------|--------|------|
| `FL_HIP_X` | 左前侧摆 | `FR_HIP_X` | 右前侧摆 |
| `FL_HIP_Y` | 左前髋 | `FR_HIP_Y` | 右前髋 |
| `FL_KNEE` | 左前膝 | `FR_KNEE` | 右前膝 |
| `HL_HIP_X` | 左后侧摆 | `HR_HIP_X` | 右后侧摆 |
| `HL_HIP_Y` | 左后髋 | `HR_HIP_Y` | 右后髋 |
| `HL_KNEE` | 左后膝 | `HR_KNEE` | 右后膝 |

### Safety - 安全监控

| 文件 | 类 | 功能 |
|------|-----|------|
| `safety_monitor.hpp` | `SafetyMonitor` | 安全状态监控 |

**SafetyMonitor 接口**:

| 方法 | 说明 |
|------|------|
| `getSafetyStatus()` | 获取综合安全状态 |
| `isFallProtectionActive()` | 检查摔倒保护状态 |
| `getThermalWarnings()` | 获取过热告警 |
| `getBatteryWarnings()` | 获取电池告警 |
| `getOverloadStatus()` | 获取过载状态 |

**告警类型**:
- 过热告警：关节电机过热、驱动器过热、电池过热
- 电池告警：过温、低电压、低电量、严重低电量
- 保护状态：摔倒保护、急停、过载保护

### Charging - 自主充电

| 文件 | 类 | 功能 |
|------|-----|------|
| `auto_charge.hpp` | `AutoCharge` | 自主充电控制 |

**AutoCharge 接口**:

| 方法 | 说明 |
|------|------|
| `startCharge()` | 开始自主充电 |
| `stopCharge()` | 停止充电任务 |
| `getChargeStatus()` | 获取充电状态 |
| `isCharging()` | 检查是否在充电 |

**充电状态枚举**:
- `IDLE` - 空闲
- `GUIDING_TO_POINT` - 引导至充电点
- `GUIDING_TO_PILE` - 引导至充电桩
- `CHARGING` - 充电中
- `CHARGE_FULL` - 已充满
- `EXITING_PILE` - 退桩中
- 以及各种失败和保护状态

### System - 系统信息

| 文件 | 类 | 功能 |
|------|-----|------|
| `system_info.hpp` | `SystemInfo` | 系统信息查询 |

**SystemInfo 接口**:

| 方法 | 说明 |
|------|------|
| `getVersionInfo()` | 获取各模块版本信息 |
| `getRobotName()` | 获取机器人名称 |
| `getMotionStatistics()` | 获取运动统计 |
| `getResourceStatus()` | 获取系统资源状态 |

**版本信息包含**:
- CMS、PMS、DCS、PNS、ACS、PPS、PMPM 版本
- 模型文件版本、本体软件版本

**运动统计信息**:
- 当次/累计运行时间
- 当次/累计运行里程

**资源状态**:
- CPU 占用率、温度、主频
- 内存占用率
- eMMC 剩余寿命

## 命名空间

所有 SDK 类型和接口都定义在 `robot::q25` 命名空间下：

```cpp
namespace robot {
namespace q25 {
    // 所有类型和接口
}
}
```

命名空间遵循目录结构规范：`robot` 对应 `include/robot/`，`q25` 对应 `q25/` 子目录。

## 版本信息

可通过以下方式获取 SDK 版本：

```cpp
// 获取版本字符串
const char* version = robot::q25::getSDKVersion();  // "1.0.0"

// 获取版本号
int major, minor, patch;
robot::q25::getSDKVersionNumber(major, minor, patch);
```

## 相关文档

- 《天狼Q25 Ultra 软件接口规格说明书.docx》 - UDP 协议详细说明
- `demo/` 目录 - 各功能 Demo 程序及说明
