<?php

namespace App\Http\Controllers;

use App\Models\TaskComment;
use App\Models\Task;
use App\Models\Project;
use App\Models\ProjectMember;
use Illuminate\Support\Facades\Auth;
use Illuminate\Http\Request;
use Illuminate\View\View;
use Illuminate\Http\RedirectResponse;
use Redirect;

class TaskCommentController extends Controller
{
    /**
     * Add a new task comment to the task database.
     */
    public function store(int $projectId, int $taskId, Request $request): RedirectResponse
    {
        $project = Project::findOrFail($projectId);
        $task = Task::findOrFail($taskId);
        if (!Auth::check()) return Redirect::route('login');
        $project_member = ProjectMember::where('user_id', Auth::user()->id)->first();
        
        $request->validate([
            'comment' => 'required|string|max:1000'
        ]);
        
        $comment = new TaskComment();
        $comment->comment = $request->input('comment');
        $comment->task_id = $task->id;
        $comment->project_member_id = $project_member->id;
        $comment->save();

        return to_route('tasks.show', compact('project', 'task'))
            ->with('success', 'Comment added successfully!');
    }

    /**
     * Update a specific comment in the task database.
     */
    public function update(int $projectId, int $taskId, int $commentId, Request $request): RedirectResponse
    {
        $project = Project::findOrFail($projectId);
        $task = Task::findOrFail($taskId);
        $comment = TaskComment::findOrFail($commentId);
        $pm_author = ProjectMember::findOrFail($comment->project_member_id);
        $pm_user = ProjectMember::where('user_id', Auth::user()->id)
            ->where('project_id', $projectId)
            ->first();

        if ($pm_author->id !== $pm_user->id) 
            return to_route('task.show', compact('project', 'task'))
                ->with('error', 'You are not allowed to edit this comment!');

        $request->validate([
            'comment' => 'required|string|max:1000'
        ]);

        $comment->comment = $request->input('comment');
        $comment->post_date = now();
        $comment->update();

        return to_route('tasks.show', compact('project', 'task'))
            ->with('success', 'Comment updated successfully!');
    }

    /**
     * Delete a task comment from the task database.
     */
    public function destroy(int $projectId, int $taskId, int $commentId): RedirectResponse
    {
        $project = Project::findOrFail($projectId);
        $task = Task::findOrFail($taskId);
        $comment = TaskComment::findOrFail($commentId);
        $pm_author = ProjectMember::findOrFail($comment->project_member_id);
        $pm_user = ProjectMember::where('user_id', Auth::user()->id)
            ->where('project_id', $projectId)
            ->first();

        if ($pm_author->id !== $pm_user->id) 
            return to_route('task.show', compact('project', 'task'))
                ->with('error', 'You are not allowed to edit this comment!');
        
        $comment->delete();

        return to_route('tasks.show', compact('project', 'task'))
            ->with('success', 'Comment deleted successfully!');
    }
}
