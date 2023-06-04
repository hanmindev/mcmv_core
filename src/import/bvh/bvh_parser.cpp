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
  parse();
}
BvhParser::~BvhParser() {
  delete this->lexer;
}

void BvhParser::handle_end_site() {
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

    Vector3 offset;
    handle_offset(offset);

    update_token();
    if (this->current_token != tok_r_brace) {
      throw std::runtime_error("Expected }");
    }
  }
}

void BvhParser::handle_joint(int parent_index) {
  if (this->next_token == tok_end) {
    handle_end_site();
    return;
  }

  update_token();
  int index = this->model->size();

  if (this->current_token != tok_joint) {
    throw std::runtime_error("Expected JOINT");
  }

  model->push_back(Joint(handle_spaced_string(), parent_index, index));
  Joint &joint = model->back();

  update_token();
  if (this->current_token != tok_l_brace) {
    throw std::runtime_error("Expected {");
  }

  handle_offset(joint.offset);

  handle_channels();

  while (this->next_token == tok_joint || this->next_token == tok_end) {
    handle_joint(index);
  }

  update_token();
  if (this->current_token != tok_r_brace) {
    throw std::runtime_error("Expected {");
  }
}

string BvhParser::handle_spaced_string() {
  update_token();
  if (this->current_token != tok_string) {
    throw std::runtime_error("Expected String");
  }
  string joint_name = std::move(this->current_string);
  while (this->next_token == tok_string) {
    update_token();
    joint_name += " " + this->current_string;
  }
  return joint_name;
}

void BvhParser::handle_offset(Vector3 &offset_v) {
  update_token();
  if (this->current_token != tok_offset) {
    throw std::runtime_error("Expected OFFSET");
  }

  float offset[3];

  for (float &i : offset) {
    update_token();
    if (this->current_token != tok_number) {
      throw std::runtime_error("Expected Number");
    }
    i = std::stof(this->current_string);
  }

  offset_v.x = offset[0];
  offset_v.y = offset[1];
  offset_v.z = offset[2];
}

void BvhParser::handle_channels() {
  order.emplace_back();
  vector<ChannelOrder> &channel_order = order.back();

  update_token();
  if (this->current_token != tok_channels) {
    throw std::runtime_error("Expected CHANNELS");
  }

  update_token();
  if (this->current_token != tok_number) {
    throw std::runtime_error("Expected Number");
  }
  int channel_count = std::stoi(this->current_string);
  channel_order.resize(channel_count);

  for (int i = 0; i < channel_count; i++) {
    update_token();
    if (this->current_token != tok_string) {
      throw std::runtime_error("Expected String");
    }
    if (this->current_string == "Xposition") {
      channel_order[i] = ChannelOrder::Xposition;
    } else if (this->current_string == "Yposition") {
      channel_order[i] = ChannelOrder::Yposition;
    } else if (this->current_string == "Zposition") {
      channel_order[i] = ChannelOrder::Zposition;
    } else if (this->current_string == "Xrotation") {
      channel_order[i] = ChannelOrder::Xrotation;
    } else if (this->current_string == "Yrotation") {
      channel_order[i] = ChannelOrder::Yrotation;
    } else if (this->current_string == "Zrotation") {
      channel_order[i] = ChannelOrder::Zrotation;
    } else {
      throw std::runtime_error("Unknown Channel");
    }
  }

}

void BvhParser::handle_root() {
  if (this->next_token != tok_root) {
    throw std::runtime_error("Expected ROOT");
  }
  // ROOT can be substituted with JOINT with no issues
  this->next_token = tok_joint;

  // treat it like a joint
  handle_joint(-1);
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

      if (order[i][j] == ChannelOrder::Xposition) {
        joint_motion[i].offset.x = value;
      } else if (order[i][j] == ChannelOrder::Yposition) {
        joint_motion[i].offset.y = value;
      } else if (order[i][j] == ChannelOrder::Zposition) {
        joint_motion[i].offset.z = value;
      } else if (order[i][j] == ChannelOrder::Xrotation) {
        euler[0] = value;
        euler_order += "X";
      } else if (order[i][j] == ChannelOrder::Yrotation) {
        euler[1] = value;
        euler_order += "Y";
      } else if (order[i][j] == ChannelOrder::Zrotation) {
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
  parse_model();
  parse_motion();

  update_token();
  if (this->current_token != tok_eof) {
    throw std::runtime_error("Expected EOF");
  }
  return true;
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
  if (this->next_token == tok_string || this->next_token == tok_number) {
    this->next_string = std::move(this->lexer->get_string());
  }
}
