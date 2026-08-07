#include <cstdio>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
using std::placeholders::_1;

class STM32InterfaceNode : public rclcpp::Node {
 public:
    STM32InterfaceNode() : Node("stm32_interface_node") {
      subscription_ = this->create_subscription<std_msgs::msg::String>("camera/camera_info", 10, std::bind(&STM32InterfaceNode::topic_callback, this, _1));
    };

 private:
  void topic_callback(const std_msgs::msg::String::SharedPtr msg) const {
    RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg->data.c_str());
  }

  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char ** argv)
{
  printf("hello world robot_firmware package\n");

  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<STM32InterfaceNode>());
  rclcpp::shutdown();
  return 0;
}
