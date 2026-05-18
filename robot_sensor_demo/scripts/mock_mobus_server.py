from pymodbus.server import StartTcpServer
from pymodbus.datastore import ModbusSequentialDataBlock, ModbusSlaveContext, ModbusServerContext
import random
import threading
import time

store = ModbusSlaveContext(
    hr=ModbusSequentialDataBlock(0, [0]*10)
)
context = ModbusServerContext(slaves=store, single=True)

def update_register():
    while True:
        distance_mm = int(random.uniform(200, 5000))  # 0.2m–5m
        context[0].setValues(3, 0, [distance_mm])
        time.sleep(0.1)

threading.Thread(target=update_register, daemon=True).start()

print("Mock Modbus server running on port 5020...")
StartTcpServer(context=context, address=("0.0.0.0", 5020))