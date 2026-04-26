#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"
#include "std_msgs/msg/bool.hpp"
#include "geometry_msgs/msg/twist.hpp"

class ObstacleAvoidanceNode : public rclcpp::Node
{
public:
    ObstacleAvoidanceNode() : Node("obstacle_avoidance_node")
    {
        this->declare_parameter("obstacle_threshold", 1.0);
        this->declare_parameter("forward_speed", 0.5);

        threshold_ = this->get_parameter("obstacle_threshold").as_double();
        speed_ = this->get_parameter("forward_speed").as_double();

        // subscription_ = this->create_subscription<std_msgs::msg::Float32>(
        //     "distance", 10,
        //     std::bind(&ObstacleAvoidanceNode::callback, this, std::placeholders::_1)
        // );
        auto qos = rclcpp::QoS(rclcpp::KeepLast(10)).best_effort();

        subscription_ = this->create_subscription<std_msgs::msg::Float32>(
            "distance", qos,
            std::bind(&ObstacleAvoidanceNode::callback, this, std::placeholders::_1)
        );

        cmd_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
        obstacle_pub_ = this->create_publisher<std_msgs::msg::Bool>("obstacle_detected", 10);

        RCLCPP_INFO(this->get_logger(), "Obstacle avoidance node started");
    }

private:
    void callback(const std_msgs::msg::Float32::SharedPtr msg)
    {
        float distance = msg->data;

        if (std::isnan(distance)) {
            RCLCPP_WARN(this->get_logger(), "Received NaN distance");
            return;
        }

        geometry_msgs::msg::Twist cmd;
        std_msgs::msg::Bool obstacle_msg;

        if (distance < threshold_) {
            cmd.linear.x = 0.0;
            obstacle_msg.data = true;
            RCLCPP_WARN(this->get_logger(), "Obstacle detected at %.2f m", distance);
        } else {
            cmd.linear.x = speed_;
            obstacle_msg.data = false;
        }

        cmd_pub_->publish(cmd);
        obstacle_pub_->publish(obstacle_msg);
    }

    double threshold_;
    double speed_;

    rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr subscription_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_pub_;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr obstacle_pub_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ObstacleAvoidanceNode>());
    rclcpp::shutdown();
    return 0;
}