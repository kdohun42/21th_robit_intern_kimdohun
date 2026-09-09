// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from custom_interfaces:msg/MyMessage.idl
// generated code does not contain a copyright notice

#include "custom_interfaces/msg/detail/my_message__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_custom_interfaces
const rosidl_type_hash_t *
custom_interfaces__msg__MyMessage__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xb9, 0xf5, 0x92, 0xdd, 0x59, 0x32, 0x15, 0x80,
      0xa0, 0xf2, 0x07, 0xc2, 0x32, 0x32, 0xf9, 0xee,
      0x7c, 0xca, 0xb7, 0x2e, 0x3b, 0x88, 0xb0, 0xc2,
      0x82, 0xef, 0xc7, 0x7b, 0xd6, 0x96, 0x44, 0x02,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types

// Hashes for external referenced types
#ifndef NDEBUG
#endif

static char custom_interfaces__msg__MyMessage__TYPE_NAME[] = "custom_interfaces/msg/MyMessage";

// Define type names, field names, and default values
static char custom_interfaces__msg__MyMessage__FIELD_NAME__data[] = "data";

static rosidl_runtime_c__type_description__Field custom_interfaces__msg__MyMessage__FIELDS[] = {
  {
    {custom_interfaces__msg__MyMessage__FIELD_NAME__data, 4, 4},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT32_UNBOUNDED_SEQUENCE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
custom_interfaces__msg__MyMessage__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {custom_interfaces__msg__MyMessage__TYPE_NAME, 31, 31},
      {custom_interfaces__msg__MyMessage__FIELDS, 1, 1},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "int32[] data";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
custom_interfaces__msg__MyMessage__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {custom_interfaces__msg__MyMessage__TYPE_NAME, 31, 31},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 13, 13},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
custom_interfaces__msg__MyMessage__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *custom_interfaces__msg__MyMessage__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}
