# 天狼Q25 四足机器人SDK (Quadruped Robot SDK)

## 概述 (Overview)

四足机器人SDK是为四足机器人设计的C++跨平台开发工具包，提供运动控制、导航、建图、传感器访问等完整功能。SDK采用功能导向的设计理念，对底层通信协议(UDP/MQTT)进行抽象，为二次开发者提供简洁易用的API。

Quadruped Robot SDK is a cross-platform C++ development kit designed for quadruped robots, providing comprehensive functionality including motion control, navigation, mapping, and sensor access. The SDK uses a feature-oriented design philosophy, abstracting the underlying communication protocols (UDP/MQTT) to provide a clean and easy-to-use API for developers.

## 目录结构 (Directory Structure)

```
sdk_design/
├── quadruped_sdk.hpp          # SDK主头文件 (Main SDK header)
├── core/                       # 核心模块 (Core modules)
│   ├── types.hpp              # 基础数据类型定义
│   └── robot.hpp              # 机器人主控制类
├── motion/                     # 运动控制模块 (Motion control)
│   ├── motion_control.hpp     # 运动控制接口
│   └── motion_state.hpp       # 运动状态查询
├── navigation/                 # 导航模块 (Navigation)
│   ├── point_navigation.hpp   # 定点导航
│   └── track_navigation.hpp   # 循迹导航
├── mapping/                    # 建图与定位模块 (Mapping & Localization)
│   ├── slam.hpp               # SLAM建图与定位
│   └── map_manager.hpp        # 地图管理
├── sensor/                     # 传感器模块 (Sensors)
│   ├── imu.hpp                # IMU传感器
│   ├── lidar.hpp              # LiDAR传感器
│   └── battery.hpp            # 电池传感器
└── utils/                      # 工具模块 (Utilities)
    └── error.hpp              # 错误处理
```

## 主要功能 (Key Features)

### 1. 运动控制 (Motion Control)

- **基础控制**: 站立、趴下、急停
- **速度控制**: 手动模式下的速度指令
- **步态控制**: 行走/跑步步态切换
- **姿态控制**: 机身高度、姿态调整
- **运动模式**: 手动、导航、辅助模式

### 2. 定点导航 (Point Navigation)

- **场景管理**: 加载、删除、导入导出场景
- **导航点管理**: 添加、删除、更新导航点
- **导航控制**: 导航到点、取消、暂停、恢复
- **状态查询**: 导航状态、进度、距离

### 3. 循迹导航 (Track Following)

- **路径管理**: 获取、删除、重命名路径
- **路径录制**: 实时录制运动轨迹
- **循迹导航**: 正向/反向、循环模式
- **进度监控**: 路径进度、状态查询

### 4. 建图与定位 (SLAM)

- **建图功能**: 开始、暂停、保存、取消建图
- **实时定位**: 获取位姿、定位置信度
- **地图管理**: 场景管理、地图导入导出
- **统计信息**: 建图面积、轨迹长度

### 5. 传感器访问 (Sensor Access)

- **IMU**: 欧拉角、角速度、线加速度
- **LiDAR**: 点云数据、多LiDAR支持
- **电池**: 电量、充电状态、详细信息

### 6. 工具功能 (Utilities)

- **错误处理**: 异常类、错误信息查询

## 快速开始 (Quick Start)

### 基本使用示例 (Basic Usage Example)

```cpp
#include <quadruped_sdk.hpp>

using namespace quadruped::sdk;

int main() {
    // 创建运动控制模块
    MotionController motion;

    // 站立
    motion.stand();

    // 设置速度
    Velocity vel;
    vel.linear_x = 0.5;  // 前进 0.5 m/s
    vel.linear_y = 0.0;
    vel.angular_z = 0.0;
    motion.setVelocity(vel);

    return 0;
}
```

### 使用Robot类管理连接 (Using Robot Class for Connection Management)

```cpp
#include <quadruped_sdk.hpp>

using namespace quadruped::sdk;

int main() {
    // 创建Robot实例管理连接
    Robot robot("192.168.1.100");

    // 连接到机器人
    if (!robot.connect()) {
        std::cerr << "Failed to connect to robot" << std::endl;
        return -1;
    }

    // 创建运动控制模块
    MotionController motion;
    motion.stand();

    // 断开连接
    robot.disconnect();

    return 0;
}
```

### 定点导航示例 (Point Navigation Example)

