# EAP: Architecture Specification and Prototype

SyncTask: Project management done easier, keeping everyone up to date.

## A7: Web Resources Specification

This section presents an overview of the complete SyncTask web application where the modules are identified and briefly described.

### 1. Overview

| Module                                           | Module Description                                                                                                                                                                                                                                                                               |
| ------------------------------------------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| M01: Authentication and Profile Management       | Web resources associated with user authentication and profile management. Features included in this module are registration, login/logout, password recovery, being able to edit profile information and delete the profile.                                                                     |
| M02: Project Management                          | Web resources related to project creation, management, and organization within SyncTask. Features include creating new projects, viewing and editing project details, inviting members and manage project status                                                                                 |
| M03: Task Management                             | Web resources dedicated to managing tasks within projects. This module supports creating, assigning, updating, and completing tasks, with functionalities such as due dates, priorities and labels.                                                                                              |
| M04: Notification System                         | Web resources for managing user notifications within the SyncTask platform. Features include sending notifications for task assignments, project invitations, role changes, and task completions. Users can view, acknowledge, and manage notifications to stay updated on project activity.     |
| M05: Discussion Forum                            | Web resources facilitating project-based communication through project discussion forum and task-specific discussions. This module allows users to post, edit, and delete comments on tasks, enabling effective team collaboration and feedback.                                                 |
| M06: Team and Membership Management              | Web resources for managing project team members. Project coordinators can use this module to add or remove members, assign roles, and adjust member permissions to ensure that each project has the necessary team structure and role assignments for efficient collaboration                    |
| M07: Activity and Timeline Tracking              | Web resources providing an overview of project activities and timelines. This module displays project goals, task deadlines, and a chronological summary of updates, helping users track progress and stay informed about upcoming events.                                                       |
| M08: Search and Filtering                        | Web resources enabling search and filtering capabilities across tasks, projects, and discussions. Users can search for specific content using various filters (e.g., by due date, priority or label), facilitating quick access to relevant information.                                         |
| M09: Administrator Dashboard and User Management | Web resources for administrative functions within SyncTask. This module allows administrators to monitor and manage user accounts, enforce platform policies, and access all projects for content moderation. It includes tools for managing account status, permissions, and project oversight. |

### 2. Permissions

This section defines the permissions to outline access levels for the various resources in SyncTask.

| Permission Code | Role          | Description                                                                                |
| --------------- | ------------- | ------------------------------------------------------------------------------------------ |
| **PUB**         | Public        | Accessible to guests without special privileges.                                           |
| **USR**         | User          | Accessible to authenticated users.                                                         |
| **OWN**         | Owner         | Access restricted to users who own the associated resource (e.g., own profile, own tasks). |
| **TEAM**        | Team Member   | Access for project team members, enabling collaboration and limited task management.       |
| **COORD**       | Coordinator   | Access for users with coordinator roles in projects.                                       |
| **ADM**         | Administrator | Access for platform administrators for oversight and user management.                      |

### 3. OpenAPI Specification

