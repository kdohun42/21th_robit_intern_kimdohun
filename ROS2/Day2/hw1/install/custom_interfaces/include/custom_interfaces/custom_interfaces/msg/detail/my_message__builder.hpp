// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from custom_interfaces:msg/MyMessage.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "custom_interfaces/msg/my_message.hpp"


#ifndef CUSTOM_INTERFACES__MSG__DETAIL__MY_MESSAGE__BUILDER_HPP_
#define CUSTOM_INTERFACES__MSG__DETAIL__MY_MESSAGE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "custom_interfaces/msg/detail/my_message__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace custom_interfaces
{

namespace msg
{

namespace builder
{

class Init_MyMessage_data
{
public:
  Init_MyMessage_data()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::custom_interfaces::msg::MyMessage data(::custom_interfaces::msg::MyMessage::_data_type arg)
  {
    msg_.data = std::move(arg);
    return std::move(msg_);
  }

private:
  ::custom_interfaces::msg::MyMessage msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::custom_interfaces::msg::MyMessage>()
{
  return custom_interfaces::msg::builder::Init_MyMessage_data();
}

}  // namespace custom_interfaces

#endif  // CUSTOM_INTERFACES__MSG__DETAIL__MY_MESSAGE__BUILDER_HPP_
