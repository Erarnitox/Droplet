/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#include "CleanUp.hpp"

#include <snowflake.h>

#include <AppContext.hpp>
#include <ChallengeRoleRepository.hpp>
#include <ReactionRoleRepository.hpp>

CleanUp::CleanUp(AppContext& ctx) : db_(ctx.db) {
}

void CleanUp::clean_up_message(const dpp::snowflake& message_id) {
	{
		ReactionRoleRepository repo{db_};
		if (repo.remove(message_id)) {
			return;
		}
	}

	{
		ChallengeRoleRepository repo{db_};
		if (repo.remove(message_id)) {
			return;
		}
	}
}

void CleanUp::on_message_delete(const dpp::message_delete_t& event) {
	clean_up_message(event.id);
}

void CleanUp::on_message_delete_bulk(const dpp::message_delete_bulk_t& event) {
	for (const auto& message_id : event.deleted) {
		clean_up_message(message_id);
	}
}
