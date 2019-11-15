// Copyright 2019 Zhihao Zhang License MIT

#include "velocity_kinematic.hpp"

#include "student_helper.hpp"

#include <cassert>
#include <memory>
#include <string>
#include <utility>

namespace assignment2
{
VelocityKinematic::VelocityKinematic(std::string const & zid,
    std::chrono::milliseconds const refresh_period, KinematicLimits config)
    : rclcpp::Node(helper::velocity_node_name(zid))
// TODO(STUDENT): CODE HERE
{
    // TODO(STUDENT): CODE HERE
}

auto VelocityKinematic::acceleration_callback(
    geometry_msgs::msg::AccelStamped::UniquePtr input_message) -> void
{
    // TODO(STUDENT): CODE HERE
}

auto VelocityKinematic::velocity_callback() -> void
{
		
}
} // namespace assignment2
