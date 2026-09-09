#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};



// Corresponds to custom_interfaces__msg__MyMessage

// This struct is not documented.
#[allow(missing_docs)]

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct MyMessage {

    // This member is not documented.
    #[allow(missing_docs)]
    pub data: Vec<i32>,

}



impl Default for MyMessage {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::MyMessage::default())
  }
}

impl rosidl_runtime_rs::Message for MyMessage {
  type RmwMsg = super::msg::rmw::MyMessage;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        data: msg.data.into(),
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        data: msg.data.as_slice().into(),
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      data: msg.data
          .into_iter()
          .collect(),
    }
  }
}


