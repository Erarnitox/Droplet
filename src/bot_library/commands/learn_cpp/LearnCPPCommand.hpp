/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#pragma once

#include <IGlobalSlashCommand.hpp>
#include <string_view>

struct AppContext;

class LearnCPPCommand : public IGlobalSlashCommand {
  public:
	static constexpr std::string_view k_name{"learn_cpp"};
	static constexpr std::string_view k_description{"Get started learning modern C++ today!"};

	explicit LearnCPPCommand(AppContext& ctx);

	void on_slashcommand(const dpp::slashcommand_t& event) override;
};
