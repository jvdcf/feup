<?php

namespace App\Http\Controllers;

use App\Models\Notification;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Auth;
use Illuminate\Support\Facades\DB;
use App\Models\ProjectMember;
use App\Models\TaskNotification;

class NotificationController extends Controller
{
    /**
     * Display a listing of the notifications.
     * @note This will also list the task notifications..
     */
    public function index()
    {
        if (Auth::check()) {
            $notifications = Notification::where('user_id', Auth::user()->id)
                ->select('notifications.id as notification_id', '*')
                ->join('projects', 'notifications.project_id', '=', 'projects.id')
                ->orderBy('notifications.sent_date', 'DESC')
                ->get();

            $taskNotifications = TaskNotification::where('user_id', Auth::user()->id)
                ->select('task_notifications.id as notification_id', '*')
                ->join('projects', 'task_notifications.project_id', '=', 'projects.id')
                ->join('tasks', 'task_notifications.task_id', '=', 'tasks.id')
                ->orderBy('task_notifications.sent_date', 'DESC')
                ->get();

            $notifications = $notifications
                ->merge($taskNotifications)
                ->sortByDesc('sent_date');

            return view('notifications.index', ['notifications' => $notifications]);
        } else {
            return redirect()->route('login');
        }
    }

    /**
     * Show the form for creating a new resource.
     */
    public function create()
    {
        //
    }

    /**
     * Store a newly created resource in storage.
     */
    public function store(Request $request)
    {
        //
    }

    /**
     * Display the specified resource.
     */
    public function show(Notification $coordinatorChangeNotification)
    {
        //
    }

    /**
     * Show the form for editing the specified resource.
     */
    public function edit(Notification $coordinatorChangeNotification)
    {
        //
    }

    /**
     * Update the specified resource in storage.
     */
    public function update(Request $request, Notification $coordinatorChangeNotification)
    {
        //
    }

    /**
     * Remove the specified resource from storage.
     */
    public function destroy(int $notificationId)
    {
        if (Auth::check()) {
            $notification = Notification::find($notificationId);

            if (!$notification) {
                return redirect()->route('notifications.index')
                    ->with('error', 'Notification not found.');
            }

            if($notification->user_id !== Auth::user()->id) {
                return redirect()->route('notifications.index')
                    ->with('error', 'You are not authorized to delete this notification.');
            }

            $notification->delete();

            return redirect()->route('notifications.index')
                ->with('success', 'Notification deleted successfully.');
        } else {
            return redirect()->route('login');
        }
    }


    public function accept(int $notificationId) {
        if (Auth::check()) {
            $notification = Notification::find($notificationId);
            if (!$notification) {
                return redirect()->route('notifications.index')
                    ->with('error', 'Notification not found.');
            }
            
            if ($notification->user_id !== Auth::id()) {
                return redirect()->route('notifications.index')
                    ->with('error', 'You are not authorized to accept this invite.');
            }

            if ($notification->notif !== 'project_invitation') {
                return redirect()->route('notifications.index')
                    ->with('error', 'This notification is not a project invitation.');
            }
            
            $project_id = $notification->project_id;
            $user_id = Auth::id();

            $new_member = new ProjectMember;
            $new_member->user_id = $user_id;
            $new_member->project_id = $project_id;
            $new_member->save();

            $notification->delete();

            return to_route("project.show", compact('project_id'));
        } else {
            return redirect()->route('login');
        }
    }
}
