<?php

namespace App\Http\Controllers;

use App\Models\Project;
use App\Models\ProjectCoordinator;
use App\Models\ProjectMember;
use App\Models\Task;
use App\Models\User;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Auth;
use Illuminate\Support\Facades\DB;

class ProjectController extends Controller
{

    public function showMembers($projectId)
    {
        $members = ProjectMember::join('users', 'project_members.user_id', '=', 'users.id')
            ->where('project_members.project_id', $projectId)
            ->select('users.*')
            ->get();

        $coordinators = ProjectMember::join('users', 'project_members.user_id', '=', 'users.id')
            ->join('project_coordinators', 'project_members.id', '=', 'project_coordinators.project_member_id')
            ->where('project_members.project_id', $projectId)
            ->select('users.*')
            ->get();

        return view('administrator.show-members', compact('members', 'coordinators'));
    }

    public function getProjectMembers($projectId)
    {
        $members = ProjectMember::join('users', 'project_members.user_id', '=', 'users.id')
            ->where('project_members.project_id', $projectId)
            ->select('users.id', 'users.name', 'users.email')
            ->get();

        return response()->json($members);
    }

    public function getProjectCoordinators($projectId)
    {
        $coordinators = ProjectMember::join('users', 'project_members.user_id', '=', 'users.id')
            ->join('project_coordinators', 'project_members.id', '=', 'project_coordinators.project_member_id')
            ->where('project_members.project_id', $projectId)
            ->select('users.id', 'users.name', 'users.email')
            ->get();

        return response()->json($coordinators);
    }

    public function getAllProjectMembers()
    {
        $members = ProjectMember::join('users', 'project_members.user_id', '=', 'users.id')
            ->select('users.id', 'users.name', 'users.email', 'project_members.project_id')
            ->get();

        return response()->json($members);
    }

    public function getAllProjectCoordinators()
    {
        $coordinators = ProjectMember::join('users', 'project_members.user_id', '=', 'users.id')
            ->join('project_coordinators', 'project_members.id', '=', 'project_coordinators.project_member_id')
            ->select('users.id', 'users.name', 'users.email', 'project_members.project_id')
            ->get();

        return response()->json($coordinators);
    }

    public static function getUserProjects() {
         $user = Auth::user();
         $projects = ProjectMember::join('projects', 'project_members.project_id', '=', 'projects.id')
            ->where('project_members.user_id', $user->id)
             ->orderBy('project_members.is_favorite', 'desc')
            ->get();
        return $projects;    
    }    


    public function listProjects()
    {
        $projects = Project::all();
        return view('administrator.list-projects', ['projects' => $projects]);
    }

    public static function getCoordinators(int $project_id)
    {
        $coordinators = User::join('project_members', 'users.id', '=', 'project_members.user_id')
            ->join('project_coordinators', 'project_members.id', '=', 'project_coordinators.project_member_id')
            ->where('project_members.project_id', $project_id)
            ->select('users.*')
            ->get();

        return $coordinators;
    }

    public static function getMembers(int $project_id)
    {
        // Ignores all the null entries
        $members = ProjectMember::join('users', 'project_members.user_id', '=', 'users.id')
            ->where('project_members.project_id', $project_id)
            ->whereNotIn('users.id', self::getCoordinators($project_id)->pluck('id'))
            ->where('project_members.user_id', '<>', null)
            ->select('users.*')
            ->get();

        return $members;
    }

    /**
     * Display a listing of the resource.
     */
    public function index()
    {
        if (Auth::check()) {
            $projects = self::getUserProjects();
            return view('project.index', ['projects' => $projects]);
        } else {
            return redirect()->route('login');
        }
    }

    /**
     * Show the form for creating a new resource.
     */
    public function create()
    {
        if (Auth::check()) {
            return view('project.create');
        } else {
            return redirect()->route('login');
        }
    }

