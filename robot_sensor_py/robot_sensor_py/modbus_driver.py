import rclpy
from rclpy.node import Node
from std_msgs.msg import Float32
from pymodbus.client import ModbusTcpClient
import math

class ModbusDriver(Node):
    def __init__(self):
        super().__init__('modbus_driver')

        # Parameters
        self.declare_parameter('ip', '127.0.0.1')
        self.declare_parameter('port', 5020)
        self.declare_parameter('publish_rate', 10.0)

        ip = self.get_parameter('ip').value
        port = self.get_parameter('port').value
        rate = self.get_parameter('publish_rate').value

        self.client = ModbusTcpClient(ip, port=port)

        # QoS
        qos = rclpy.qos.QoSProfile(depth=10)
        qos.reliability = rclpy.qos.ReliabilityPolicy.BEST_EFFORT

        self.publisher = self.create_publisher(Float32, 'distance', qos)

        self.timer = self.create_timer(1.0 / rate, self.read_modbus)

        self.get_logger().info("Modbus driver started")

    def read_modbus(self):
        if not self.client.connect():
            self.get_logger().error("Failed to connect to Modbus server")
            return

        result = self.client.read_holding_registers(0, 1)

        if result.isError():
            self.get_logger().error("Modbus read error")
            return

        distance_mm = result.registers[0]
        distance_m = distance_mm / 1000.0

        if math.isnan(distance_m):
            return

        msg = Float32()
        msg.data = distance_m

        self.publisher.publish(msg)

        self.get_logger().info(f"Distance: {distance_m:.2f} m")

def main(args=None):
    rclpy.init(args=args)
    node = ModbusDriver()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()