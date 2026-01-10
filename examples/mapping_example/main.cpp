/**
 * @file main.cpp
 * @brief Q25 SDK - 定位导航功能示例程序
 *
 * 本程序演示了SLAM建图、定位、轨迹录制功能的交互式使用流程
 */

#include <robot/q25/quadruped_sdk.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <iomanip>
#include <limits>

using namespace robot::q25;

/**
 * @class MappingCLI
 * @brief 定位导航功能的CLI交互类
 */
class MappingCLI {
private:
    Robot* robot;
    SLAM* slam;
    MapManager* map_manager;
    bool connected;

    /**
     * @brief 清除输入缓冲区
     */
    void clearInputBuffer() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    /**
     * @brief 获取SLAM工作模式字符串
     */
    std::string getWorkModeString(SLAMWorkMode mode) const {
        switch (mode) {
            case SLAMWorkMode::IDLE:      return "空闲";
            case SLAMWorkMode::MAPPING:   return "建图中";
            case SLAMWorkMode::SAVING:    return "保存中";
            case SLAMWorkMode::RELOCATING: return "重定位中";
            case SLAMWorkMode::LOCALIZING: return "定位中";
            default:                      return "未知";
        }
    }

    /**
     * @brief 获取SLAM错误码字符串
     */
    std::string getErrorCodeString(SLAMErrorCode code) const {
        switch (code) {
            case SLAMErrorCode::NORMAL:            return "正常";
            case SLAMErrorCode::UNABLE_START:      return "无法启动定位";
            case SLAMErrorCode::NO_IMU:            return "无IMU数据";
            case SLAMErrorCode::NO_LIDAR:          return "无雷达数据";
            case SLAMErrorCode::LIDAR_INCOMPLETE:  return "雷达被遮挡";
            case SLAMErrorCode::MAP_NOFOUND:       return "无法找到3D地图";
            case SLAMErrorCode::LOCALIZE_ERROR:    return "定位失败";
            case SLAMErrorCode::SLAM_COLLAPSE:     return "SLAM崩溃";
            case SLAMErrorCode::MAPPING_FAIL:      return "建图失败";
            case SLAMErrorCode::LIDAR_ERROR:       return "雷达异常";
            case SLAMErrorCode::IMU_ERROR:         return "IMU异常";
            case SLAMErrorCode::GNSS_ERROR:        return "GNSS异常";
            case SLAMErrorCode::SYNC_ERROR:        return "传感器同步异常";
            default:                              return "未知错误";
        }
    }

    /**
     * @brief 获取场景类型字符串
     */
    std::string getSceneTypeString(MappingSceneType type) const {
        switch (type) {
            case MappingSceneType::INDOOR:         return "室内";
            case MappingSceneType::OUTDOOR_NORMAL: return "室外普通";
            case MappingSceneType::OUTDOOR_OPEN:   return "室外空旷";
            default:                              return "未知";
        }
    }

public:
    MappingCLI() : robot(nullptr), slam(nullptr), map_manager(nullptr), connected(false) {
        // 直接实例化SLAM和MapManager
        slam = new SLAM();
        map_manager = new MapManager();
    }

    ~MappingCLI() {
        disconnect();
        if (slam) delete slam;
        if (map_manager) delete map_manager;
    }

    // ============ 连接管理 ============

    /**
     * @brief 连接到机器人
     */
    void connectToRobot(const std::string& ip) {
        if (connected) {
            std::cout << "已连接到机器人，请先断开连接。" << std::endl;
            return;
        }

        std::cout << "正在连接到机器人 " << ip << "..." << std::endl;
        robot = new Robot(ip);

        try {
            robot->connect();
            // 等待连接建立
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            if (robot->isConnected()) {
                connected = true;
                std::cout << "成功连接到机器人!" << std::endl;
            } else {
                std::cout << "连接失败，请检查IP地址是否正确。" << std::endl;
                delete robot;
                robot = nullptr;
            }
        } catch (const std::exception& e) {
            std::cout << "连接异常: " << e.what() << std::endl;
            delete robot;
            robot = nullptr;
        }
    }

    /**
     * @brief 断开机器人连接
     */
    void disconnect() {
        if (!connected) {
            return;
        }

        if (robot) {
            robot->disconnect();
            delete robot;
            robot = nullptr;
        }
        connected = false;
        std::cout << "已断开机器人连接。" << std::endl;
    }

