<?php

namespace App\Http\Controllers;

use App\Models\Project;
use App\Models\ProjectCoordinator;
use App\Models\Suggested;
use App\Models\Task;
use App\Models\ProjectMember;
use App\Models\User;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Auth;
use Illuminate\Support\Facades\DB;
use Illuminate\View\View;
use Illuminate\Http\RedirectResponse;

class SuggestedController extends Controller
{
    /**
     * Display all suggestions for a given task to the Project Coordinator
     */
    public function index(int $project_id, int $task_id): View | RedirectResponse
    {
        if (!ProjectCoordinator::isCoordinator($project_id, Auth::user()->id)) {
            return to_route('tasks.show', ['project' => $project_id, 'task' => $task_id])
                ->withErrors(['error' => 'You do not have permission to view this page.'])
                ->onlyInput('error');
        }

        $project = Project::findorFail($project_id);
        $task = Task::findorFail($task_id);
        $suggestions = Suggested::where('task_id', $task_id)
            ->join('project_members', 'project_members.id', '=', 'suggested.project_member_id')
            ->join('users', 'project_members.user_id', '=', 'users.id')
            ->select('suggested.id', 'users.name', 'users.id as user_id')
            ->get();

        return view('suggested.index', compact('suggestions', 'project', 'task'));
    }

    /**
     * Form for a Project Member to suggest a Project Member as assignee
     */
    public function create(int $project_id, int $task_id): View | RedirectResponse
    {
        if (ProjectCoordinator::isCoordinator($project_id, Auth::user()->id)) {
            return to_route('tasks.show', ['project' => $project_id, 'task' => $task_id])
                ->with('error', 'You are a Project Coordinator, try to assign the task directly.');
        }

        $project = Project::findorFail($project_id);
        $task = Task::findorFail($task_id);
        $members = ProjectMember::join('users', 'project_members.user_id', '=', 'users.id')
            ->where('project_id', $project_id)
            ->select('project_members.id', 'users.name')
            ->get();
        return view('suggested.create', compact('project', 'task', 'members'));
    }

    /**
     * Store the response of the 'create' form
     */
    public function store(int $projectId, int $taskId, Request $request): RedirectResponse
    {
        $request->validate([
            'project_id' => ['required', 'exists:projects,id'],
            'project_member_id' => ['required', 'exists:project_members,id'],
            'task_id' => ['required', 'exists:tasks,id'],
        ]);

        $suggested = new Suggested();
        $suggested->task_id = $request->input('task_id');
        $suggested->project_member_id = $request->input('project_member_id');
        $suggested->save();

        return to_route('tasks.show', ['task' => $taskId, 'project' => $projectId])
            ->with('success', 'Suggestion created successfully.');
    }

    /**
     * Decline a suggestion, as stated by the Project Coordinator
     */
    public function destroy(int $projectId, int $taskId, int $suggestedId): RedirectResponse
    {
        Suggested::where("id", $suggestedId)
            ->delete();

        return to_route('suggested.index', ['project' => $projectId, 'task' => $taskId])
            ->with('success', 'Suggestion declined successfully.');
    }

    /**
     * Turn a suggestion into an assignee and delete all other suggestions
     */
    public function accept(int $projectId, int $taskId, int $suggestedId): RedirectResponse
    {
        $task = Task::find($taskId);
        $projectMemberSuggested = Suggested::where('id', $suggestedId)
            ->first()
            ->project_member_id;
        
        $task->update(['project_member_id' => $projectMemberSuggested]);
        $task->save();

        Suggested::where('task_id', $taskId)
            ->delete();

        return to_route('tasks.show', ['project' => $projectId, 'task' => $taskId])
            ->with('success', 'Suggestion accepted successfully.');
    }
}
