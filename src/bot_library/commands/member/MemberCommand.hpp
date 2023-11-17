#pragma once

#include <Bot.hpp>

class MemberCommand : public IGlobalSlashCommand {
  public:
	MemberCommand();

	virtual void on_slashcommand(const dpp::slashcommand_t& event) override;
};