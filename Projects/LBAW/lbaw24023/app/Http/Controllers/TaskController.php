<?php

namespace App\Http\Controllers;

use App\Models\Project;
use App\Models\ProjectCoordinator;
use App\Models\Task;
use App\Models\TaskComment;
use App\Models\ProjectMember;
use App\Models\Suggested;
use App\Models\TaskNotification;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Auth;

class TaskController extends Controller
{
    /**
     * Display a listing of the resource.
     */
    public function index($projectId)
    {
        $tasks = Task::leftJoin('project_members', 'tasks.project_member_id', '=', 'project_members.id')
            ->leftJoin('users', 'users.id', '=', 'project_members.user_id')
            ->where('tasks.project_id', $projectId)
            ->select('tasks.id', 'tasks.title', 'tasks.due_date', 'tasks.is_completed', 'users.name')
            ->get();
        
        $users = ProjectMember::join('users', 'project_members.user_id', '=', 'users.id')
            ->where('project_members.project_id', $projectId)
            ->get();
        
        $curr_proj = Project::findOrFail($projectId);
        
        if (Auth::check()) {
            $can_create_task = ProjectMember::where('project_members.project_id', $projectId)
            ->where('project_members.user_id', Auth::id())
            ->count() > 0;    
        } else {
            $can_create_task = false; 
        }         
        return view('tasks.index', compact('tasks', 'users', 'curr_proj', 'can_create_task'));
    }

    /**
     * Show the form for creating a new resource.
     */
    public function create($projectId)
    {
        $project = Project::where('id', $projectId)
            ->first();

        if ($project === null) {
            return redirect()
                ->route('project.index')
                ->with('error', 'Project not found.');
        }

        $members = ProjectMember::join('users', 'project_members.user_id', '=', 'users.id')
            ->where('project_members.project_id', $projectId)
            ->select('project_members.id', 'users.name')
            ->get();

        return view('tasks.create', compact('project', 'members'));
    }

    /**
     * Store a newly created resource in storage.
     */
    public function store(Request $request): \Illuminate\Http\RedirectResponse
    {
        $request->validate([
            'title' => ['required', 'string'],
            'description' => ['required', 'string'],
            'project_id' => ['required', 'exists:projects,id'],
            'project_member_id' => ['nullable', 'exists:project_members,id'],
            'due_date' => ['nullable', 'date'],
            'is_completed' => ['nullable', 'boolean'],
        ]);

        $task = new Task($request->all());
        $task->save();
        $project = $task->project_id;

        // TODO: Change this route to tasks.show
        return to_route('tasks.index', compact('project'))
            ->with('success', 'Task created successfully.');
    }

    /**
     * Display the specified resource.
     */
    public function show($projectId, $taskId): \Illuminate\Foundation\Application|\Illuminate\Contracts\View\View|\Illuminate\Contracts\View\Factory|\Illuminate\Contracts\Foundation\Application|\Illuminate\Http\RedirectResponse
    {
        $project = Project::findOrFail($projectId);
        $task = Task::findOrFail($taskId);

        $assignedUser = ProjectMember::join('users', 'project_members.user_id', '=', 'users.id')
            ->where('project_members.id', $task->project_member_id)
            ->select('users.id', 'users.name')
            ->get()
            ->first();

        $isCoordinator = ProjectCoordinator::isCoordinator($projectId, Auth::user()->id);

        $comments = TaskComment::where('task_id', $taskId)
            ->join('project_members', 'task_comments.project_member_id', '=', 'project_members.id')
            ->leftJoin('users', 'project_members.user_id', '=', 'users.id')
            ->select('task_comments.id', 'task_comments.post_date', 'task_comments.comment', 'users.name', 'users.id as user_id')
            ->get();

        return view('tasks.show', compact('task', 'project', 'isCoordinator', 'assignedUser', 'comments'));
    }

    /**
     * Show the form for editing the specified resource.
     */
    public function edit($projectId, $taskId): \Illuminate\Foundation\Application|\Illuminate\Contracts\View\View|\Illuminate\Contracts\View\Factory|\Illuminate\Contracts\Foundation\Application|\Illuminate\Http\RedirectResponse
    {
        $task = Task::findOrFail($taskId);
        $project = Project::findOrFail($projectId);
        $members = ProjectMember::join('users', 'project_members.user_id', '=', 'users.id')
            ->where('project_members.project_id', $projectId)
            ->select('project_members.id', 'users.name')
            ->get();

        return view('tasks.edit', compact('task', 'project', 'members'));
    }

    /**
     * Update the specified resource in storage.
     */
    public function update(Request $request, $projectId, $taskId)
    {
        $request->validate([
            'name' => 'nullable|string',
            'description' => 'nullable|string',
            'project_member_id' => 'nullable|exists:project_members,id',
            'due_date' => 'nullable|date',
            'is_completed' => 'nullable|boolean',
        ]);

        $project = Project::findOrFail($projectId);
        $task = Task::findOrFail($taskId);
        $task->update($request->all());
        $task->save();

        return to_route('tasks.show', compact('project', 'task'))
            ->with('success', 'Task updated successfully.');
    }

    /**
     * Remove the specified resource from storage.
     */
    public function destroy($projectId, $taskId): \Illuminate\Http\RedirectResponse
    {
        $project = Project::findOrFail($projectId);
        $task = Task::findOrFail($taskId);

        if ($project->id != $task->project_id) {
            return redirect()
                ->route('tasks.index', compact('project'))
                ->with('error', 'Task not found in project.');
        }

        // TODO: Convert this into a trigger in SQL
        Suggested::where('task_id', $taskId)
            ->delete();
        TaskComment::where('task_id', $taskId)
            ->delete();
        TaskNotification::where('task_id', $taskId)
            ->delete();

        $task->delete();

        return to_route('tasks.index', compact('project'))
            ->with('success', 'Task deleted successfully.');
    }

    /**
     * Seach for a task
     */
    public function search(int $proj_id, Request $request) {
        $request->validate([
            // Query
            'q' => 'nullable|string',
            // Completed
            'c' => 'nullable|string',
            // Assigned to
            'a' => 'nullable|exists:project_members,id',
            // Due Date
            'dd' => 'nullable|date'         
        ]);

        $result = Task::where('tasks.project_id', '=', $proj_id);
        
        if ($request->filled('q')) {
            $result = $result->whereFullText('tasks.title', $request->string('q'));
        }
        if ($request->filled('c')) {
            $result = $result->where('tasks.is_completed', '=', $request->string('c'));
        }
        if ($request->filled('a')) {
            $result = $result->where('tasks.project_member_id', '=', $request->integer('a'));
        }
        if ($request->filled('dd')) {
            $result = $result->where('tasks.due_date', '<=', $request->date('dd'));
        }
        $result = $result
            ->leftJoin('project_members', 'project_members.id', '=', 'tasks.project_member_id')
            ->leftJoin('users', 'project_members.user_id', '=', 'users.id')
            ->select('tasks.id','tasks.title', 'users.name','tasks.is_completed', 'tasks.due_date');

        return response()->json(['tasks' => $result->get()]);
    }

    public function complete($projectId, $taskId) {
        $task = Task::findOrFail($taskId);
        $project = Project::findOrFail($projectId);

        $task->is_completed = true;
        $task->save();

        return to_route('tasks.show', compact('project', 'task'))
            ->with('success', 'Task marked as complete.');
    }
}
