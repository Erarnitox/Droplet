"use strict";

/*
 *  (c) Copyright dropsoft.org - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *  
 *  License: MIT License
 *
 *  Join us: https://discord.erarnitox.de
*/

document.addEventListener('DOMContentLoaded', async function() {
    try {
        const backend_port = 3000; 
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
            const res_json = await response.json();
            const users = res_json.users
            
            const leaderboard_table = document.getElementById("leaderboard");
            const tbody = leaderboard_table.querySelector('tbody');
            tbody.innerHTML = '';

            users.forEach((user, index) => {
                const row = tbody.insertRow();
                row.insertCell().textContent = index + 1;
                row.insertCell().textContent = user.user_name || 'Unknown';
                row.insertCell().textContent = user.exp || 0;
            });
        }
    } catch (error) {
        console.log(error.message);
        const leaderboard_table = document.getElementById("leaderboard");
        const tbody = leaderboard_table.querySelector('tbody');
        tbody.innerHTML = '';
        const row = tbody.insertRow();
        const cell = row.insertCell();
        cell.colSpan = 3;
        cell.textContent = 'Failed to load leaderboard';
    }
}, false);
