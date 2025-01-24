<?php

namespace App\Http\Controllers;

use App\Models\TaskNotification;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Auth;

class TaskNotificationController extends Controller
{
    /**
     * Display a listing of the resource.
     */
    public function index()
    {
        //
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
    public function show(TaskNotification $taskAssignNotification)
    {
        //
    }

    /**
     * Show the form for editing the specified resource.
     */
    public function edit(TaskNotification $taskAssignNotification)
    {
        //
    }

    /**
     * Update the specified resource in storage.
     */
    public function update(Request $request, TaskNotification $taskAssignNotification)
    {
        //
    }

    /**
     * Remove the specified resource from storage.
     */
    public function destroy(int $notificationId)
    {
        if (Auth::check()) {
            $task_notification = TaskNotification::find($notificationId);

            if(!$task_notification) {
                return redirect()->route('notifications.index')
                    ->with('error', 'Notification not found.');
            }

            if($task_notification->user_id !== Auth::user()->id) {
                return redirect()->route('notifications.index')
                    ->with('error', 'You are not authorized to delete this notification.');
            }

            $task_notification->delete();

            return redirect()->route('notifications.index')
                ->with('success', 'Notification deleted successfully.');
        } else {
            return redirect()->route('login');
        }
    }
}
