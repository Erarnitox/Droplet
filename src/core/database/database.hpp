#pragma once

#include <cstddef>
#include <string>
#include <optional>

class Database {
    
public:
    Database() = delete;
    
    [[nodiscard]]
    static auto connect (
        const std::string& db_name,
        const std::string& user, 
        const std::string& password,
        const std::string& host,
        const std::string& port
    ) -> bool;

    [[nodiscard]]
    static auto connect(const std::string& connection_string) -> bool;
    
    static auto has_connection() noexcept -> bool;
    static auto reconnect() noexcept -> void;

    static auto disconnect() noexcept -> void;

    [[nodiscard]]
    static auto get_challenge_role_data(size_t message_id) noexcept -> std::pair<size_t, std::string>;
    static auto insert_challenge_role_data(size_t role_id, size_t guild_id, size_t message_id, const std::string& flag) noexcept -> void;

    [[nodiscard]]
    static auto get_reaction_role_data(size_t message_id, const std::string& reaction_emoji) noexcept -> size_t;
    static auto insert_reaction_role_data(const std::string& role_id, size_t guild_id, const std::string& message_id, const std::string& emoji) noexcept -> void;

    [[nodiscard]]
    static auto get_welcome_channel_id(size_t guild_id) noexcept -> size_t;
    static auto insert_welcome_channel_id(size_t guild_id, size_t channel_id) noexcept -> void;   

    [[nodiscard]]
    static auto get_goodbye_channel_id(size_t guild_id) noexcept -> size_t;
    static auto insert_goodbye_channel_id(size_t guild_id, size_t channel_id) noexcept -> void;   

    [[nodiscard]]
    static auto get_log_channel_id(size_t guild_id) noexcept -> size_t;
    static auto insert_log_channel_id(size_t guild_id, size_t channel_id) noexcept -> void;
};