The specification is also available in the [repository](https://gitlab.up.pt/lbaw/lbaw2425/lbaw24023/-/blob/main/a7_openapi.yaml?ref_type=heads).

```yaml
openapi: 3.0.0

info:
  version: "1.0"
  title: "SyncTask Web API"
  description: "Web Resources Specification (A7) for SyncTask"

servers:
  - url: http://localhost:8000
    description: Development server

tags:
  - name: "M01: Authentication and Profile Management"
  - name: "M02: Project Management"
  - name: "M03: Task Management"
  - name: "M04: Notification System"
  - name: "M05: Messaging and Discussion Forum"
  - name: "M06: Team and Membership Management"
  - name: "M07: Activity and Timeline Tracking"
  - name: "M08: Search and Filtering"
  - name: "M09: Administrator Dashboard and User Management"

paths:
  /login:
    get:
      operationId: R101
      summary: "R101: Login Form"
      description: "Provide the login form for users. Access: PUB"
      tags:
        - "M01: Authentication and Profile Management"
      responses:
        "200":
          description: "The login UI is displayed successfully."
        "404":
          description: "The login page could not be found."
        "500":
          description: "Internal server error. Could not display the login form."

    post:
      operationId: R102
      summary: "R102: Login Action"
      description: "Authenticate user credentials and establish a session. Access: PUB"
      tags:
        - "M01: Authentication and Profile Management"
      requestBody:
        required: true
        content:
          application/x-www-form-urlencoded:
            schema:
              type: object
              properties:
                usernameOrEmail:
                  type: string
                  description: "The username or email of the user."
                password:
                  type: string
                  description: "The password of the user."
              required:
                - usernameOrEmail
                - password
      responses:
        "302":
          description: "Redirect after processing login."
          headers:
            Location:
              schema:
                type: string
              examples:
                302Success:
                  description: "Successful login. Redirect to user dashboard."
                  value: "/projects"
                302Failure:
                  description: "Failed login. Redirect to login form."
                  value: "/login"
        "401":
          description: "Unauthorized. Incorrect username/email or password."
        "404":
          description: "Login endpoint not found."
        "500":
          description: "Internal server error during login process."

  /logout:
    post:
      operationId: R103
      summary: "R103: Logout Action"
      description: "Logout the current authenticated user. Access: USR"
      tags:
        - "M01: Authentication and Profile Management"
      responses:
        "302":
          description: "Redirect after logout."
          headers:
            Location:
              schema:
                type: string
              examples:
                302Success:
                  description: "Successful logout. Redirect to login form."
                  value: "/login"
        "404":
          description: "Logout endpoint not found."
        "500":
          description: "Internal server error during logout process."

  /register:
    get:
      operationId: R104
      summary: "R104: Registration Form"
      description: "Show registration form. Access: PUB"
      tags:
        - "M01: Authentication and Profile Management"
      responses:
        "200":
          description: "Ok. Show registration UI"
        "404":
          description: "The registration page could not be found."
        "500":
          description: "Internal server error. Could not display the registration form."

    post:
      operationId: R105
      summary: "R105: Register New User"
      description: "Register a new user in the system. Access: PUB"
      tags:
        - "M01: Authentication and Profile Management"
      requestBody:
        required: true
        content:
          application/x-www-form-urlencoded:
            schema:
              type: object
              properties:
                name:
                  type: string
                  description: "The name desired by the user"
                username:
                  type: string
                  description: "The username desired by the user"
                email:
                  type: string
                  description: "The email of the user"
                password:
                  type: string
                  description: "Password desired for the account"
                photo:
                  type: string
                  format: binary
              required:
                - name
                - username
                - email
                - password
      responses:
        "302":
          description: "Redirect after successful registration."
          headers:
            Location:
              schema:
                type: string
              examples:
                302Success:
                  description: "Registration complete. Redirect to login form."
                  value: "/login"
                302Failure:
                  description: "Failed registration. Redirect to registration form."
                  value: "/register"
        "400":
          description: "Registration failed due to invalid input or duplicate entries."
        "404":
          description: "Registration endpoint not found."
        "409":
          description: "Conflict. Email or username already exists."
        "500":
          description: "Internal server error during registration process."

  /projects:
    get:
      operationId: R201
      summary: "R201: Show projects of the user"
      description: "Show all the projects the user is currently in. Access: USR."
      tags:
        - "M02: Project Management"
      responses:
        "200":
          description: "Ok. Show projects page."
        "302":
          description: "User is not logged in. Redirect to login page."
          headers:
            Location:
              schema:
                type: string
              examples:
                302Failure:
                  description: "User is not logged in. Redirect to login page."
                  value: "/login"
        "500":
          description: "Internal server error."

    post:
      operationId: R203
      summary: "R203: Store a new project"
      description: "Create a new project. Access: USR"
      tags:
        - "M02: Project Management"
      requestBody:
        required: true
        content:
          application/x-www-form-urlencoded:
            schema:
              type: object
              properties:
                name:
                  type: string
                detail:
                  type: string
                user_id:
                  type: integer
              required:
                - name
                - user_id
      responses:
        "302":
          description: "Redirect after creating a new project successfully."
          headers:
            Location:
              schema:
                type: string
              examples:
                302Success:
                  description: "Created successfully. Redirect to project page."
                  value: "/projects/{project_id}"
                302Failure:
                  description: "Failed creation. Redirect to project creation page."
                  value: "/projects/create"
        "500":
          description: "Internal server error."

  /projects/create:
    get:
      operationId: R202
      summary: "R202: Create New Project Form"
      description: "Provide form for creating a new project. Access: USR"
      tags:
        - "M02: Project Management"
      responses:
        "200":
          description: "Ok. Show new project form."
        "302":
          description: "User is not logged in. Redirect to login page."
          headers:
            Location:
              schema:
                type: string
              examples:
                302Failure:
                  description: "User is not logged in. Redirect to login page."
                  value: "/login"
        "500":
          description: "Internal server error."

  /projects/{project_id}:
    get:
      operationId: R204
      summary: "R204: Show a specific project"
      description: "Show details about a specific project given an id. Access: TEAM"
      parameters:
        - name: project_id
          in: path
          description: "Project ID"
          required: true
          schema:
            type: integer
      tags:
        - "M02: Project Management"
      responses:
        "200":
          description: "Ok. Show project details."
        "404":
          description: "Project not found."
        "500":
          description: "Internal server error."

  /projects/{project}/tasks/create:
    get:
      operationId: R302
      summary: "R302: Create New Task Form"
      description: "Provide form for creating a new task inside a given project. Access: COORD"
      tags:
        - "M03: Task Management"
      parameters:
        - in: path
          name: project
          schema:
            type: integer
          required: true
      responses:
        "200":
          description: "Ok. Show new task form."
        "404":
          description: "Project not found."
        "500":
          description: "Internal server error."

  /projects/{project}/tasks:
    get:
      operationId: R301
      summary: "R301: Get all tasks of a project"
      description: "Get all tasks of a given project in order. Access: TEAM"
      tags:
        - "M03: Task Management"
      parameters:
        - in: path
          name: project
          schema:
            type: integer
          required: true
      responses:
        "200":
          description: "Ok. Show Project Tasks page (UI08)."
        "404":
          description: "Project not found."
        "500":
          description: "Internal server error."

    post:
      operationId: R303
      summary: "R303: Store a new task"
      description: "Create a new task inside a given project. Access: COORD"
      tags:
        - "M03: Task Management"
      parameters:
        - in: path
          name: project
          schema:
            type: integer
          required: true
      requestBody:
        required: true
        content:
          application/x-www-form-urlencoded:
            schema:
              type: object
              properties:
                title:
                  type: string
                description:
                  type: string
                dueDate:
                  type: string
                  format: date-time
                projectId:
                  type: number
                assignedId:
                  type: number
              required:
                - title
                - projectId
      responses:
        "302":
          description: "Redirect after creating a new task successfully."
          headers:
            Location:
              schema:
                type: string
              examples:
                302Success:
                  description: "Created successfully. Redirect to task page (UI09)."
                  value: "/projects/{project}/tasks/{task}"
                302Failure:
                  description: "Failed creation. Redirect to task form with an error message."
                  value: "/projects/{project}/tasks/create"
        "404":
          description: "Project not found."
        "500":
          description: "Internal server error."

  /projects/{project}/tasks/{task}:
    get:
      operationId: R304
      summary: "R304: Show a specific task"
      description: "Show details about a specific task given an id. Access: TEAM"
      tags:
        - "M03: Task Management"
      parameters:
        - in: path
          name: project
          schema:
            type: integer
          required: true
        - in: path
          name: task
          schema:
            type: integer
          required: true
      responses:
        "200":
          description: "Ok. Show task details (UI09)."
        "404":
          description: "Task not found."
        "500":
          description: "Internal server error."

    patch:
      operationId: R306
      summary: "R306: Update a specific task"
      description: "Update some or all components of a task given an id. Access: COORD"
      tags:
        - "M03: Task Management"
      parameters:
        - in: path
          name: project
          schema:
            type: integer
          required: true
        - in: path
          name: task
          schema:
            type: integer
          required: true
      requestBody:
        required: true
        content:
          application/x-www-form-urlencoded:
            schema:
              type: object
              properties:
                title:
                  type: string
                description:
                  type: string
                dueDate:
                  type: string
                  format: date-time
                assignedId:
                  type: number
      responses:
        "302":
          description: "Redirect after updating a task successfully."
          headers:
            Location:
              schema:
                type: string
              examples:
                302Success:
                  description: "Updated successfully. Redirect to task page (UI09)."
                  value: "/projects/{project}/tasks/{id}"
                302Failure:
                  description: "Failed update. Redirect to task update form with an error message."
                  value: "/projects/{project}/tasks/{id}/edit"
        "404":
          description: "Task or project not found."
        "500":
          description: "Internal server error."

  /projects/{project}/tasks/{task}/edit:
    get:
      operationId: R305
      summary: "R305: Edit a task form"
      description: "Show task update form given a specific id. Access: COORD"
      tags:
        - "M03: Task Management"
      parameters:
        - in: path
          name: project
          schema:
            type: integer
          required: true
        - in: path
          name: task
          schema:
            type: integer
          required: true
      responses:
        "200":
          description: "Ok. Show task update form."
        "404":
          description: "Task or project not found."
        "500":
          description: "Internal server error."

  /projects/{project}/tasks/{task}/delete:
    delete:
      operationId: R307
      summary: "R307: Delete a specific task"
      description: "Delete a specific task given an id. Access: COORD"
      tags:
        - "M03: Task Management"
      parameters:
        - in: path
          name: project
          schema:
            type: integer
          required: true
        - in: path
          name: task
          schema:
            type: integer
          required: true
      responses:
        "302":
          description: "Redirect after deleting a task successfully."
          headers:
            Location:
              schema:
                type: string
              examples:
                302Success:
                  description: "Deleted successfully. Redirect to project tasks page."
                  value: "/projects/{project}/tasks"
                302Failure:
                  description: "Failed deletion. Redirect to project tasks page with an error message."
                  value: "/projects/{project}/tasks"
        "404":
          description: "Task not found."
        "500":
          description: "Internal server error."

  /members:
    post:
      operationId: R601
      summary: "R601: Add a user to a project"
      description: "Store new project member (add user to project). Access: COORD"
      tags:
        - "M06: Team and Membership Management"
      requestBody:
        required: true
        content:
          application/x-www-form-urlencoded:
            schema:
              type: object
              properties:
                project_id:
                  type: integer
                user_input:
                  type: string
              required:
                - project_id
                - user_input
      responses:
        "302":
          description: "Redirect after adding a user to a project successfully."
          headers:
            Location:
              schema:
                type: string
              examples:
                302Success:
                  description: "Added successfully. Redirect to project page."
                  value: "/projects/{id}"
                302Failure:
                  description: "Failed addition. Redirect to project page with an error message."
                  value: "/members/create/{project_id}"
        "404":
          description: "Project not found."
        "500":
          description: "Internal server error."

  /members/create/{project_id}:
    get:
      operationId: R602
      summary: "R602: Show form to add User to Project."
      description: "Provide form for adding a user to a project. Access: COORD"
      parameters:
        - name: project_id
          in: path
          description: "Project ID"
          required: true
          schema:
            type: integer
      tags:
        - "M06: Team and Membership Management"
      responses:
        "200":
          description: "Ok. Show add user form."
        "404":
          description: "Project not found."
        "500":
          description: "Internal server error."

  /administrators-users:
    get:
      operationId: R901
      summary: "R901: List All Users"
      description: "List of all users on the platform. "
      tags:
        - "M09: Administrator Dashboard and User Management"
      responses:
        "200":
          description: "List of users retrieved successfully."
          content:
            application/json:
              schema:
                type: array
                items:
                  type: object
                  properties:
                    id:
                      type: integer
                    username:
                      type: string
                    name:
                      type: string
                    email:
                      type: string
                    isBlocked:
                      type: boolean
        "401":
          description: "Unauthorized access."
        "500":
          description: "Internal server error."

  /administrators-users/new:
    get:
      operationId: R902
      summary: "R902: Create User Form"
      description: "Get the form for creating a new user. "
      tags:
        - "M09: Administrator Dashboard and User Management"
      responses:
        "200":
          description: "Form for creating an user created successfully."
        "401":
          description: "Unauthorized access."
        "500":
          description: "Internal server error."

  /administrators-users/register:
    post:
      operationId: R903
      summary: "R903: Create a New User"
      description: "Submit a new user's details to create an account. "
      tags:
        - "M09: Administrator Dashboard and User Management"
      requestBody:
        required: true
        content:
          multipart/form-data:
            schema:
              type: object
              properties:
                username:
                  type: string
                name:
                  type: string
                email:
                  type: string
                password:
                  type: string
                photo:
                  type: string
                  format: binary
                isBlocked:
                  type: boolean
              required:
                - username
                - name
                - email
                - password
      responses:
        "201":
          description: "USer created successfully."
        "400":
          description: "Invalid input data."
        "409":
          description: "Conflict - username or email already exists."
        "500":
          description: "Internal server error."

  /administrators-users/{id}/profile:
    get:
      operationId: R904
      summary: "R904: Retrieve User Profile"
      description: "Retrieve details of a specific user by ID. "
      tags:
        - "M09: Administrator Dashboard and User Management"
      parameters:
        - name: id
          in: path
          required: true
          schema:
            type: integer
          description: "ID of the user to retrieve."
      responses:
        "200":
          description: "User profile retrieved successfully."
          content:
            application/json:
              schema:
                type: object
                properties:
                  id:
                    type: integer
                  username:
                    type: string
                  name:
                    type: string
                  email:
                    type: string
                  photo:
                    type: string
                  isBlocked:
                    type: boolean
        "404":
          description: "User not found."
        "500":
          description: "Internal server error."

  /administrators/{id}/profile/edit:
    get:
      operationId: R905
      summary: "R905: Render Edit User Form"
      description: "Render the form for editing an user's profile. "
      tags:
        - "M09: Administrator Dashboard and User Management"
      parameters:
        - name: id
          in: path
          required: true
          schema:
            type: integer
          description: "ID of the user to edit."
      responses:
        "200":
          description: "Form for editing an user profile rendered successfully."
        "404":
          description: "User not found."
        "401":
          description: "Unauthorized access."
        "500":
          description: "Internal server error."

  /administrators/{id}/update:
    put:
      operationId: R906
      summary: "R906: Update USer Profile"
      description: "Submit updates to an user's profile details. "
      tags:
        - "M09: Administrator Dashboard and User Management"
      parameters:
        - name: id
          in: path
          required: true
          schema:
            type: integer
          description: "ID of the user to update."
      requestBody:
        required: true
        content:
          multipart/form-data:
            schema:
              type: object
              properties:
                username:
                  type: string
                name:
                  type: string
                email:
                  type: string
                password:
                  type: string
                photo:
                  type: string
                  format: binary
                isBlocked:
                  type: boolean
      responses:
        "200":
          description: "User profile updated successfully."
        "400":
          description: "Invalid input data."
        "404":
          description: "User not found."
        "500":
          description: "Internal server error."

  /administrators/{id}:
    delete:
      operationId: R907
      summary: "R907: Delete User"
      description: "Delete an user's account by ID."
      tags:
        - "M09: Administrator Dashboard and User Management"
      parameters:
        - name: id
          in: path
          required: true
          schema:
            type: integer
          description: "ID of the user to delete."
      responses:
        "200":
          description: "User deleted successfully."
        "404":
          description: "User not found."
        "500":
          description: "Internal server error."

  /administrators/projects:
    get:
      operationId: R908
      summary: "R908: View Administrator Dashboard"
      description: "Retrieve data and render the administrator dashboard."
      tags:
        - "M09: Administrator Dashboard and User Management"
      responses:
        "200":
          description: "Dashboard data retrieved successfully."
        "401":
          description: "Unauthorized access."
        "500":
          description: "Internal server error."

  /administrators/{id}/delete:
    delete:
      operationId: R909
      summary: "R909: Delete Own Profile"
      description: "Delete the currently logged-in administrator's profile."
      tags:
        - "M09: Administrator Dashboard and User Management"
      parameters:
        - name: id
          in: path
          required: true
          schema:
            type: integer
          description: "ID of the administrator to delete (must match authenticated user)."
      responses:
        "200":
          description: "Administrator's own profile deleted successfully."
        "401":
          description: "Unauthorized access."
        "403":
          description: "Forbidden - cannot delete another administrator's profile."
        "500":
          description: "Internal server error."

  /api/projects/{project_id}/tasks:
    get:
      operationId: R801
      summary: "R801: Search and Filter Tasks API"
      tags:
        - "M08: Search and Filtering"
      parameters:
        - in: path
          name: project_id
          description: Id of the project to search.
          required: true
          schema:
            type: integer
        - in: query
          name: q
          description: String for Full Text Search.
          schema:
            type: string
        - in: query
          name: c
          description: Search only the completed tasks.
          schema:
            type: string
        - in: query
          name: a
          description: Filter by user who is assigned.
          schema:
            type: integer
        - in: query
          name: dd
          description: Filter by the date a task is due.
          schema:
            type: string
      responses:
        "200":
          description: There were matches for the query.
          content:
            application/json:
              schema:
                type: array
                items:
                  type: object
                  properties:
                    id:
                      type: integer
                    title:
                      type: string
                    is_completed:
                      type: string
                    name:
                      type: string
                    dueDate:
                      type: string
        "204":
          description: There are no such results that match the requirements.
        "500":
          description: "Internal server error."

  /api/users:
    get:
      operationId: R802
      summary: "R802: Search and Filter Users API"
      tags:
        - "M08: Search and Filtering"
      parameters:
        - in: query
          name: q
          description: String for Full Text Search
          schema:
            type: string
        - in: query
          name: p
          description: Search only users present in a specific project.
          schema:
            type: integer
        - in: query
          name: b
          description: Filter off users who are blocked.
          schema:
            type: boolean
      responses:
        "200":
          description: There were matches for the query.
          content:
            application/json:
              schema:
                type: array
                items:
                  type: object
                  properties:
                    id:
                      type: integer
                    isBlocked:
                      type: boolean
                    name:
                      type: string
                    photo:
                      type: string
        "204":
          description: There are no such results that match the requirements.
        "500":
          description: "Internal server error."
```

---

## A8: Vertical prototype

This vertical prototype aims to be a proof of concept for the vision of SyncTask, the technologies and architecture selected.

### 1. Implemented Features

#### 1.1. Implemented User Stories

| User Story reference | Name                         | Priority | Description                                                                                                                                                |
| -------------------- | ---------------------------- | -------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------- |
| US.001               | Login                        | High     | As a user, I want to enter my account so that I can access my projects and platform                                                                        |
| US.004               | Logout                       | High     | As a user, I want to log out of my account so that I can control when my session is open or not                                                            |
| US.005               | Register                     | High     | As a user, I want to create a new account so that I can start using the platform                                                                           |
| US.201               | Create Project               | High     | As an authenticated user, I want to create a new project so that I can organize tasks and collaborate with others                                          |
| US.202               | View My Projects             | High     | As an authenticated user, I want to view all the projects I’m part of so that I can easily manage my work                                                  |
| US.203               | View and Edit my Profile     | High     | As an authenticated user, I want to see and modify my public profile, including my name and email, so that everyone has an updated description of who I am |
| US.301               | Create Task                  | High     | As a project member, I want to create tasks so that I can define the work that needs to be done                                                            |
| US.302               | Manage Tasks                 | High     | As a project member, I want to update task details like priority, labels, and due dates so that tasks stay organized and relevant                          |
| US.303               | View Task Details            | High     | As a project member, I want to view task details so that I understand what’s required for each task                                                        |
| US.304               | Search Tasks                 | High     | As a project member, I want to search for specific tasks (exact match or full-text) so that I can quickly find what I’m looking for                        |
| US.305               | Complete an Assigned Task    | High     | As a project member, I want to mark my assigned tasks as complete so that the team knows they’re done                                                      |
| US.501               | Add User to Project          | High     | As a project coordinator, I want to add new members to the project so that I can build a complete team                                                     |
| US.601               | See and Search User Accounts | High     | As an administrator, I want to see and search for user accounts so that I can supervise the platform more easily                                           |
| US.605               | Force Edit User Accounts     | High     | As an administrator, I want to force edit user accounts so that I can enforce the product's terms and conditions                                           |
| US.606               | Create User Accounts         | High     | As an administrator, I want to create new accounts so that I can manage who has access to the platform                                                     |

#### 1.2. Implemented Web Resources

> Module M01: Authentication and Profile Management

| Web Resource Reference  | URL                                             |
| ----------------------- | ----------------------------------------------- |
| R101: Login Form        | GET [/login](http://localhost:8001/login)       |
| R102: Login Action      | POST /login                                     |
| R103: Logout Action     | POST /logout                                    |
| R104: Registration Form | GET [/register](http://localhost:8001/register) |
| R105: Register New User | POST /register                                  |

> Module M02: Project Management

| Web Resource Reference          | URL                                                            |
| ------------------------------- | -------------------------------------------------------------- |
| R201: Show projects of the user | GET [/projects](http://localhost:8001/projects)                |
| R202: Create New Project Form   | GET [/projects/create](http://localhost:8001/projects/create)  |
| R203: Store a new project       | POST /projects                                                 |
| R204: Show a specific project   | GET [/projects/{project_id}](http://localhost:8001/projects/1) |

> Module M03: Task Management

| Web Resource Reference           | URL                                                                                      |
| -------------------------------- | ---------------------------------------------------------------------------------------- |
| R301: Get all tasks of a project | GET [/projects/{project}/tasks](http://localhost:8001/projects/1/tasks)                  |
| R302: Create New Task Form       | GET [/projects/{project}/tasks/create](http://localhost:8001/projects/1/tasks/create)    |
| R303: Store a new task           | POST /projects/{project}/tasks                                                           |
| R304: Show a specific task       | GET [/projects/{project}/tasks/{task}](http://localhost:8001/projects/1/tasks/1)         |
| R305: Edit task form             | GET [/projects/{project}/tasks/{id}/edit](http://localhost:8001/projects/1/tasks/1/edit) |
| R306: Update a specific task     | PATCH /projects/{project}/tasks/{task}                                                   |
| R307: Delete a specific task     | DELETE /projects/{project}/tasks/{task}/delete                                           |

> Module M06: Team and Membership Management

| Web Resource Reference                            | URL                                                                        |
| ------------------------------------------------- | -------------------------------------------------------------------------- |
| R601: Add a user to a project                     | POST /members                                                              |
| R602: Show form to add User to Project **[TODO]** | GET [/members/create/{project_id}](http://localhost:8001/members/create/1) |

> Module M08: Search and Filtering

| Web Resource Reference            | URL                                                                             |
| --------------------------------- | ------------------------------------------------------------------------------- |
| R801: Search and Filter Tasks API | GET [/api/projects/{project}/tasks](http://localhost:8001/api/projects/1/tasks) |

> Module M09: Administrator Dashboard and User Management

| Web Resource Reference             | URL                                                                                                      |
| ---------------------------------- | -------------------------------------------------------------------------------------------------------- |
| R901: List All Users               | GET [/administrators-users](http://localhost:8001/administrators-users)                                  |
| R902: Create User Form             | GET [/administrators-users/new](http://localhost:8001/administrators-users/new)                          |
| R903: Create a New User            | POST /administrators-users/register                                                                      |
| R904: Retrieve User Profile        | GET [/administrators-users/{id}/profile](http://localhost:8001/administrators-users/1/profile)           |
| R905: Render Edit User Form        | GET [/administrators-users/{id}/profile/edit](http://localhost:8001/administrators-users/1/profile/edit) |
| R906: Update User Profile          | PUT /administrators-users/{id}/update                                                                    |
| R907: Delete User                  | DELETE /administrators/{id}                                                                              |
| R908: View Administrator Dashboard | GET [/administrators/projects](http://localhost:8001/administrators/projects)                            |
| R909: Delete Own Profile           | DELETE /administrators/{id}/delete                                                                       |

### 2. Prototype

The prototype is available as a Docker Image at the [GitLab's Registry Container](https://gitlab.up.pt/lbaw/lbaw2425/lbaw24023/container_registry) and its source code is available at https://gitlab.up.pt/lbaw/lbaw2425/lbaw24023/-/tree/EAP?ref_type=tags.

Here are the instructions to run the prototype on a local machine:

1. Download the Docker Image:

```bash
docker run -d --name lbaw24023 -p 8001:80 gitlab.up.pt:5050/lbaw/lbaw2425/lbaw24023
```

2. Access the prototype at [http://localhost:8001](http://localhost:8001)

3. Login to SyncTask using one of the following credentials:
   - **Administrator**: admin@fe.up.pt / password
   - **Regular User**: user@fe.up.pt / password

---

## Revision history

> This is the first submission; changes made after this submission will be placed here.

---

GROUP2423, 28/11/2024

- Duarte Souto Assunção, up202208319@up.pt
- Guilherme Duarte Silva Matos, up202208755@up.pt
- João Vítor da Costa Ferreira, up202208393@up.pt (Editor)
- Pedro Afonso Nunes Fernandes, up202207987@up.pt
