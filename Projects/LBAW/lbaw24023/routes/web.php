<?php

use App\Http\Controllers\NotificationController;
use App\Http\Controllers\ProjectController;
use Illuminate\Support\Facades\Route;

use App\Http\Controllers\ProjectMemberController;
use App\Http\Controllers\ProjectCoordinatorController;
use App\Http\Controllers\SuggestedController;
use App\Http\Controllers\TaskNotificationController;
use App\Http\Controllers\TaskCommentController;
use App\Http\Controllers\TaskController;
use App\Http\Controllers\UserController;
use App\Http\Controllers\GuestController;

use App\Http\Controllers\Auth\LoginController;
use App\Http\Controllers\Auth\RegisterController;
use App\Http\Controllers\ProfileController;
use App\Http\Controllers\AdministratorController;
use App\Http\Controllers\SideBarController;

use Illuminate\Support\Facades\Mail;
use App\Models\SendMail;
use Illuminate\Http\Request;
use App\Http\Controllers\EmailController;
use Illuminate\Support\Facades\Auth;


/*
|--------------------------------------------------------------------------
| Web Routes
|--------------------------------------------------------------------------
|
| Here is where you can register web routes for your application. These
| routes are loaded by the RouteServiceProvider and all of them will
| be assigned to the "web" middleware group. Make something great!
|
*/

// Guest/Public Pages
Route::controller(GuestController::class)->group(function () {
    Route::get('/', 'root')->name('guest.root');
    Route::get('/features', 'features')->name('guest.features');
    Route::get('/about', 'about')->name('guest.about');
    Route::get('/contacts', 'contacts')->name('guest.contacts');
});

Route::controller(EmailController::class)->group(function () {
    Route::post('/send-email', 'emailResetPassword')->name('password.email');
    Route::post('/send-email/{user}', 'emailResetPassword')->name('password.email1');
});

// Authentication
Route::controller(LoginController::class)->group(function () {
    Route::get('/login', 'showLoginForm')->name('login');
    Route::post('/login', 'authenticate');
    Route::get('/logout', 'logout')->name('logout');
});

Route::controller(RegisterController::class)->group(function () {
    Route::get('/register', 'showRegistrationForm')->name('register');
    Route::post('/register', 'register');
});

Route::controller(AdministratorController::class)->group(function () {
    Route::get('/administrators-users', 'listAdministrators')->name('administrator.index')->middleware('auth:admin');
    Route::get('/administrators-users/new', 'createAdministrator')->name('administrator.create')->middleware('auth:admin');
    Route::post('/administrators-users/register', 'storeAdministrator')->name('administrator.store')->middleware('auth:admin');
    Route::get('/administrators-users/{administrator}/profile', 'showProfile')->name('administrator.show')->middleware('auth:admin');
    Route::get('/administrators/{administrator}/profile/edit', 'editAdministratorProfile')->name('administrator.edit')->middleware('auth:admin');
    Route::put('/administrators/{administrator}/update', 'updateAdministratorProfile')->name('administrator.update')->middleware('auth:admin');
    Route::delete('/administrators/{administrator}', 'deleteProfile')->name('administrator.destroy')->middleware('auth:admin');
    Route::get('/administrators/dashboard', 'dashboard')->name('admin.dashboard')->middleware('auth:admin');
    Route::delete('/administrators/{administrators}/delete', 'deleteOwnProfile')->name('admin.destroy.own')->middleware('auth:admin');
});

Route::controller(UserController::class)->group(function () {
    Route::get('/users', 'listUsers')->name('users.index')->middleware('auth:admin');
    Route::get('/users/create', 'createUser')->name('users.create')->middleware('auth:admin');
    Route::post('/users', 'storeUser')->name('users.store')->middleware('auth:admin');
    Route::get('/users/{user}', 'showUser')->name('users.show')->middleware(['auth.some', 'show.profile']);
    Route::get('/users/{user}/edit', 'editUser')->name('users.edit')->middleware(['auth.some', 'user']);
    Route::put('/users/{user}', 'updateUser')->name('users.update')->middleware(['auth.some', 'user']);
    Route::delete('/users/{user}', 'destroyUser')->name('users.delete')->middleware(['auth.some', 'user']);
    Route::delete('/users/{user}/self', 'deleteOwnUser')->name('users.destroy.own')->middleware(['auth.some', 'user']);
    Route::patch('/users/{user}/toggle-block', 'toggleBlock')->name('users.toggleBlock')->middleware('auth:admin');
    Route::get('/api/members/users/{user}', 'searchUser')->name('members.searchUsers')->middleware('auth');
    Route::get('/storage/photos/{filename}', 'getPhoto')->name('photos.show')->middleware('auth.some');
    Route::delete('/user/photo', 'deletePhoto')->name('user.photo.delete')->middleware('auth');

    Route::get('/reset-page', 'showResetPage')->name('password.reset');
    Route::put('/reset-page/{user}', 'resetPassword')->name('password.reset.post');

    Route::get('/password/reset/{user}', 'showResetPage')->name('password.reset.form');
});

