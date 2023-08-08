#pragma once 

#include <dpp/dispatcher.h>
#include <vector> 
#include <dpp/dpp.h>

#include <iostream>

// tricks for static polymorphism 
namespace Commands {
    typedef void (*register_global_slash_commands)(std::vector<dpp::slashcommand>& command_list, const dpp::cluster& bot); 
    typedef void (*handle_global_slash_commands)(const dpp::slashcommand_t& event, dpp::cluster& bot, const std::vector<dpp::slashcommand>& command_list); 
    typedef void (*welcome_member)(const dpp::guild_member_add_t& event, dpp::cluster& bot); 
    typedef void (*leave_member)(const dpp::guild_member_remove_t& event, dpp::cluster& bot); 
    typedef void (*handle_button_clicks)(const dpp::button_click_t& event, dpp::cluster& bot); 
    typedef void (*handle_form_submits)(const dpp::form_submit_t& event, dpp::cluster& bot); 
    typedef void (*handle_reaction_added)(const dpp::message_reaction_add_t& event, dpp::cluster& bot); 
    typedef void (*handle_reaction_removed)(const dpp::message_reaction_remove_t& event, dpp::cluster& bot);

    static inline
    void call_register_global_slash_commands(register_global_slash_commands func, std::vector<dpp::slashcommand>& command_list, const dpp::cluster& bot){
        func(command_list, bot);
    }

    static inline
    void call_handle_global_slash_commands(handle_global_slash_commands func, const dpp::slashcommand_t& event, dpp::cluster& bot, 
    const std::vector<dpp::slashcommand>& command_list){
        func(event, bot, command_list);
    }

    static inline
    void call_welcome_member(welcome_member func, const dpp::guild_member_add_t& event, dpp::cluster& bot) {
        func(event, bot);
    }

    static inline
    void call_leave_member(leave_member func, const dpp::guild_member_remove_t& event, dpp::cluster& bot) {
        func(event, bot);
    }

    static inline
    void call_handle_button_clicks(handle_button_clicks func, const dpp::button_click_t& event, dpp::cluster& bot) {
        func(event, bot);
    }

    static inline
    void call_handle_form_submits(handle_form_submits func, const dpp::form_submit_t& event, dpp::cluster& bot) {
        func(event, bot);
    }

    static inline
    void call_handle_reaction_added(handle_reaction_added func, const dpp::message_reaction_add_t& event, dpp::cluster& bot) {
        func(event, bot);
    }

    static inline
    void call_handle_reaction_removed(handle_reaction_removed func, const dpp::message_reaction_remove_t& event, dpp::cluster& bot) {
        func(event, bot);
    }

    class CommandFunctionPointers {
    public:
        static std::vector<Commands::register_global_slash_commands>    _register_global_slash_commands;
        static std::vector<Commands::handle_global_slash_commands>      _handle_global_slash_commands;
        static std::vector<Commands::handle_button_clicks>              _handle_button_clicks;
        static std::vector<Commands::handle_form_submits>               _handle_form_submits;
        static std::vector<Commands::handle_reaction_added>             _handle_reaction_added;
        static std::vector<Commands::handle_reaction_removed>           _handle_reaction_removed;
        static std::vector<Commands::leave_member>                      _leave_member;
        static std::vector<Commands::welcome_member>                    _welcome_member;
    
        static std::vector<Commands::register_global_slash_commands>&    register_global_slash_commands();
        static std::vector<Commands::handle_global_slash_commands>&      handle_global_slash_commands();
        static std::vector<Commands::handle_button_clicks>&              handle_button_clicks();
        static std::vector<Commands::handle_form_submits>&               handle_form_submits();
        static std::vector<Commands::handle_reaction_added>&             handle_reaction_added();
        static std::vector<Commands::handle_reaction_removed>&           handle_reaction_removed();
        static std::vector<Commands::leave_member>&                      leave_member();
        static std::vector<Commands::welcome_member>&                    welcome_member();
    };

    template <typename T>
    struct CommandRegistration {
        CommandRegistration() {
            CommandFunctionPointers::_register_global_slash_commands.push_back(&T::registerGlobalSlashCommand);
            CommandFunctionPointers::_handle_global_slash_commands.push_back(&T::handleGlobalSlashCommand);
            CommandFunctionPointers::_handle_button_clicks.push_back(&T::handleButtonClicks);
            CommandFunctionPointers::_handle_form_submits.push_back(&T::handleFormSubmits);
            CommandFunctionPointers::_handle_reaction_added.push_back(&T::handleReactionAdded);
            CommandFunctionPointers::_handle_reaction_removed.push_back(&T::handleReactionRemoved);
            CommandFunctionPointers::_leave_member.push_back(&T::leaveMember);
            CommandFunctionPointers::_welcome_member.push_back(&T::welcomeMember);
        }
    };
}