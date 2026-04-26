#include <chrono>
#include <random>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"

using namespace std::chrono_literals;

class DistanceSensorDriver : public rclcpp::Node
{
public:
    DistanceSensorDriver() : Node("distance_sensor_driver")
    {
        // Declare parameters
        this->declare_parameter("publish_rate", 10.0);
        this->declare_parameter("min_range", 0.2);
        this->declare_parameter("max_range", 5.0);
        this->declare_parameter("noise_stddev", 0.05);

        // Get parameters
        publish_rate_ = this->get_parameter("publish_rate").as_double();
        min_range_ = this->get_parameter("min_range").as_double();
        max_range_ = this->get_parameter("max_range").as_double();
        noise_stddev_ = this->get_parameter("noise_stddev").as_double();

        // QoS (sensor-like)
        auto qos = rclcpp::QoS(rclcpp::KeepLast(10)).best_effort();

        publisher_ = this->create_publisher<std_msgs::msg::Float32>("distance", qos);

        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(static_cast<int>(1000.0 / publish_rate_)),
            std::bind(&DistanceSensorDriver::publish_distance, this)
        );

        rng_ = std::mt19937(rd_());
        dist_ = std::uniform_real_distribution<>(min_range_, max_range_);
        noise_ = std::normal_distribution<>(0.0, noise_stddev_);

        RCLCPP_INFO(this->get_logger(), "Distance sensor driver started");
    }

private:
    void publish_distance()
    {
        float value = dist_(rng_) + noise_(rng_);

        std_msgs::msg::Float32 msg;
        msg.data = value;

        publisher_->publish(msg);

        RCLCPP_DEBUG(this->get_logger(), "Published distance: %.2f", value);
    }

    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;

    double publish_rate_;
    double min_range_;
    double max_range_;
    double noise_stddev_;

    std::random_device rd_;
    std::mt19937 rng_;
    std::uniform_real_distribution<> dist_;
    std::normal_distribution<> noise_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<DistanceSensorDriver>());
    rclcpp::shutdown();
    return 0;
}