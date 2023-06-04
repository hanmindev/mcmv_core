#include <map>
#include "bvh_parser.h"
#include "../../math/euler.h"

const map<string, Order> order_map = {
    {"XZY", Order::xzy},
    {"XYZ", Order::xyz},
    {"YXZ", Order::yxz},
    {"YZX", Order::yzx},
    {"ZXY", Order::zxy},
    {"ZYX", Order::zyx}
};

BvhParser::BvhParser(BvhLexer *lexer) {
  this->lexer = lexer;
}
BvhParser::~BvhParser() {
  delete this->lexer;
}

void BvhParser::handle_joint() {
  update_token();
  if (this->current_token == tok_end) {
    update_token();
    if (this->current_token != tok_site) {
      throw std::runtime_error("Expected Site");
    }
    update_token();
    if (this->current_token != tok_l_brace) {
      throw std::runtime_error("Expected {");
    }
    update_token();
    if (this->current_token != tok_offset) {
      throw std::runtime_error("Expected OFFSET");
    }
    update_token();

    // eat three numbers
    update_token();
    update_token();
    update_token();
    if (this->current_token != tok_r_brace) {
      throw std::runtime_error("Expected }");
    }
  }

  if (this->current_token != tok_offset) {
    throw std::runtime_error("Expected OFFSET or End Site");
  }

  update_token();

  float offset[3];

  for (float &i : offset) {
    if (this->current_token != tok_number) {
      throw std::runtime_error("Expected Number");
    }
    i = std::stof(this->current_string);
  }

  Vector3 offset_v{
      offset[0],
      offset[1],
      offset[2]
  };

  update_token();
  if (this->current_token != tok_channels) {
    throw std::runtime_error("Expected CHANNELS");
  }

  update_token();
  if (this->current_token != tok_number) {
    throw std::runtime_error("Expected Number");
  }
  int channel_count = std::stoi(this->current_string);

  vector<ChannelOrder> channel_order(channel_count);
  channel_order.shrink_to_fit();

  string channel_name;
  for (int i = 0; i < channel_count; i++) {
    update_token();
    if (this->current_token != tok_string) {
      throw std::runtime_error("Expected String");
    }
    channel_name = std::move(this->current_string);
    if (channel_name == "Xposition") {
      channel_order[i] = ChannelOrder::Xposition;
    } else if (channel_name == "Yposition") {
      channel_order[i] = ChannelOrder::Yposition;
    } else if (channel_name == "Zposition") {
      channel_order[i] = ChannelOrder::Zposition;
    } else if (channel_name == "Xrotation") {
      channel_order[i] = ChannelOrder::Xrotation;
    } else if (channel_name == "Yrotation") {
      channel_order[i] = ChannelOrder::Yrotation;
    } else if (channel_name == "Zrotation") {
      channel_order[i] = ChannelOrder::Zrotation;
    } else {
      throw std::runtime_error("Unknown Channel");
    }
  }

  order.push_back(channel_order);

  update_token();
  if (this->current_token != tok_l_brace) {
    throw std::runtime_error("Expected }");
  }

  handle_joint();

  update_token();
  if (this->current_token != tok_r_brace) {
    throw std::runtime_error("Expected {");
  }
}

void BvhParser::handle_root() {
  update_token();
  if (this->current_token != tok_root) {
    throw std::runtime_error("Expected ROOT");
  }
  update_token();
  if (this->current_token != tok_string) {
    throw std::runtime_error("Expected Root Name");
  }
  string &root_name = this->current_string;

  model->push_back(Joint(root_name, -1, 0));

  update_token();
  if (this->current_token != tok_l_brace) {
    throw std::runtime_error("Expected }");
  }

  handle_joint();

  update_token();
  if (this->current_token != tok_r_brace) {
    throw std::runtime_error("Expected {");
  }

}

void BvhParser::parse_model() {
  update_token();
  if (this->current_token != tok_hierarchy) {
    throw std::runtime_error("Expected HIERARCHY");
  }
  handle_root();
}

void BvhParser::parse_frame() {
  auto *joint_motion = new JointMotion[model->size()];

  for (int i = 0; i < order.size(); i++) {
    joint_motion[i].offset = Vector3{0, 0, 0};
    string euler_order;
    float euler[3];

    for (int j = 0; j < order[i].size(); j++) {
      update_token();
      if (this->current_token != tok_number) {
        throw std::runtime_error("Expected Number");
      }
      float value = std::stof(this->current_string);

      if (order[i][j]== ChannelOrder::Xposition) {
        joint_motion[i].offset.x = value;
      } else if (order[i][j]== ChannelOrder::Yposition) {
        joint_motion[i].offset.y = value;
      } else if (order[i][j]== ChannelOrder::Zposition) {
        joint_motion[i].offset.z = value;
      } else if (order[i][j]== ChannelOrder::Xrotation) {
        euler[0] = value;
        euler_order += "X";
      } else if (order[i][j]== ChannelOrder::Yrotation) {
        euler[1] = value;
        euler_order += "Y";
      } else if (order[i][j]== ChannelOrder::Zrotation) {
        euler[2] = value;
        euler_order += "Z";
      }
    }
    joint_motion[i].rotation = Euler(order_map.at(euler_order), euler[0], euler[1], euler[2]).to_quaternion();
  }
  animation_frames->push_back(joint_motion);
}

void BvhParser::parse_motion() {
  update_token();
  if (this->current_token != tok_motion) {
    throw std::runtime_error("Expected MOTION");
  }
  update_token();
  if (this->current_token != tok_frames) {
    throw std::runtime_error("Expected Frames");
  }
  update_token();
  if (this->current_token != tok_colon) {
    throw std::runtime_error("Expected :");
  }

  update_token();
  if (this->current_token != tok_number) {
    throw std::runtime_error("Expected Number");
  }
  frame_count = std::stoi(this->current_string);

  update_token();
  if (this->current_token != tok_frame) {
    throw std::runtime_error("Expected Frame");
  }

  update_token();
  if (this->current_token != tok_time) {
    throw std::runtime_error("Expected Time");
  }
  update_token();
  if (this->current_token != tok_colon) {
    throw std::runtime_error("Expected :");
  }
  update_token();
  if (this->current_token != tok_number) {
    throw std::runtime_error("Expected Number");
  }
  // TODO: handle frame time to get frame rate

  for (int i = 0; i < frame_count; i++) {
    parse_frame();
  }
}

bool BvhParser::parse() {
  update_token();
  if (this->current_token != tok_start) {
    throw std::runtime_error("Expected Start");
  }
  model->clear();
  parse_model();

  animation_frames->clear();
  parse_motion();

  if (this->current_token != tok_end) {
    throw std::runtime_error("Expected EOF");
  }
}

vector<Joint> *BvhParser::get_model() {
  return model;
}

vector<JointMotion *> *BvhParser::get_animation_frames() {
  return animation_frames;
}

void BvhParser::update_token() {
  this->current_token = this->next_token;
  this->current_string = std::move(this->next_string);

  this->next_token = this->lexer->get_token();
  if (this->next_token == tok_string) {
    this->next_string = std::move(this->lexer->get_string());
  }
}
