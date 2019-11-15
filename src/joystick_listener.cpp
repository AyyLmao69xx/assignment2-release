// Copyright 2019 Zhihao Zhang License MIT

#include "joystick_listener.hpp"

#include "student_helper.hpp"

#include <memory>
#include <string>
#include <utility>

namespace assignment2
{
JoystickListener::JoystickListener(
    std::string const & zid, JoystickConfig config)
    : rclcpp::Node{helper::joy_node_name(zid)}
    , config_{}
    , zid_{zid}
{
    { // Subscriber
        this->joystick_input_ =
            create_subscription<sensor_msgs::msg::Joy>(
				"/"+zid+"/joy", 
				10,
                [this](sensor_msgs::msg::Joy::UniquePtr joy_message){
                    joy_message_callback(std::move(joy_message));
                });
        // Publisher
        this->acceleration_output_ =
            create_publisher<geometry_msgs::msg::AccelStamped>(
                "/"+zid+"/acceleration", 
				10);
    }	
}
// Function takes axes as inuts and puts them through series of if statements to 
// find and remove appropriate deadzone
double fixDeadzone(size_t axisType, double deadzone){
    double accel;
    // Removes specified deadzone
    if (axisType => deadzone){
            accel = mapFunction(axisType, deadzone, 1.0, 0.0, 1.0);
    } else if(axisType <= -deadzone){
			accel = mapFunciton(axisType, -deadzone, -1.0, 0.0, -1.0);
    } else{
			accel = 0;
    }
    return accel;
}
// Mapping Function- returns equation of a straight line using y=mx+b
double mapFunction(
    double x, double lowerIn, double upperIn, double lowerOut, double upperOut){
    double y = (upperOut - lowerOut) / (upperIn - lowerIn) * x + (upperOut -
               (upperIn * (upperOut - lowerOut) / (upperIn - lowerIn)));
    return y;
}


// ReSharper disable once CppMemberFunctionMayBeConst
auto JoystickListener::joy_message_callback(
    sensor_msgs::msg::Joy::UniquePtr joy_message) -> void{
    // Task 1A- print values
    std::for_each(joy_message->axes.begin(), 
		joy_message->axes.end(),
        [](float & n){ std::cout << n << '\t'; });
    std::cout << '\n';

    std::cout << "Total number of buttons pressed is "
              << std::count(joy_message->buttons.begin(),
                     joy_message->buttons.end(), 1)
              << '\n';
}	// Task 1B- map acceleration values
 
	// Put all the different axes through the deadzone if statement corrections
	double positive_linear_acceleration =
		mapFunction(speed_plus_axis_corrected, -1.0, 1.0, 0.0, 1.0);
	double negative_linear_acceleration =
		mapFunction(speed_minus_axis_corrected, -1.0, 1.0, 0.0, -1.0);
	double steering_axis_corrected = fixDeadzone(
		joy_message->axes[config_.steering_axis], config_.steering_deadzone);
	double speed_plus_axis_corrected = fixDeadzone(
		joy_message->axes[config_.speed_plus_axis], config_.speed_deadzone);
	double net_linear_acceleration =
		positive_linear_acceleration + negative_linear_acceleration;
	double speed_minus_axis_corrected = fixDeadzone(
		joy_message->axes[config_.speed_minus_axis], config_.speed_deadzone);
	double angular_acceleration = steering_axis_corrected;

	auto acceleration_stamped_data = geometry_msgs::msg::AccelStamped();

	acceleration_stamped_data.accel.linear.x = net_linear_acceleration;
	acceleration_stamped_data.accel.angular.z = angular_acceleration;
	acceleration_stamped_data.header.stamp = joy_message->header.stamp;
	// Publisher- publish acceleration data
	acceleration_stamped_data.header.frame_id = zid;
	acceleration_output_->publish(acceleration_stamped_data);
} // namespace assignment2