    // ============ 建图功能 ============

    /**
     * @brief 开始建图
     */
    void startMapping() {
        if (!connected) {
            std::cout << "请先连接到机器人。" << std::endl;
            return;
        }

        if (slam->isMapping()) {
            std::cout << "当前已在建图模式中。" << std::endl;
            return;
        }

        std::string scene_name;
        int scene_type;

        std::cout << "\n=== 开始建图 ===" << std::endl;
        std::cout << "请输入场景名称: ";
        std::getline(std::cin, scene_name);

        if (scene_name.empty()) {
            std::cout << "场景名称不能为空!" << std::endl;
            return;
        }

        std::cout << "请选择场景类型:" << std::endl;
        std::cout << "  1. 室内" << std::endl;
        std::cout << "  2. 室外普通" << std::endl;
        std::cout << "  3. 室外空旷" << std::endl;
        std::cout << "请输入选项 (1-3): ";
        std::cin >> scene_type;
        clearInputBuffer();

        MappingSceneType type = MappingSceneType::INDOOR;
        switch (scene_type) {
            case 1: type = MappingSceneType::INDOOR; break;
            case 2: type = MappingSceneType::OUTDOOR_NORMAL; break;
            case 3: type = MappingSceneType::OUTDOOR_OPEN; break;
            default:
                std::cout << "无效选项，使用默认值: 室内" << std::endl;
                break;
        }

        try {
            slam->startMapping(scene_name, type);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            if (slam->isMapping()) {
                std::cout << "建图已开始! 场景名称: " << scene_name
                          << ", 场景类型: " << getSceneTypeString(type) << std::endl;
            } else {
                SLAMErrorCode error = slam->getErrorCode();
                std::cout << "建图启动失败! 错误: " << getErrorCodeString(error) << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "异常: " << e.what() << std::endl;
        }
    }

    /**
     * @brief 保存建图
     */
    void finishMapping() {
        if (!connected) {
            std::cout << "请先连接到机器人。" << std::endl;
            return;
        }

        if (!slam->isMapping()) {
            std::cout << "当前不在建图模式中。" << std::endl;
            return;
        }

        std::cout << "\n=== 保存建图 ===" << std::endl;
        std::cout << "正在保存建图数据..." << std::endl;

        try {
            slam->finishMapping();
            // 等待保存完成
            std::this_thread::sleep_for(std::chrono::seconds(2));

            if (!slam->isMapping()) {
                std::cout << "建图已保存!" << std::endl;
            } else {
                std::cout << "建图保存可能仍在进行中..." << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "异常: " << e.what() << std::endl;
        }
    }

    /**
     * @brief 显示建图状态
     */
    void showMappingStatus() {
        if (!connected) {
            std::cout << "请先连接到机器人。" << std::endl;
            return;
        }

        std::cout << "\n=== 建图状态 ===" << std::endl;

        SLAMWorkMode mode = slam->getWorkMode();
        SLAMErrorCode error = slam->getErrorCode();

        std::cout << "工作模式: " << getWorkModeString(mode) << std::endl;
        std::cout << "状态: " << getErrorCodeString(error) << std::endl;
        std::cout << "建图中: " << (slam->isMapping() ? "是" : "否") << std::endl;

        // 显示建图路径点
        auto path_points = slam->getMappingPathPoints();
        if (!path_points.empty()) {
            std::cout << "\n建图路径点数量: " << path_points.size() << std::endl;
            std::cout << "最新5个路径点:" << std::endl;
            size_t count = std::min(size_t(5), path_points.size());
            for (size_t i = path_points.size() - count; i < path_points.size(); ++i) {
                std::cout << "  [" << i << "] x: " << std::fixed << std::setprecision(3)
                          << path_points[i].x << ", y: " << path_points[i].y
                          << ", z: " << path_points[i].z << std::endl;
            }
        }
    }

    // ============ 定位功能 ============

    /**
     * @brief 开启定位
     */
    void startLocalization() {
        if (!connected) {
            std::cout << "请先连接到机器人。" << std::endl;
            return;
        }

        if (slam->isLocalized()) {
            std::cout << "定位已开启。" << std::endl;
            return;
        }

        std::string scene_name;

        std::cout << "\n=== 开启定位 ===" << std::endl;
        std::cout << "请输入场景名称: ";
        std::getline(std::cin, scene_name);

        if (scene_name.empty()) {
            std::cout << "场景名称不能为空!" << std::endl;
            return;
        }

        try {
            slam->startLocalization(scene_name);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            if (slam->isLocalized()) {
                std::cout << "定位已开启! 场景名称: " << scene_name << std::endl;
            } else {
                SLAMErrorCode error = slam->getErrorCode();
                std::cout << "定位启动失败! 错误: " << getErrorCodeString(error) << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "异常: " << e.what() << std::endl;
        }
    }

    /**
     * @brief 关闭定位
     */
    void stopLocalization() {
        if (!connected) {
            std::cout << "请先连接到机器人。" << std::endl;
            return;
        }

        if (!slam->isLocalized()) {
            std::cout << "定位未开启。" << std::endl;
            return;
        }

        std::string scene_name;

        std::cout << "\n=== 关闭定位 ===" << std::endl;
        std::cout << "请输入场景名称: ";
        std::getline(std::cin, scene_name);

        try {
            slam->stopLocalization(scene_name);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            if (!slam->isLocalized()) {
                std::cout << "定位已关闭。" << std::endl;
            } else {
                std::cout << "定位关闭可能未完成..." << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "异常: " << e.what() << std::endl;
        }
    }

    /**
     * @brief 显示定位状态
     */
    void showLocalizationStatus() {
        if (!connected) {
            std::cout << "请先连接到机器人。" << std::endl;
            return;
        }

        std::cout << "\n=== 定位状态 ===" << std::endl;

        SLAMWorkMode mode = slam->getWorkMode();
        SLAMErrorCode error = slam->getErrorCode();

        std::cout << "工作模式: " << getWorkModeString(mode) << std::endl;
        std::cout << "状态: " << getErrorCodeString(error) << std::endl;
        std::cout << "定位中: " << (slam->isLocalized() ? "是" : "否") << std::endl;

        // 显示定位信息
        LocalizationInfo info = slam->getLocalizationInfo();
        std::cout << "\n定位信息:" << std::endl;
        std::cout << "  位置: x=" << std::fixed << std::setprecision(3) << info.position_x
                  << ", y=" << info.position_y
                  << ", z=" << info.position_z << std::endl;
        std::cout << "  姿态: qw=" << info.orientation_w
                  << ", qx=" << info.orientation_x
                  << ", qy=" << info.orientation_y
                  << ", qz=" << info.orientation_z << std::endl;
        std::cout << "  激光质量: " << info.laser_quality << std::endl;
    }

    // ============ 轨迹录制功能 ============

    /**
     * @brief 开始轨迹录制
     */
    void startRecording() {
        if (!connected) {
            std::cout << "请先连接到机器人。" << std::endl;
            return;
        }

        if (!slam->isLocalized()) {
            std::cout << "请先开启定位功能才能开始录制!" << std::endl;
            return;
        }

        SLAMWorkMode mode = slam->getWorkMode();
        if (mode == SLAMWorkMode::MAPPING) {
            std::cout << "建图模式下无法录制轨迹，请先完成建图。" << std::endl;
            return;
        }

        std::cout << "\n=== 开始轨迹录制 ===" << std::endl;

        try {
            // 设置录制事件回调
            slam->subscribeRecordingEvent([](RecordResult result) {
                switch (result) {
                    case RecordResult::POINT_ADDED:
                        std::cout << "[事件] 路径点已添加" << std::endl;
                        break;
                    case RecordResult::SUCCESS:
                        std::cout << "[事件] 录制成功完成" << std::endl;
                        break;
                    case RecordResult::FAIL:
                        std::cout << "[事件] 录制失败" << std::endl;
                        break;
                }
            });

            slam->startRecording();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            std::cout << "轨迹录制已开始!" << std::endl;
            std::cout << "提示: 移动机器人并添加路径点..." << std::endl;
        } catch (const std::exception& e) {
            std::cout << "异常: " << e.what() << std::endl;
        }
    }

    /**
     * @brief 添加路径点
     */
    void addPathPoint() {
        if (!connected) {
            std::cout << "请先连接到机器人。" << std::endl;
            return;
        }

        if (!slam->isLocalized()) {
            std::cout << "请先开启定位并开始录制才能添加路径点!" << std::endl;
            return;
        }

        std::cout << "\n=== 添加路径点 ===" << std::endl;

        try {
            slam->addPathPoint();
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            std::cout << "路径点添加指令已发送。" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "异常: " << e.what() << std::endl;
        }
    }

    /**
     * @brief 结束轨迹录制
     */
    void finishRecording() {
        if (!connected) {
            std::cout << "请先连接到机器人。" << std::endl;
            return;
        }

        std::cout << "\n=== 结束轨迹录制 ===" << std::endl;
        std::cout << "正在保存轨迹..." << std::endl;

        try {
            slam->finishRecording();
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout << "轨迹录制已结束并保存。" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "异常: " << e.what() << std::endl;
        }
    }

    // ============ 主菜单 ============

    /**
     * @brief 显示主菜单
     */
    void showMainMenu() {
        std::cout << "\n========================================" << std::endl;
        std::cout << "       Q25 SDK - 定位导航示例程序       " << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "连接状态: " << (connected ? "已连接" : "未连接") << std::endl;

        if (connected) {
            SLAMWorkMode mode = slam->getWorkMode();
            SLAMErrorCode error = slam->getErrorCode();
            std::cout << "SLAM状态: " << getWorkModeString(mode)
                      << " (" << getErrorCodeString(error) << ")" << std::endl;
        }

        std::cout << "\n主菜单:" << std::endl;
        std::cout << "  1. 连接机器人" << std::endl;
        std::cout << "  2. 断开连接" << std::endl;
        std::cout << "\n  --- 建图功能 ---" << std::endl;
        std::cout << "  11. 开始建图" << std::endl;
        std::cout << "  12. 保存建图" << std::endl;
        std::cout << "  13. 查看建图状态" << std::endl;
        std::cout << "\n  --- 定位功能 ---" << std::endl;
        std::cout << "  21. 开启定位" << std::endl;
        std::cout << "  22. 关闭定位" << std::endl;
        std::cout << "  23. 查看定位状态" << std::endl;
        std::cout << "\n  --- 轨迹录制 ---" << std::endl;
        std::cout << "  31. 开始轨迹录制" << std::endl;
        std::cout << "  32. 添加路径点" << std::endl;
        std::cout << "  33. 结束轨迹录制" << std::endl;
        std::cout << "\n  0. 退出程序" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "请输入选项: ";
    }

    /**
     * @brief 运行主循环
     */
    void run() {
        int choice;

        while (true) {
            showMainMenu();
            std::cin >> choice;
            clearInputBuffer();

            switch (choice) {
                // 连接管理
                case 1: {
                    std::string ip;
                    std::cout << "\n请输入机器人IP地址 (默认: 192.168.1.100): ";
                    std::getline(std::cin, ip);
                    if (ip.empty()) {
                        ip = "192.168.1.100";
                    }
                    connectToRobot(ip);
                    break;
                }
                case 2:
                    disconnect();
                    break;

                // 建图功能
                case 11:
                    startMapping();
                    break;
                case 12:
                    finishMapping();
                    break;
                case 13:
                    showMappingStatus();
                    break;

                // 定位功能
                case 21:
                    startLocalization();
                    break;
                case 22:
                    stopLocalization();
                    break;
                case 23:
                    showLocalizationStatus();
                    break;

                // 轨迹录制
                case 31:
                    startRecording();
                    break;
                case 32:
                    addPathPoint();
                    break;
                case 33:
                    finishRecording();
                    break;

                // 退出
                case 0:
                    std::cout << "\n正在退出程序..." << std::endl;
                    disconnect();
                    std::cout << "程序已退出。" << std::endl;
                    return;

                default:
                    std::cout << "\n无效选项，请重新输入。" << std::endl;
                    break;
            }

            // 等待用户按键继续
            if (choice != 0) {
                std::cout << "\n按 Enter 键继续...";
                std::cin.get();
            }
        }
    }
};

/**
 * @brief 主函数
 */
int main() {
    std::cout << "Q25 SDK - 定位导航功能示例程序" << std::endl;
    std::cout << "Version 1.0.0" << std::endl;
    std::cout << "本程序演示SLAM建图、定位、轨迹录制功能的交互式使用。" << std::endl;

    MappingCLI cli;
    cli.run();

    return 0;
}
