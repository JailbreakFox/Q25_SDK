/**
 * @file main.cpp
 * @brief Q25 SDK - 定位导航功能示例程序
 *
 * 本程序演示了SLAM建图、定位、场景管理、地图管理功能的交互式使用流程
 */

#include <robot/q25/quadruped_sdk.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <iomanip>
#include <limits>
#include <atomic>
#include <mutex>

#ifndef WIN32
#include <unistd.h>
#endif

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
    PointNavigation* point_navigation; // 定点导航实例
    bool connected;
    bool recording;                    // 轨迹录制状态
    std::string recording_scene_name;  // 当前录制关联的场景名称
    std::thread* monitor_thread;      // 连接监控线程
    std::atomic<bool> should_monitor;  // 是否继续监控
    mutable std::mutex robot_mutex;    // 保护 robot 和 connected 的互斥锁

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
    MappingCLI() : robot(nullptr), slam(nullptr), map_manager(nullptr), point_navigation(nullptr),
                   connected(false), recording(false), recording_scene_name(""),
                   monitor_thread(nullptr), should_monitor(false) {
    }

    ~MappingCLI() {
        // 停止监控线程
        if (monitor_thread) {
            should_monitor = false;
            if (monitor_thread->joinable()) {
                monitor_thread->join();
            }
            delete monitor_thread;
            monitor_thread = nullptr;
        }

        if (slam) delete slam;
        if (map_manager) delete map_manager;
        if (point_navigation) delete point_navigation;
        if (robot) {
            delete robot;
            robot = nullptr;
        }
    }

    // ============ 连接管理 ============

    /**
     * @brief 连接监控线程函数
     * 持续检查连接状态并更新全局变量
     */
    void monitorConnection() {
        while (should_monitor) {
            {
                std::lock_guard<std::mutex> lock(robot_mutex);
                if (robot) {
                    connected = robot->isConnected();
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }

    /**
     * @brief 线程安全地检查连接状态
     */
    bool isConnected() const {
        std::lock_guard<std::mutex> lock(robot_mutex);
        return connected;
    }

    /**
     * @brief 连接到机器人
     */
    void connectToRobot(const std::string& ip) {
        if (isConnected() || monitor_thread) {
            std::cout << "已连接到机器人，请先断开连接。" << std::endl;
            return;
        }

        std::cout << "正在连接到机器人 " << ip << "..." << std::endl;
        std::cout << "[DEBUG] 创建 Robot 对象..." << std::endl;
        robot = new Robot(ip);

        try {
            std::cout << "[DEBUG] 调用 robot->connect()..." << std::endl;
            robot->connect();
            std::cout << "[DEBUG] connect() 返回成功" << std::endl;

            // 创建 SLAM 和 MapManager
            std::cout << "[DEBUG] 创建 SLAM..." << std::endl;
            if (!slam) {
                slam = new SLAM();
            }
            std::cout << "[DEBUG] SLAM 创建完成" << std::endl;

            std::cout << "[DEBUG] 创建 MapManager..." << std::endl;
            if (!map_manager) {
                map_manager = new MapManager();
            }
            std::cout << "[DEBUG] MapManager 创建完成" << std::endl;

            // 创建 PointNavigation
            std::cout << "[DEBUG] 创建 PointNavigation..." << std::endl;
            if (!point_navigation) {
                point_navigation = new PointNavigation();
            }
            std::cout << "[DEBUG] PointNavigation 创建完成" << std::endl;

            // 订阅录制事件回调
            slam->subscribeRecordingEvent([this](RecordResult result) {
                std::cout << "\n[录制事件] " << getRecordResultString(result) << std::endl;
                if (result == RecordResult::SUCCESS) {
                    recording = false;
                    std::cout << "轨迹录制已完成并保存。" << std::endl;
                } else if (result == RecordResult::FAIL) {
                    recording = false;
                    std::cout << "轨迹录制失败!" << std::endl;
                }
            });

            // 启动监控线程，持续检查连接状态
            should_monitor = true;
            monitor_thread = new std::thread(&MappingCLI::monitorConnection, this);
        } catch (const std::exception& e) {
            std::cout << "连接异常: " << e.what() << std::endl;
            delete robot;
            robot = nullptr;
        } catch (...) {
            std::cout << "连接异常: 未知错误" << std::endl;
            delete robot;
            robot = nullptr;
        }
    }

    /**
     * @brief 断开机器人连接
     */
    void disconnect() {
		try {
			// 停止监控线程
			if (monitor_thread) {
				should_monitor = false;
				if (monitor_thread->joinable()) {
					monitor_thread->join();
				}
				delete monitor_thread;
				monitor_thread = nullptr;
			}

			{
				std::lock_guard<std::mutex> lock(robot_mutex);
				if (!connected && !robot) {
					return;
				}

				if (robot) {
					robot->disconnect();
					delete robot;
					robot = nullptr;
				}

				connected = false;
			}

			// 断开连接时释放 SLAM、MapManager 和 PointNavigation
			if (slam) {
				delete slam;
				slam = nullptr;
			}
			if (map_manager) {
				delete map_manager;
				map_manager = nullptr;
			}
			if (point_navigation) {
				delete point_navigation;
				point_navigation = nullptr;
			}

			std::cout << "已断开机器人连接。" << std::endl;
		} catch (const std::exception& e) {
		}
    }

    // ============ 建图功能 ============

    /**
     * @brief 开始建图
     */
    void startMapping() {
        if (!isConnected()) {
            std::cout << "请先连接到机器人。" << std::endl;
            return;
        }

        // 检查是否已经在建图中
        if (slam->isMapping()) {
            std::cout << "\n警告：当前正在建图过程中！" << std::endl;
            std::cout << "重新开始建图将终止当前建图场景，当前进度将丢失。" << std::endl;
            std::cout << "确定要重新开始建图吗？(y/n): ";
            char confirm;
            std::cin >> confirm;
            clearInputBuffer();

            if (confirm != 'y' && confirm != 'Y') {
                std::cout << "已取消重新建图，继续当前建图。" << std::endl;
                return;
            }
            std::cout << "将终止当前建图并开始新的建图..." << std::endl;
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
            std::this_thread::sleep_for(std::chrono::milliseconds(3000));

            if (slam->isMapping()) {
                std::cout << "建图已开始! 场景名称: " << scene_name
                          << ", 场景类型: " << getSceneTypeString(type) << std::endl;
            } else {
                SLAMErrorCode error = slam->getErrorCode();
				if (robot::q25::SLAMErrorCode::NORMAL != error)
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
        if (!isConnected()) {
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
            std::this_thread::sleep_for(std::chrono::seconds(10));

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
        if (!isConnected()) {
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
        if (!isConnected()) {
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
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));

            if (slam->isLocalized()) {
                std::cout << "定位已开启! 场景名称: " << scene_name << std::endl;
            } else {
                SLAMErrorCode error = slam->getErrorCode();
				if (robot::q25::SLAMErrorCode::NORMAL != error)
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
        if (!isConnected()) {
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
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));

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
        if (!isConnected()) {
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

    // ============ 场景管理功能 ============

    /**
     * @brief 输入场景名称
     */
    std::string inputSceneName() {
        std::string name;
        std::cout << "请输入场景名称: ";
        std::getline(std::cin, name);
        return name;
    }

    /**
     * @brief 输入下载路径
     */
    std::string inputDownloadPath() {
        std::string path;
        std::cout << "请输入下载路径 (默认: ./): ";
        std::getline(std::cin, path);
        if (path.empty()) {
            path = "./";
        }
        return path;
    }

    /**
     * @brief 输入路径名称
     */
    std::string inputPathName() {
        std::string name;
        std::cout << "请输入路径名称: ";
        std::getline(std::cin, name);
        return name;
    }

    /**
     * @brief 获取录制结果字符串
     */
    std::string getRecordResultString(RecordResult result) const {
        switch (result) {
            case RecordResult::POINT_ADDED: return "路径点添加成功";
            case RecordResult::SUCCESS:      return "录制完成";
            case RecordResult::FAIL:         return "录制失败";
            default:                         return "未知";
        }
    }

    /**
     * @brief 获取导航状态字符串
     */
    std::string getNavigationStatusString(NavigationStatus status) const {
        switch (status) {
            case NavigationStatus::IDLE:      return "空闲";
            case NavigationStatus::RUNNING:   return "运行中";
            case NavigationStatus::FINISHED:  return "已完成";
            case NavigationStatus::CANCELLED: return "已取消";
            case NavigationStatus::FAILED:    return "失败";
            default:                          return "未知";
        }
    }

    /**
     * @brief 输入目标位姿
     */
    Pose inputTargetPose() {
        Pose pose;
        std::cout << "请输入目标坐标:" << std::endl;
        std::cout << "  X: ";
        std::cin >> pose.position.x;
        std::cout << "  Y: ";
        std::cin >> pose.position.y;
        pose.position.z = 0.0f;  // 默认地面高度

        // 默认朝向（无旋转）
        pose.orientation.x = 0.0f;
        pose.orientation.y = 0.0f;
        pose.orientation.z = 0.0f;
        pose.orientation.w = 1.0f;

        clearInputBuffer();
        return pose;
    }

    /**
     * @brief 刷新场景列表
     */
    void refreshScenesList() {
        if (!isConnected()) {
            std::cout << "请先连接到机器人。" << std::endl;
            return;
        }

        std::cout << "\n=== 刷新场景列表 ===" << std::endl;
        std::cout << "正在刷新场景列表..." << std::endl;

        try {
            map_manager->refreshScenes();
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout << "场景列表已刷新。" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "异常: " << e.what() << std::endl;
        }
    }

    /**
     * @brief 查看场景列表
     */
    void showScenesList() {
        if (!isConnected()) {
            std::cout << "请先连接到机器人。" << std::endl;
            return;
        }

        std::cout << "\n=== 场景列表 ===" << std::endl;

        try {
            auto scenes = map_manager->getScenes();
            if (scenes.empty()) {
                std::cout << "当前没有场景。" << std::endl;
            } else {
                std::cout << "场景数量: " << scenes.size() << std::endl;
                for (size_t i = 0; i < scenes.size(); ++i) {
                    std::cout << "  [" << (i + 1) << "] " << scenes[i] << std::endl;
                }
            }
        } catch (const std::exception& e) {
            std::cout << "异常: " << e.what() << std::endl;
        }
    }

    /**
     * @brief 查看场景详情
     */
    void showSceneDetails() {
        if (!isConnected()) {
            std::cout << "请先连接到机器人。" << std::endl;
            return;
        }

        std::string scene_name = inputSceneName();
        if (scene_name.empty()) {
            std::cout << "场景名称不能为空!" << std::endl;
            return;
        }

        std::cout << "\n=== 场景详情: " << scene_name << " ===" << std::endl;

        try {
            auto details = map_manager->getScenesDetail(scene_name);
            if (details.sub_scenes.empty()) {
                std::cout << "场景 '" << scene_name << "' 不存在或没有子场景。" << std::endl;
            } else {
                std::cout << "子场景数量: " << details.sub_scenes.size() << std::endl;
                for (const auto& sub_scene : details.sub_scenes) {
                    std::cout << "\n  [子场景 ID: " << sub_scene.sub_scene_id << "]" << std::endl;
                    std::cout << "    YAM 文件: " << sub_scene.yam_filename << std::endl;
                    std::cout << "    PGM 文件: " << sub_scene.pgm_filename << std::endl;
                }
            }
        } catch (const std::exception& e) {
            std::cout << "异常: " << e.what() << std::endl;
        }
    }

    /**
     * @brief 删除场景
     */
    void deleteScene() {
        if (!isConnected()) {
            std::cout << "请先连接到机器人。" << std::endl;
            return;
        }

        std::string scene_name = inputSceneName();
        if (scene_name.empty()) {
            std::cout << "场景名称不能为空!" << std::endl;
            return;
        }

        std::cout << "\n=== 删除场景 ===" << std::endl;
        std::cout << "确定要删除场景 '" << scene_name << "' 吗? (y/n): ";
        char confirm;
        std::cin >> confirm;
        clearInputBuffer();

        if (confirm != 'y' && confirm != 'Y') {
            std::cout << "已取消删除。" << std::endl;
            return;
        }

        try {
            map_manager->deleteScene(scene_name);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout << "场景删除指令已发送。" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "异常: " << e.what() << std::endl;
        }
    }

    /**
     * @brief 删除所有场景
     */
    void deleteAllScenes() {
        if (!isConnected()) {
            std::cout << "请先连接到机器人。" << std::endl;
            return;
        }

        std::cout << "\n=== 删除所有场景 ===" << std::endl;
        std::cout << "警告: 此操作将删除所有场景!" << std::endl;
        std::cout << "确定要继续吗? (y/n): ";
        char confirm;
        std::cin >> confirm;
        clearInputBuffer();

        if (confirm != 'y' && confirm != 'Y') {
            std::cout << "已取消删除。" << std::endl;
            return;
        }

        try {
            map_manager->deleteAllScenes();
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout << "删除所有场景指令已发送。" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "异常: " << e.what() << std::endl;
        }
    }

    // ============ 地图管理功能 ============

    /**
     * @brief 下载地图
     */
    void downloadMap() {
        if (!isConnected()) {
            std::cout << "请先连接到机器人。" << std::endl;
            return;
        }

        std::string scene_name = inputSceneName();
        if (scene_name.empty()) {
            std::cout << "场景名称不能为空!" << std::endl;
            return;
        }

        std::string download_path = inputDownloadPath();

        std::cout << "\n=== 下载地图 ===" << std::endl;
        std::cout << "场景名称: " << scene_name << std::endl;
        std::cout << "下载路径: " << download_path << std::endl;

        try {
            SceneDetail details = map_manager->getScenesDetail(scene_name);
            if (!details.sub_scenes.size()) {
                std::cout << "场景 '" << scene_name << "' 不存在或没有子场景。" << std::endl;
                return;
            }

            map_manager->downloadMap(scene_name, details.sub_scenes[0].sub_scene_id, download_path, nullptr);
            std::cout << "地图下载指令已发送，请等待下载完成..." << std::endl;
        } catch (const std::exception& e) {
        }
    }

    // ============ 轨迹录制功能 ============

    /**
     * @brief 开始录制轨迹
     */
    void startRecording() {
        if (!isConnected()) {
            std::cout << "请先连接到机器人。" << std::endl;
            return;
        }

        if (!slam->isLocalized()) {
            std::cout << "请先开启定位功能。" << std::endl;
            return;
        }

        if (recording) {
            std::cout << "当前正在录制中，请先结束当前录制。" << std::endl;
            return;
        }

        std::cout << "\n=== 开始轨迹录制 ===" << std::endl;
        std::cout << "轨迹录制需要在定位模式下进行。" << std::endl;
        std::cout << "录制过程中，机器人移动的轨迹将被记录。" << std::endl;

        std::string scene_name = inputSceneName();
        if (scene_name.empty()) {
            std::cout << "场景名称不能为空!" << std::endl;
            return;
        }

        try {
            slam->startRecording();
            recording = true;
            recording_scene_name = scene_name;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            if (recording) {
                std::cout << "轨迹录制已开始! 场景名称: " << scene_name << std::endl;
                std::cout << "移动机器人录制轨迹，完成后选择\"结束录制\"保存。" << std::endl;
            } else {
                std::cout << "录制启动失败!" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "异常: " << e.what() << std::endl;
            recording = false;
        }
    }

    /**
     * @brief 结束录制轨迹
     */
    void finishRecording() {
        if (!isConnected()) {
            std::cout << "请先连接到机器人。" << std::endl;
            return;
        }

        if (!recording) {
            std::cout << "当前没有正在进行的录制。" << std::endl;
            return;
        }

        std::cout << "\n=== 结束轨迹录制 ===" << std::endl;
        std::cout << "正在保存轨迹..." << std::endl;

        try {
            slam->finishRecording();
            std::cout << "录制结束指令已发送，请等待保存完成..." << std::endl;
            // 录制状态会在回调中更新
        } catch (const std::exception& e) {
            std::cout << "异常: " << e.what() << std::endl;
            recording = false;
        }
    }

    // ============ 轨迹管理功能 ============

    /**
     * @brief 刷新轨迹列表
     */
    void refreshTrajectories() {
        if (!isConnected()) {
            std::cout << "请先连接到机器人。" << std::endl;
            return;
        }

        std::cout << "\n=== 刷新轨迹列表 ===" << std::endl;
        std::cout << "正在刷新轨迹列表..." << std::endl;

        try {
            map_manager->refreshTrajectories();
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout << "轨迹列表已刷新。" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "异常: " << e.what() << std::endl;
        }
    }

    /**
     * @brief 查看所有轨迹
     */
    void showAllTrajectories() {
        if (!isConnected()) {
            std::cout << "请先连接到机器人。" << std::endl;
            return;
        }

        std::cout << "\n=== 所有轨迹列表 ===" << std::endl;

        try {
            auto trajectories = map_manager->getNavigationTrajectories();
            if (trajectories.empty()) {
                std::cout << "当前没有轨迹。" << std::endl;
            } else {
                std::cout << "轨迹数量: " << trajectories.size() << std::endl;
                for (const auto& traj : trajectories) {
                    std::cout << "\n[轨迹 ID: " << traj.trajectory_id << "]" << std::endl;
                    std::cout << "  场景名称: " << traj.scene_name << std::endl;
                    std::cout << "  导航点数量: " << traj.waypoints.size() << std::endl;
                    std::cout << "  路径数量: " << traj.paths.size() << std::endl;

                    if (!traj.paths.empty()) {
                        std::cout << "  路径列表:" << std::endl;
                        for (const auto& path : traj.paths) {
                            std::cout << "    - [" << path.path_id << "] " << path.path_name
                                      << " (点数: " << path.points.size() << ")" << std::endl;
                        }
                    }
                }
            }
        } catch (const std::exception& e) {
            std::cout << "异常: " << e.what() << std::endl;
        }
    }

    /**
     * @brief 查看场景轨迹
     */
    void showSceneTrajectories() {
        if (!isConnected()) {
            std::cout << "请先连接到机器人。" << std::endl;
            return;
        }

        std::string scene_name = inputSceneName();
        if (scene_name.empty()) {
            std::cout << "场景名称不能为空!" << std::endl;
            return;
        }

        std::cout << "\n=== 场景轨迹详情: " << scene_name << " ===" << std::endl;

        try {
            auto trajectory = map_manager->getNavigationTrajectory(scene_name);
            if (trajectory.paths.empty()) {
                std::cout << "场景 '" << scene_name << "' 没有轨迹。" << std::endl;
            } else {
                std::cout << "轨迹 ID: " << trajectory.trajectory_id << std::endl;
                std::cout << "场景名称: " << trajectory.scene_name << std::endl;
                std::cout << "导航点数量: " << trajectory.waypoints.size() << std::endl;
                std::cout << "路径数量: " << trajectory.paths.size() << std::endl;

                std::cout << "\n路径详情:" << std::endl;
                for (const auto& path : trajectory.paths) {
                    std::cout << "\n  [路径 ID: " << path.path_id << "] " << path.path_name << std::endl;
                    std::cout << "    路径点数量: " << path.points.size() << std::endl;

                    if (!path.points.empty() && path.points.size() <= 10) {
                        std::cout << "    路径点坐标:" << std::endl;
                        for (size_t i = 0; i < path.points.size(); ++i) {
                            const auto& pt = path.points[i];
                            std::cout << "      [" << i << "] x: " << std::fixed << std::setprecision(3)
                                      << pt.position.x << ", y: " << pt.position.y
                                      << ", z: " << pt.position.z << std::endl;
                        }
                    } else if (path.points.size() > 10) {
                        std::cout << "    (路径点过多，仅显示前5个和后5个)" << std::endl;
                        std::cout << "    前5个路径点:" << std::endl;
                        for (size_t i = 0; i < 5; ++i) {
                            const auto& pt = path.points[i];
                            std::cout << "      [" << i << "] x: " << std::fixed << std::setprecision(3)
                                      << pt.position.x << ", y: " << pt.position.y
                                      << ", z: " << pt.position.z << std::endl;
                        }
                        std::cout << "    后5个路径点:" << std::endl;
                        for (size_t i = path.points.size() - 5; i < path.points.size(); ++i) {
                            const auto& pt = path.points[i];
                            std::cout << "      [" << i << "] x: " << std::fixed << std::setprecision(3)
                                      << pt.position.x << ", y: " << pt.position.y
                                      << ", z: " << pt.position.z << std::endl;
                        }
                    }
                }
            }
        } catch (const std::exception& e) {
            std::cout << "异常: " << e.what() << std::endl;
        }
    }

    /**
     * @brief 删除场景轨迹
     */
    void deleteSceneTrajectories() {
        if (!isConnected()) {
            std::cout << "请先连接到机器人。" << std::endl;
            return;
        }

        std::string scene_name = inputSceneName();
        if (scene_name.empty()) {
            std::cout << "场景名称不能为空!" << std::endl;
            return;
        }

        std::cout << "\n=== 删除场景轨迹 ===" << std::endl;
        std::cout << "确定要删除场景 '" << scene_name << "' 的所有轨迹吗? (y/n): ";
        char confirm;
        std::cin >> confirm;
        clearInputBuffer();

        if (confirm != 'y' && confirm != 'Y') {
            std::cout << "已取消删除。" << std::endl;
            return;
        }

        try {
            map_manager->deleteNavigationTrajectory(scene_name);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout << "场景轨迹删除指令已发送。" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "异常: " << e.what() << std::endl;
        }
    }

    /**
     * @brief 删除指定路径
     */
    void deleteTrajectoryPath() {
        if (!isConnected()) {
            std::cout << "请先连接到机器人。" << std::endl;
            return;
        }

        std::string scene_name = inputSceneName();
        if (scene_name.empty()) {
            std::cout << "场景名称不能为空!" << std::endl;
            return;
        }

        std::string path_name = inputPathName();
        if (path_name.empty()) {
            std::cout << "路径名称不能为空!" << std::endl;
            return;
        }

        std::cout << "\n=== 删除轨迹路径 ===" << std::endl;
        std::cout << "场景: " << scene_name << std::endl;
        std::cout << "路径: " << path_name << std::endl;
        std::cout << "确定要删除此路径吗? (y/n): ";
        char confirm;
        std::cin >> confirm;
        clearInputBuffer();

        if (confirm != 'y' && confirm != 'Y') {
            std::cout << "已取消删除。" << std::endl;
            return;
        }

        try {
            map_manager->deleteNavigationPath(scene_name, path_name);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout << "路径删除指令已发送。" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "异常: " << e.what() << std::endl;
        }
    }

    // ============ 定点导航功能 ============

    /**
     * @brief 导航到目标点
     */
    void navigateToTarget() {
        if (!isConnected()) {
            std::cout << "请先连接到机器人。" << std::endl;
            return;
        }

        if (!point_navigation) {
            std::cout << "导航模块未初始化。" << std::endl;
            return;
        }

        if (!slam->isLocalized()) {
            std::cout << "请先开启定位功能。" << std::endl;
            return;
        }

        std::cout << "\n=== 导航到目标点 ===" << std::endl;

        std::string scene_name = inputSceneName();
        if (scene_name.empty()) {
            std::cout << "场景名称不能为空!" << std::endl;
            return;
        }

        Pose target_pose = inputTargetPose();

        std::cout << "\n场景: " << scene_name << std::endl;
        std::cout << "目标坐标: (" << target_pose.position.x << ", "
                  << target_pose.position.y << ", "
                  << target_pose.position.z << ")" << std::endl;

        try {
            bool success = point_navigation->navigateToPose(scene_name, target_pose);
            if (success) {
                std::cout << "导航任务已启动!" << std::endl;
                std::cout << "使用\"查看导航状态\"选项查看导航进度。" << std::endl;
            } else {
                std::cout << "导航任务启动失败!" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "异常: " << e.what() << std::endl;
        }
    }

    /**
     * @brief 查看导航状态
     */
    void showNavigationStatus() {
        if (!isConnected()) {
            std::cout << "请先连接到机器人。" << std::endl;
            return;
        }

        if (!point_navigation) {
            std::cout << "导航模块未初始化。" << std::endl;
            return;
        }

        std::cout << "\n=== 导航状态 ===" << std::endl;

        try {
            NavigationStatus status = point_navigation->getNavigationStatus();
            std::cout << "当前状态: " << getNavigationStatusString(status) << std::endl;

            if (status == NavigationStatus::RUNNING) {
                std::cout << "导航任务正在执行中..." << std::endl;
            } else if (status == NavigationStatus::FINISHED) {
                std::cout << "导航任务已完成!" << std::endl;
            } else if (status == NavigationStatus::FAILED) {
                std::cout << "导航任务失败!" << std::endl;
            } else if (status == NavigationStatus::CANCELLED) {
                std::cout << "导航任务已取消。" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "异常: " << e.what() << std::endl;
        }
    }

    /**
     * @brief 取消导航
     */
    void cancelNavigation() {
        if (!isConnected()) {
            std::cout << "请先连接到机器人。" << std::endl;
            return;
        }

        if (!point_navigation) {
            std::cout << "导航模块未初始化。" << std::endl;
            return;
        }

        std::cout << "\n=== 取消导航 ===" << std::endl;
        std::cout << "确定要取消当前导航任务吗? (y/n): ";
        char confirm;
        std::cin >> confirm;
        clearInputBuffer();

        if (confirm != 'y' && confirm != 'Y') {
            std::cout << "已取消。" << std::endl;
            return;
        }

        try {
            bool success = point_navigation->cancelNavigation();
            if (success) {
                std::cout << "导航任务已取消。" << std::endl;
            } else {
                std::cout << "取消导航失败!" << std::endl;
            }
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
        std::cout << "连接状态: " << (isConnected() ? "已连接" : "未连接") << std::endl;

        if (isConnected() && slam) {
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
        std::cout << "\n  --- 场景管理 ---" << std::endl;
        std::cout << "  31. 刷新场景列表" << std::endl;
        std::cout << "  32. 查看场景列表" << std::endl;
        std::cout << "  33. 查看场景详情" << std::endl;
        std::cout << "  34. 删除场景" << std::endl;
        std::cout << "  35. 删除所有场景" << std::endl;
        std::cout << "\n  --- 地图管理 ---" << std::endl;
        std::cout << "  41. 下载地图" << std::endl;
        std::cout << "\n  --- 轨迹录制 ---" << std::endl;
        std::cout << "  51. 开始录制" << std::endl;
        std::cout << "  52. 结束录制" << std::endl;
        std::cout << "\n  --- 轨迹管理 ---" << std::endl;
        std::cout << "  61. 刷新轨迹列表" << std::endl;
        std::cout << "  62. 查看所有轨迹" << std::endl;
        std::cout << "  63. 查看场景轨迹" << std::endl;
        std::cout << "  64. 删除场景轨迹" << std::endl;
        std::cout << "  65. 删除指定路径" << std::endl;
        std::cout << "\n  --- 定点导航 ---" << std::endl;
        std::cout << "  71. 导航到目标点" << std::endl;
        std::cout << "  72. 查看导航状态" << std::endl;
        std::cout << "  73. 取消导航" << std::endl;
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
                    std::cout << "\n请输入机器人IP地址 (默认: 192.168.1.103): ";
                    std::getline(std::cin, ip);
                    if (ip.empty()) {
                        ip = "192.168.1.103";
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

                // 场景管理
                case 31:
                    refreshScenesList();
                    break;
                case 32:
                    showScenesList();
                    break;
                case 33:
                    showSceneDetails();
                    break;
                case 34:
                    deleteScene();
                    break;
                case 35:
                    deleteAllScenes();
                    break;

                // 地图管理
                case 41:
                    downloadMap();
                    break;

                // 轨迹录制
                case 51:
                    startRecording();
                    break;
                case 52:
                    finishRecording();
                    break;

                // 轨迹管理
                case 61:
                    refreshTrajectories();
                    break;
                case 62:
                    showAllTrajectories();
                    break;
                case 63:
                    showSceneTrajectories();
                    break;
                case 64:
                    deleteSceneTrajectories();
                    break;
                case 65:
                    deleteTrajectoryPath();
                    break;

                // 定点导航
                case 71:
                    navigateToTarget();
                    break;
                case 72:
                    showNavigationStatus();
                    break;
                case 73:
                    cancelNavigation();
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
#ifdef _WIN32
    // 设置 Windows 控制台为 UTF-8 编码
    system("chcp 65001 > nul");
#endif

    std::cout << "Q25 SDK - 定位导航功能示例程序" << std::endl;
    std::cout << "Version " << getSDKVersion() << std::endl;
    std::cout << "本程序演示SLAM建图、定位、场景管理、地图管理功能的交互式使用。" << std::endl;

    MappingCLI cli;
    cli.run();

    return 0;
}
