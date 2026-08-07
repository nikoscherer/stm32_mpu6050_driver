import rclpy
from rclpy.node import Node

from std_msgs.msg import String

class USBCameraNode(Node):
    def __init__(self):
        super().__init__("usb_camera_node")
        self.publisher_ = self.create_publisher(String, "camera/camera_info", 10)
        timer_period = 0.5
        self.timer = self.create_timer(timer_period, self.timer_callback)

    def timer_callback(self):
        msg = String()
        msg.data = "Example data"
        self.publisher_.publish(msg)
        self.get_logger().info('Publishing: "%s"' % msg.data)

def main(args=None):
    print('Hi from robot_vision.')

    rclpy.init(args=args)

    usb_camera_node = USBCameraNode()

    rclpy.spin(usb_camera_node)

    rclpy.shutdown()


if __name__ == '__main__':
    main()
