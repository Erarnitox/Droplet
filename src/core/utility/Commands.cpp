#include "Commands.hpp"

// initialize members:
std::vector<Commands::register_global_slash_commands>
	Commands::CommandFunctionPointers::_register_global_slash_commands;
std::vector<Commands::handle_global_slash_commands> Commands::CommandFunctionPointers::_handle_global_slash_commands;
std::vector<Commands::handle_button_clicks> Commands::CommandFunctionPointers::_handle_button_clicks;
std::vector<Commands::handle_form_submits> Commands::CommandFunctionPointers::_handle_form_submits;
std::vector<Commands::handle_reaction_added> Commands::CommandFunctionPointers::_handle_reaction_added;
std::vector<Commands::handle_reaction_removed> Commands::CommandFunctionPointers::_handle_reaction_removed;
std::vector<Commands::leave_member> Commands::CommandFunctionPointers::_leave_member;
std::vector<Commands::welcome_member> Commands::CommandFunctionPointers::_welcome_member;

// implement geters
std::vector<Commands::register_global_slash_commands> &
Commands::CommandFunctionPointers::register_global_slash_commands() {
	return Commands::CommandFunctionPointers::_register_global_slash_commands;
}

std::vector<Commands::handle_global_slash_commands> &Commands::CommandFunctionPointers::handle_global_slash_commands() {
	return Commands::CommandFunctionPointers::_handle_global_slash_commands;
}

std::vector<Commands::handle_button_clicks> &Commands::CommandFunctionPointers::handle_button_clicks() {
	return Commands::CommandFunctionPointers::_handle_button_clicks;
}

std::vector<Commands::handle_form_submits> &Commands::CommandFunctionPointers::handle_form_submits() {
	return Commands::CommandFunctionPointers::_handle_form_submits;
}

std::vector<Commands::handle_reaction_added> &Commands::CommandFunctionPointers::handle_reaction_added() {
	return Commands::CommandFunctionPointers::_handle_reaction_added;
}

std::vector<Commands::handle_reaction_removed> &Commands::CommandFunctionPointers::handle_reaction_removed() {
	return Commands::CommandFunctionPointers::_handle_reaction_removed;
}

std::vector<Commands::leave_member> &Commands::CommandFunctionPointers::leave_member() {
	return Commands::CommandFunctionPointers::_leave_member;
}

std::vector<Commands::welcome_member> &Commands::CommandFunctionPointers::welcome_member() {
	return Commands::CommandFunctionPointers::_welcome_member;
}