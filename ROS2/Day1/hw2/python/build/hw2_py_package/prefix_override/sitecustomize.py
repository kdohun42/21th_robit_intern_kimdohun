import sys
if sys.prefix == '/usr':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = '/home/kdh/Desktop/21th_robit_intern_kimdohun/ROS2/Day1/hw2/python/install/hw2_py_package'
