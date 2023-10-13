# pragma once

#include <dpp/cluster.h>
#include <dpp/dispatcher.h>

class IReactionCommand {
    public:
        virtual
        auto handle_reaction_added(const dpp::message_reaction_add_t&, dpp::cluster& bot) -> void = 0;

        virtual
        auto handle_reaction_removed(const dpp::message_reaction_remove_t&, dpp::cluster& bot) -> void = 0;
};