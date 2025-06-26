"use strict";

/*
 * (c) Copyright dropsoft.org - All rights reserved
 * Author: Erarnitox <david@erarnitox.de>
 *
 * License: MIT License
 *
 * Join us: https://discord.erarnitox.de
*/

document.addEventListener('DOMContentLoaded', async function() {
    const resourceTable = document.getElementById("reslist");
    const tbody = resourceTable.querySelector('tbody');
    const filterForm = document.getElementById('resource-filter');

    // Function to fetch and render resources
    async function fetchAndRenderResources(filters) {
        // Clear existing table content
        tbody.innerHTML = '<tr><td colspan="4" class="text-center py-8 text-red-400 italic">Loading resources data...</td></tr>';

        const backend_port = 3000;
        const hostname = 'api.erarnitox.de';
        
        // Only use the category filter for the backend API call if it exists
        const backendParams = new URLSearchParams();
        if (filters.category && filters.category !== 'all') {
            backendParams.append('category', filters.category);
        }
        
        const url = `${window.location.protocol}//${hostname}:${backend_port}/resources${backendParams.toString() ? '?' + backendParams.toString() : ''}`;

        try {
            const response = await fetch(url, {
                method: 'GET',
                mode: 'cors',
                credentials: 'include',
                headers: {
                    "Accept": "application/json",
                    "Content-Type": "application/json;charset=UTF-8",
                },
            });
            
            if (response.ok) {
                const res_json = await response.json();
                let resources = res_json.resources;

                // Filter by difficulty
                if (filters.difficulty && filters.difficulty > 0) {
                    resources = resources.filter(res => res.difficulty === filters.difficulty);
                }

                // Filter by tags
                if (filters.tags.length > 0) {
                    resources = resources.filter(res => {
                        return filters.tags.every(filterTag => res.tags.includes(filterTag));
                    });
                }

                // Clear loading message
                tbody.innerHTML = '';
                
                if (resources.length === 0) {
                    const row = tbody.insertRow();
                    const cell = row.insertCell();
                    cell.colSpan = 4;
                    cell.className = 'text-center py-8 text-gray-500 italic';
                    cell.textContent = 'No resources found with the selected filters.';
                } else {
                    resources.forEach((res) => {
                        const row = tbody.insertRow();
                        // Add a data attribute for the URL to each row
                        row.setAttribute('data-url', res.url); 
                        row.classList.add('transition-colors', 'duration-150', 'hover:bg-gray-700');
                        
                        row.insertCell().textContent = res.category;
                        row.insertCell().textContent = res.title;
                        row.insertCell().textContent = res.description;
                        row.insertCell().textContent = res.tags;
        
                        row.style.cursor = 'pointer';
                        row.addEventListener('click', () => {
                            window.location.href = res.url;
                        });
                    });
                }
            } else {
                throw new Error('Server responded with an error.');
            }
        } catch (error) {
            console.error('Failed to fetch resources:', error);
            tbody.innerHTML = '<tr><td colspan="4" class="text-center py-8 text-red-400 italic">Failed to load resources</td></tr>';
        }
    }

    // Handle initial page load and filtering from URL parameters
    const initialUrlParams = new URLSearchParams(window.location.search);
    const initialFilters = {
        category: initialUrlParams.get('category') || '',
        tags: initialUrlParams.get('tags')?.split(',').map(tag => tag.trim()).filter(tag => tag) || [],
        difficulty: parseInt(initialUrlParams.get('difficulty'), 10) || 0,
    };
    
    // Set form inputs to match URL parameters
    if (initialFilters.category) document.getElementById('category-input').value = initialFilters.category;
    if (initialFilters.tags.length > 0) document.getElementById('tags-input').value = initialFilters.tags.join(',');
    if (initialFilters.difficulty) document.getElementById('difficulty-select').value = initialFilters.difficulty.toString();

    fetchAndRenderResources(initialFilters);

    // Handle form submission for filtering
    filterForm.addEventListener('submit', function(event) {
        event.preventDefault(); // Prevent page reload
        
        const newCategory = document.getElementById('category-input').value.trim().toLowerCase();
        const newDifficulty = parseInt(document.getElementById('difficulty-select').value, 10);
        const newTags = document.getElementById('tags-input').value.split(',').map(tag => tag.trim()).filter(tag => tag);

        const newFilters = {
            category: newCategory,
            difficulty: newDifficulty,
            tags: newTags
        };

        // Update URL search parameters to reflect the new filters
        const newUrlParams = new URLSearchParams();
        if (newFilters.category) newUrlParams.set('category', newFilters.category);
        if (newFilters.difficulty > 0) newUrlParams.set('difficulty', newFilters.difficulty.toString());
        if (newFilters.tags.length > 0) newUrlParams.set('tags', newFilters.tags.join(','));
        
        const newUrl = `${window.location.pathname}?${newUrlParams.toString()}`;
        window.history.pushState({path: newUrl}, '', newUrl);

        // Fetch and render resources with the new filters
        fetchAndRenderResources(newFilters);
    });
});
