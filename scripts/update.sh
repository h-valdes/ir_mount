gnome-terminal -- /bin/bash -c "source /opt/ros/foxy/setup.bash; \
source ~/microros_ws/install/local_setup.bash; \
cd ~/microros_ws; \
ros2 run micro_ros_setup build_firmware.sh; \
ros2 run micro_ros_setup flash_firmware.sh" &
