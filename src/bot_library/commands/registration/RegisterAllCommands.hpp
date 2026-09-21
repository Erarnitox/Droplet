#pragma once

struct AppContext;
class Bot;

namespace CommandRegistration {

void register_all(Bot& bot, AppContext& ctx);

}
