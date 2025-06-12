"use strict";

/*
 *  (c) Copyright dropsoft.org - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *  
 *  License: MIT License
 *
 *  Join us: https://discord.dropsoft.org
*/

document.addEventListener('DOMContentLoaded', async function() {
    try {
        let backend_port = 3000;
        
        const hostname = 'api.erarnitox.de';
        const response = await fetch(`${window.location.protocol}//${hostname}:${backend_port}/leaderboard`, {
            method: 'GET',
            mode: 'cors',
            credentials: 'include',
            headers: {
                "Accept": "application/json",
                "Content-Type": "application/json;charset=UTF-8",
            },
        });
        
        if(response.ok) {
            const users = await response.json();
            
            let leaderboard_table = document.getElementById("leaderboard");

            const topUsers = users.slice(0, 10);

            topUsers.forEach((user, index) => {
                const row = leaderboard_table.insertRow();
                row.insertCell().textContent = index + 1;
                row.insertCell().textContent = user.user_name || 'Unknown';
                row.insertCell().textContent = user.exp || 0;
            });
        }
    } catch (error) {
        console.log(error.message);
        const leaderboard_table = document.getElementById("leaderboard");
        const row = leaderboard_table.insertRow();
        const cell = row.insertCell();
        cell.colSpan = 3;
        cell.textContent = 'Failed to load leaderboard';
    }
}, false);