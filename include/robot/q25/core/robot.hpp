#ifndef QUADRUPED_SDK_CORE_ROBOT_HPP
#define QUADRUPED_SDK_CORE_ROBOT_HPP

#include <string>
#include <memory>

namespace robot {
namespace q25 {

// 前向声明
class RobotImpl;

/**
 * Robot - 机器人SDK入口类
 * 提供连接管理和各功能模块的访问接口
 */
class Robot {
public:
    /**
     * 构造函数
     * @param robot_ip 机器人IP地址
     */
    explicit Robot(const std::string& robot_ip);

    ~Robot();

    // ============ 连接管理 ============

    /**
     * 连接到机器人
     * @return true表示连接成功
     */
    bool connect();

    /**
     * 断开与机器人的连接
     */
    void disconnect();

    /**
     * 检查是否已连接
     * @return true表示已连接
     */
    bool isConnected() const;

private:
    std::unique_ptr<RobotImpl> pImpl;
    
    // 禁用拷贝
    Robot(const Robot&) = delete;
    Robot& operator=(const Robot&) = delete;
};

} // namespace q25
} // namespace robot

#endif // QUADRUPED_SDK_CORE_ROBOT_HPP
