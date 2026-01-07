#ifndef QUADRUPED_SDK_SYSTEM_INFO_HPP
#define QUADRUPED_SDK_SYSTEM_INFO_HPP

#include "../common/types.hpp"
#include <cstdint>
#include <string>

namespace robot {
namespace q25 {

/**
 * 版本信息
 * 对应 PublicInterface::RobotVersionInfo
 */
struct VersionInfo {
    std::string cms_version;        // CMS版本号
    std::string pms_version;        // PMS版本号
    std::string dcs_version;        // DCS版本号
    std::string pns_version;        // PNS版本号
    std::string acs_version;        // ACS版本号
    std::string pps_version;        // PPS版本号
    std::string pmpm_version;       // PMPM版本号
    std::string policy_version;     // 模型文件版本号
    std::string software_version;   // 本体软件版本号
};

/**
 * 运动统计信息
 * 对应 ACS::MotionStatistics
 */
struct MotionStatistics {
    float current_runtime;      // 当次开机后的运行时间 (秒)
    float total_runtime;        // 累计运行时间 (秒)
    float current_mileage;      // 当次开机后运行的里程数 (米)
    float total_mileage;        // 累计运行里程数 (米)
};

/**
 * 系统资源状态
 * 对应 PublicInterface::RobotResourceCheck
 */
struct ResourceStatus {
    uint32_t cpu_occupy;        // CPU占用率 (%)
    uint32_t memory_occupy;     // 内存占用率 (%)
    float cpu_temperature;      // CPU温度 (℃)
    float emmc_left_life;       // eMMC剩余寿命 (%)
    uint32_t cpu_frequency;     // CPU主频 (MHz)
};

/**
 * SystemInfo - 系统信息接口
 * 提供机器人系统信息访问功能
 * 可独立使用，不依赖其他模块
 */
class SystemInfo {
public:
    SystemInfo();
    ~SystemInfo();

    // 禁用复制
    SystemInfo(const SystemInfo&) = delete;
    SystemInfo& operator=(const SystemInfo&) = delete;

    // ============ 信息查询 ============

    /**
     * 获取版本信息
     * @return 各模块版本信息
     */
    VersionInfo getVersionInfo() const;

    /**
     * 获取机器人名称
     * @return 机器人名称
     */
    std::string getRobotName() const;

    /**
     * 获取运动统计信息
     * @return 运动统计数据
     */
    MotionStatistics getMotionStatistics() const;

    /**
     * 获取系统资源状态
     * @return 资源状态信息
     */
    ResourceStatus getResourceStatus() const;

private:
    struct Impl;
    Impl* impl_;
};

} // namespace q25
} // namespace robot

#endif // QUADRUPED_SDK_SYSTEM_INFO_HPP

