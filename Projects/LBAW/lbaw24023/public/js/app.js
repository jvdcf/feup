function addEventListeners() {
  let searchInput = document.getElementById('search');
  let filterBlocked = document.getElementById('filter-blocked');
  let filterRole = document.getElementById('filter-role');
  let projectSearchInput = document.getElementById('project-search');
  
  let searchTask = document.getElementById('search-task');
  let filterCompletion = document.getElementById('filter-is-completed');
  let filterAssigned = document.getElementById('filter-assigned-to');
  let filterDueDate = document.getElementById('date');  

  const footer = document.getElementById('footer-popup');
  const footerToggle = document.getElementById('footer-toggle');


  if (searchInput != null) {
    searchInput.addEventListener('input', filterUsers);
  }

  if (filterBlocked != null) {
    filterBlocked.addEventListener('change', filterUsers);
  }

  if (filterRole != null) {
    filterRole.addEventListener('change', filterUsers);
  }

  if (projectSearchInput != null) {
    projectSearchInput.addEventListener('input', filterProjects);
  }

  if (searchTask != null) {
    searchTask.addEventListener('input', filterTasks);
  }

  if (filterCompletion != null) {
    filterCompletion.addEventListener('change', filterTasks);
  }

  if (filterAssigned != null) {
    filterAssigned.addEventListener('change', filterTasks);
  }

  if (filterDueDate != null) {
    filterDueDate.addEventListener('change', filterTasks);
  }  

  if (footerToggle != null) {
    footerToggle.addEventListener('click', () => {
      footer.classList.toggle('d-none'); // Show/hide footer
      footerToggle.innerHTML = footer.classList.contains('d-none') ? '&#9650;' : '&#9660;'; // Change arrow direction
      footerToggle.classList.toggle('mb-5'); // Move button up when footer is visible
    });
  }

  // Search user (to add to project):
  let userInput = document.getElementById('user_input');
  if(userInput != null) {
    userInput.addEventListener('input', searchUser);
  }

  let resetPasswordLink = document.getElementById('reset-password-link');
  let resetPasswordPopup = document.getElementById('reset-password-popup');

  if (resetPasswordLink != null) {
    resetPasswordLink.addEventListener('click', function() {
      resetPasswordPopup.classList.remove('d-none');
    });
  }

  if (resetPasswordPopup != null) {
    resetPasswordPopup.addEventListener('click', function(event) {
      if (event.target === this) {
        this.classList.add('d-none');
      }
    });
  }
}

async function filterTasks() {
  let searchValue = document.getElementById('search-task').value.toLowerCase();
  if (searchValue == "") {
    searchValue = null;
  }
  let filterCompletion = document.getElementById('filter-is-completed').value;
  filterCompletion = filterCompletion == "all" ? null : (filterCompletion == "completed" ? true : false); 
  
  let filterAssigned = document.getElementById('filter-assigned-to').value;
  filterAssigned = filterAssigned == "any" ? null : filterAssigned; 
  
  let filterDueDate = document.getElementById('date').value;
  filterDueDate = filterAssigned == "" ? null : filterDueDate; 
  
  const rows = document.querySelectorAll('#tasks-table')[0];
  const curr_path = window.location.pathname;
  const curr_path_parts = curr_path.split("/");
  let proj_id = Number(curr_path_parts[2]);
  
  if (proj_id === 0) {
    return;
  }
  
  let api_path = "/api/projects/" + proj_id + "/tasks";
  let query_string = new URLSearchParams();
  if (searchValue != null) {
    query_string.append("q",searchValue);
  }
  if (filterCompletion != null) {
    query_string.append("c",filterCompletion);
  }
  if (filterAssigned != null) {
    query_string.append("a",filterAssigned);
  }
  if (filterDueDate != null) {
    query_string.append("dd",filterDueDate);
  }
  let qstr = query_string.toString();
  let query_str = qstr == "" ? api_path : api_path + "?" + query_string.toString();
  //console.log(query_str);
  let tasks = await fetch(query_str).then(response => response.json());

  //console.log(tasks.tasks);
  for (let row of rows.rows) {
    row.style.display = "none";
  }

  for (let row of rows.rows) {
    for (let task of tasks.tasks) {
      let task_id = Number(row.getAttribute("name"));
      if (task_id == task.id) {
        row.style.display = "";
      }
    }
  }  
}

