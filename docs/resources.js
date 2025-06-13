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
        // get parameters:
        const urlParams = new URLSearchParams(window.location.search);
        const category = urlParams.get('category');
        const tagsInput = urlParams.get('tags') || '';
        const tags = tagsInput.split(',').map(tag => tag.trim()).filter(tag => tag);
        const difficulty = urlParams.get('difficulty');

        // Construct query parameters for backend
        const params = new URLSearchParams();
        if (category && category.length > 0) {
            params.append('category', category);
        }

        /*
        if (difficulty && difficulty != 0) {
            params.append('difficulty', difficulty);
        }
        if (tags && tags.length > 0) {
            params.append('tags', tags.join(','));
        }*/

        const backend_port = 3000;
        const hostname = 'api.erarnitox.de';
        const response = await fetch(`${window.location.protocol}//${hostname}:${backend_port}/resources${params.toString() ? '?' + params.toString() : ''}`, {
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
            const resources = res_json.resources
            
            const leaderboard_table = document.getElementById("reslist");
            const tbody = leaderboard_table.querySelector('tbody');

            resources.forEach((res, index) => {
                const row = tbody.insertRow();
                row.insertCell().textContent = res.category
                row.insertCell().textContent = res.title
                row.insertCell().textContent = res.description
                row.insertCell().textContent = res.tags

                row.style.cursor = 'pointer';
                row.addEventListener('click', () => {
                    window.location.href = res.url;
                });
            });
        }
    } catch (error) {
        console.log(error.message);
        const leaderboard_table = document.getElementById("reslist");
        const tbody = leaderboard_table.querySelector('tbody');
        const row = tbody.insertRow();
        const cell = row.insertCell();
        cell.colSpan = 4;
        cell.textContent = 'Failed to load resources';
    }
}, false);