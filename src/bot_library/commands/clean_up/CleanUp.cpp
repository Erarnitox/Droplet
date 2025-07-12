/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 *
 *  Description:
 *
 *  Documentation: https://droplet.erarnitox.de/doxygen/html/
 */

#include "CleanUp.hpp"

#include <snowflake.h>

#include <ChallengeRoleRepository.hpp>
#include <ReactionRoleRepository.hpp>

//-----------------------------------------------------
//
//-----------------------------------------------------
void CleanUp::on_message_create(const dpp::message_create_t& event) {
	(void)event;

	return;
}

//-----------------------------------------------------
//
//-----------------------------------------------------
static inline void clean_up_message(const dpp::snowflake& message_id) {
	// clean up reaction roles
	{
		ReactionRoleRepository repo;
		if (repo.remove(message_id)) {
			return;
		}
	}

	// clean up challenge role
	{
		ChallengeRoleRepository repo;
		if (repo.remove(message_id)) {
			return;
		}
	}

	/*
	// Don't clean up challenge badges!
	// They are needed for user statistics
	// clean up challenge badge
	{
		ChallengeBadgeRepository repo;
		if(repo.remove(message_id)){
			return;
		}
	}*/

	return;
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void CleanUp::on_message_delete(const dpp::message_delete_t& event) {
	const auto& message_id{event.id};
	return clean_up_message(message_id);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void CleanUp::on_message_delete_bulk(const dpp::message_delete_bulk_t& event) {
	const auto& message_ids{event.deleted};

	for (const auto& message_id : message_ids) {
		clean_up_message(message_id);
	}

	return;
}