    /**
     * Store a newly created resource in storage.
     */
    public function store(Request $request)
    {
        if (Auth::check()) {
            $request->validate([
                'name' => ['required'],
                'detail' => ['required']
            ]);

            $project = new Project($request->all());
            $project->save();

            $project_id = $project->id;
            $user_id = Auth::user()->id;
            $project_member = new ProjectMember([
                'project_id' => $project_id,
                'user_id' => $user_id
            ]);
            $project_member->save();

            $project_coordinator = new ProjectCoordinator([
                'project_member_id' => $project_member->id
            ]);
            $project_coordinator->save();

            return redirect()->route('project.show', ['project' => $project_id])
                ->with('success', 'Project created successfully.');
        } else {
            return redirect()->route('login');
        }
    }

    /**
     * Display the specified resource.
     */
    public function show(int $project_id)
    {
        if (Auth::check()) {
            $project = Project::find($project_id);

            if (!$project) {
                return redirect()->route('project.index')
                    ->with('error', 'Project not found.');
            }

            $project_member = ProjectMember::where('project_id', $project->id)
                ->where('user_id', Auth::user()->id)
                ->first();

            if (!$project_member) {
                return redirect()->route('project.index')
                    ->with('error', 'You are not a member of this project.');
            }

            return view('project.show', ['project' => $project]); // 'members' => $members, 'coordinators' => $coordinators
        } else {
            return redirect()->route('login');
        }
    }

    /**
     * Show the form for editing the specified resource.
     */
    public function edit(Project $project)
    {
        //
    }

    /**
     * Update the specified resource in storage.
     */
    public function update(Request $request, int $projectId)
    {
        if (Auth::check()) {
            if (Auth::guard('admin')->check()) {
                return redirect()->route('administrator.list-projects')
                    ->with('error', 'You are not allowed to update projects.');
            }

            $request->validate([
                'detail' => ['required']
            ]);

            $project = Project::find($projectId);

            if (!$project) {
                return redirect()->route('project.index')
                    ->with('error', 'Project not found.');
            }

            $project_member = ProjectMember::where('user_id', Auth::user()->id)
                ->where('project_id', $projectId)
                ->first();

            if (!$project_member) {
                return redirect()->route('project.index')
                    ->with('error', 'You are not a member of this project.');
            }

            $project->detail = $request->detail;
            $project->save();

            return redirect()->route('project.show', ['project' => $projectId])
                ->with('success', 'Project updated successfully.');
        } else {
            return redirect()->route('login');
        }
    }

    /**
     * Remove the specified resource from storage.
     */
    public function destroy(Project $project)
    {
        //
    }

    public function archive(int $projectId) {
        $project = Project::findOrFail($projectId);
        $project->is_archived = true;
        $project->save();
        return redirect()->back();
    }

    public function getProjectStatistics(int $projectId)
    {
        $task_count = Task::join('project_members', 'tasks.project_member_id', '=', 'project_members.id')
            ->join('users', 'project_members.user_id', '=', 'users.id')
            ->where('tasks.project_id', $projectId)
            ->where('tasks.is_completed', true)
            ->select('users.name AS name', DB::raw('COUNT(tasks.id) AS tasks_completed'))
            ->groupBy('name')
            ->get();

        if (!$task_count) {
            return response()->json(['error' => 'No task completed yet.']);
        }

        $barLabels = array();
        $barData = array();
        foreach ($task_count as $c) {
            $barLabels[] = $c->name;
            $barData[] = $c->tasks_completed;
        }

        $completed = Task::where('is_completed', true)
            ->where('project_id', $projectId)
            ->count();

        $not_completed = Task::where('is_completed', false)
            ->where('project_id', $projectId)
            ->count();

        return response()->json([
            "barLabels" => $barLabels,
            "barData" => $barData,
            "doughnutLabels" => ["Completed", "Not Completed"],
            "doughnutData" => [$completed, $not_completed]
        ]);
    }
}