```cpp
#include <quadruped_sdk.hpp>

using namespace quadruped::sdk;

int main() {
    // 创建定点导航模块
    PointNavigation pointNav;

    // 获取场景列表
    auto scenes = pointNav.getScenes();

    // 加载第一个场景
    if (!scenes.empty()) {
        pointNav.loadScene(scenes[0].scene_id);

        // 获取导航点
        auto waypoints = pointNav.getWayPoints();

        // 导航到第一个点
        if (!waypoints.empty()) {
            pointNav.navigateToPoint(waypoints[0].point_id);

            // 轮询导航状态直到完成
            while (pointNav.getNavigationStatus() == NavigationStatus::RUNNING) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
    }

    return 0;
}
```

### 循迹导航示例 (Track Navigation Example)

```cpp
#include <quadruped_sdk.hpp>

using namespace quadruped::sdk;

int main() {
    // 创建循迹导航模块
    TrackNavigation trackNav;

    // 开始录制路径
    trackNav.startRecording("my_path");

    // 手动控制机器人移动...
    // (用户通过其他方式控制机器人移动)

    // 停止录制
    trackNav.stopRecording();

    // 获取路径列表
    auto paths = trackNav.getPaths();

    // 执行循迹导航
    if (!paths.empty()) {
        trackNav.startTrackFollowing(paths[0].path_id, true);
    }

    return 0;
}
```

### 建图示例 (Mapping Example)

```cpp
#include <quadruped_sdk.hpp>

using namespace quadruped::sdk;

int main() {
    // 创建SLAM模块
    SLAM slam;

    // 开始建图
    slam.startMapping("office_map");

    // 手动控制机器人移动以建图...
    // (用户控制机器人探索环境)

    // 停止建图并保存
    slam.stopMappingAndSave();

    return 0;
}
```

### 多模块使用示例 (Multiple Modules Example)

```cpp
#include <quadruped_sdk.hpp>

using namespace quadruped::sdk;

int main() {
    // 使用Robot管理连接
    Robot robot("192.168.1.100");
    robot.connect();

    // 创建多个模块
    MotionController motion;
    IMUSensor imu;
    BatterySensor battery;

    // 使用运动控制
    motion.stand();

    // 轮询获取IMU数据
    IMUData imu_data = imu.getData();
    std::cout << "IMU - Roll: " << imu_data.roll
              << " Pitch: " << imu_data.pitch
              << " Yaw: " << imu_data.yaw << std::endl;

    // 轮询获取电池状态
    BatteryState bat_state = battery.getBatteryState();
    std::cout << "Battery: " << (int)bat_state.percentage << "%" << std::endl;
    if (bat_state.is_charging) {
        std::cout << "Charging..." << std::endl;
    }

    // 断开连接
    robot.disconnect();

    return 0;
}
```

## API设计原则 (API Design Principles)

1. **功能导向 (Feature-Oriented)**: 按功能模块组织，而非通信协议
2. **简洁易用 (Simple & Easy)**: 提供直观的高层API
3. **模块化设计 (Modular Design)**: 功能模块独立，可单独创建和使用
4. **分离关注点 (Separation of Concerns)**: Robot类负责连接管理，功能模块专注业务逻辑
5. **轮询模式 (Polling-Based)**: 通过轮询方式获取状态，无回调机制
6. **类型安全 (Type-Safe)**: 使用强类型枚举和结构体
7. **跨平台 (Cross-Platform)**: 纯头文件接口，便于跨平台实现

## 编译要求 (Build Requirements)

- C++11 或更高版本
- 支持的平台: Windows, Linux, macOS

## 接口实现说明 (Implementation Notes)

本SDK提供的是纯接口头文件，具体实现需要：

1. 实现各个功能模块接口类 (MotionController, PointNavigation等)
2. 实现Robot类的连接管理功能（UDP/MQTT通信）
3. 功能模块通过单例或全局连接获取底层通信能力
4. 处理底层协议细节 (UDP指令、MQTT消息、Protobuf序列化)
5. 实现线程安全的状态查询机制
6. 使用轮询模式获取状态，无需实现回调机制

## 版本历史 (Version History)

- **v1.0.0** (2025-12-16): 初始版本
  - 模块化架构，功能模块可独立创建和使用
  - Robot类负责连接管理
  - 轮询模式状态查询，无回调机制
  - 完整的运动控制API
  - 定点导航和循迹导航
  - SLAM建图与定位
  - 传感器数据访问
  - 错误处理和日志系统

## 许可证 (License)

[待定 / TBD]

## 联系方式 (Contact)

[待定 / TBD]