async function filterUsers() {
  let searchValue = document.getElementById('search').value.toLowerCase();
  let filterBlockedValue = document.getElementById('filter-blocked').value;
  let filterRoleValue = document.getElementById('filter-role').value;
  let rows = document.querySelectorAll('#users-table tr');

  let projectMembers = await fetch('/api/projects/members').then(response => response.json());
  let projectCoordinators = await fetch('/api/projects/coordinators').then(response => response.json());

  for (let row of rows) {
    let username = row.cells[0].textContent.toLowerCase();
    let name = row.cells[1].textContent.toLowerCase();
    let email = row.cells[2].textContent.toLowerCase();
    let isBlocked = row.dataset.blocked === 'true';
    let userId = row.dataset.userId;

    let matchesSearch = username.includes(searchValue) || name.includes(searchValue) || email.includes(searchValue);
    let matchesBlockedFilter = (filterBlockedValue === 'default') ||
        (filterBlockedValue === 'blocked' && isBlocked) ||
        (filterBlockedValue === 'not-blocked' && !isBlocked);

    let matchesRoleFilter = checkRoleFilter(userId, filterRoleValue, projectMembers, projectCoordinators);

    if (matchesSearch && matchesBlockedFilter && matchesRoleFilter) {
      row.style.display = '';
    } else {
      row.style.display = 'none';
    }
  }
}

function checkRoleFilter(userId, filterRoleValue, projectMembers, projectCoordinators) {
  if (filterRoleValue === 'default') {
    return true;
  }

  if (filterRoleValue === 'member') {
    return projectMembers.some(member => member.id == userId);
  } else if (filterRoleValue === 'coordinator') {
    return projectCoordinators.some(coordinator => coordinator.id == userId);
  }

  return false;
}

function filterProjects() {
  let searchValue = document.getElementById('project-search').value.toLowerCase();
  let rows = document.querySelectorAll('#projects-table tbody tr');

  for (let row of rows) {
    let projectName = row.cells[0].textContent.toLowerCase();
    let projectDetail = row.cells[1].textContent.toLowerCase();

    let matchesSearch = projectName.includes(searchValue) || projectDetail.includes(searchValue);

    if (matchesSearch) {
      row.style.display = '';
    } else {
      row.style.display = 'none';
    }
  }
}

function previewImage(event) {
  const preview = document.getElementById('photo-preview');
  preview.src = URL.createObjectURL(event.target.files[0]);
  preview.style.display = 'block';
}

async function deletePhoto() {
  const currentPhoto = document.getElementById('current-photo');
  const noCurrentPhoto = document.getElementById('no-current-photo');
  const photoPreview = document.getElementById('photo-preview');
  const deletePhotoInput = document.getElementById('delete_photo');

  try {
    const response = await fetch('/user/photo', {
      method: 'DELETE',
      headers: {
        'Content-Type': 'application/json',
        'X-CSRF-TOKEN': document.querySelector('meta[name="csrf-token"]').getAttribute('content')
      }
    });

    const data = await response.json();

    if (data.success) {
      currentPhoto?.remove();
      noCurrentPhoto ? noCurrentPhoto.style.display = 'block' : currentPhoto.insertAdjacentHTML('afterend', '<div id="no-current-photo" class="mb-3"><span>No current photo</span></div>');
      photoPreview.style.display = 'none';
      deletePhotoInput.value = '1';
    }
  } catch (error) {
    console.error('Error:', error);
  }
}


async function searchUser() {
  let userInput = document.getElementById('user_input').value.toLowerCase();
  let results = document.getElementById('search_results');

  if (userInput === '') {
    results.style.display = 'none';
    return;
  }

  let user = await fetch('/api/members/users/' + userInput).then(response => response.json());

  if (user.username != null) {
    results.children[0].textContent = user.name;
    results.children[1].textContent = user.username;
    results.children[2].textContent = user.email;
    results.style.display = '';
  } else {
    results.style.display = 'none';
  }
}

addEventListeners();