Route::controller(ProjectController::class)->group(function () {
    Route::get('/administrator/dashboard/list-projects', 'listProjects')->name('administrator.list-projects')->middleware('auth:admin');
    Route::get('/projects/{project}/members', 'showMembers')->name('administrator.show-members')->middleware('auth:admin');
    Route::post('/projects/{project}/archive', 'archive')->name('project.archive')->middleware(['auth', 'member']);
    Route::get('/projects', 'index')->name('project.index')->middleware('auth');
    Route::get('/projects/{project}', 'show')->where('project', '[0-9]+')->name('project.show')->middleware(['auth', 'member']);
    Route::get('/projects/create', 'create')->name('project.create')->middleware('auth');
    Route::post('/projects', 'store')->name('project.store')->middleware('auth');
    Route::get('/api/projects/members', 'getAllProjectMembers')->name('get.projects.all.members')->middleware('auth:admin');
    Route::get('/api/projects/coordinators', 'getAllProjectCoordinators')->name('get.projects.all.coordinators')->middleware('auth:admin');
    Route::patch('/projects/{project}', 'update')->where('project', '[0-9]+')->name('project.update')->middleware(['auth', 'coordinator']);
    Route::get('/api/projects/{project}/statistics', 'getProjectStatistics')->where('project', '[0-9]+')->name('get.projects.statistics')->middleware(['auth', 'member']);
});

// M03: Task Management
Route::controller(TaskController::class)->group(function () {
    Route::get('/projects/{project}/tasks', 'index')->name('tasks.index')->middleware(['auth', 'member']);
    Route::get('/projects/{project}/tasks/create', 'create')->name('tasks.create')->middleware(['auth']);
    Route::post('/projects/{project}/tasks', 'store')->name('tasks.store')->middleware(['auth']);
    Route::get('/projects/{project}/tasks/{task}', 'show')->name('tasks.show')->middleware(['auth', 'member']);
    Route::get('/projects/{project}/tasks/{task}/edit', 'edit')->name('tasks.edit')->middleware(['auth', 'coordinator']);
    Route::patch('/projects/{project}/tasks/{task}', 'update')->name('tasks.update')->middleware(['auth', 'coordinator']);
    Route::delete('/projects/{project}/tasks/{task}', 'destroy')->name('tasks.destroy')->middleware(['auth', 'coordinator']);

    Route::get('/api/projects/{project}/tasks', 'search')->name('tasks.search')->middleware(['auth', 'member']);
    Route::get('/projects/{project}/tasks/{task}/complete', 'complete')->name('tasks.complete')->middleware(['auth']);
});

Route::controller(ProjectMemberController::class)->group(function () {
    Route::get('/members/create/{project}', 'create')->where('project', '[0-9]+')->name('members.create')->middleware(['auth', 'coordinator']);
    Route::post('/members', 'store')->name('members.store')->middleware('auth');
    Route::post('/notifications', 'invite')->name('members.invite')->middleware(['auth', 'coordinator']);
    Route::delete('/members/{member_id}', 'destroy')->where('id', '[0-9]+')->name('members.destroy')->middleware('auth');
    Route::patch('/members/{member_id}', 'update')->where('id', '[0-9]+')->name('members.update')->middleware('auth');
});

Route::controller(ProjectCoordinatorController::class)->group(function () {
    Route::post('/coordinators', 'store')->name('coordinators.store');
    Route::delete('/coordinators/{id}', 'destroy')->where('id', '[0-9]+')->name('coordinators.destroy');
});

Route::controller(NotificationController::class)->group(function () {
    Route::get('/notifications', 'index')->name('notifications.index')->middleware('auth');
    Route::delete('/notifications/{id}', 'destroy')->where('id', '[0-9]+')->name('notifications.destroy')->middleware('auth');
    Route::get('/notifications/{id}/accept', 'accept')->where('id', '[0-9]+')->name('notifications.accept_invite')->middleware('auth');
});

Route::controller(SuggestedController::class)->group(function () {
    Route::get('/projects/{project}/tasks/{task}/suggested', 'index')->name('suggested.index')->middleware(['auth', 'coordinator']);
    Route::get('/projects/{project}/tasks/{task}/suggested/create', 'create')->name('suggested.create')->middleware(['auth', 'member']);
    Route::post('/projects/{project}/tasks/{task}/suggested', 'store')->name('suggested.store')->middleware(['auth', 'member']);
    Route::delete('/projects/{project}/tasks/{task}/suggested/{id}/decline', 'destroy')->name('suggested.destroy')->middleware(['auth', 'coordinator']);
    Route::post('/projects/{project}/tasks/{task}/suggested/{id}/accept', 'accept')->name('suggested.accept')->middleware(['auth', 'coordinator']);
});

Route::controller(TaskNotificationController::class)->group(function () {
    Route::delete('/task-notifications/{id}', 'destroy')->where('id', '[0-9]+')->name('task_notifications.destroy')->middleware('auth');
});

Route::controller(TaskCommentController::class)->group(function () {
    Route::post('/projects/{project}/tasks/{task}/comments', 'store')->name('task_comments.store')->middleware(['auth', 'member']);
    Route::put('/projects/{project}/tasks/{task}/comments/{id}', 'update')->name('task_comments.update')->middleware(['auth', 'member']);
    Route::delete('/projects/{project}/tasks/{task}/comments/{id}', 'destroy')->name('task_comments.destroy')->middleware(['auth', 'member']);
});


