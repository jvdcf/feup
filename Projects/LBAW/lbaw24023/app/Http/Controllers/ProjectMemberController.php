<?php

namespace App\Http\Controllers;

use App\Models\Notification;
use App\Models\Project;
use App\Models\ProjectCoordinator;
use App\Models\ProjectMember;
use App\Models\User;
use Barryvdh\Debugbar\Facades\Debugbar;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Auth;
use Illuminate\Support\Facades\DB;

class ProjectMemberController extends Controller
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
    public function create(int $project_id)
    {
        if (Auth::check()) {
            $project = Project::where('id', $project_id)
                ->first();

            if (!$project) {
                return redirect()->route('project.index')
                    ->with('error', 'Project not found');
            }

            $project_member = ProjectMember::where('project_id', $project_id)
                ->where('user_id', Auth::user()->id)
                ->first();

            if (!$project_member) {
                return redirect()->route('project.index')
                    ->with('error', 'You are not a member of this project.');
            }

            if(!ProjectCoordinator::isCoordinator($project_id, Auth::user()->id)) {
                return redirect()->route('project.show', ['project' => $project_id])
                    ->with('error', 'You are not a coordinator of this project.');
            }

            return view('members.create', ['project' => $project]);
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
                'project_id' => ['required'],
                'user_input' => ['required']
            ]);

            $project = Project::where('id', $request->project_id)
                ->first();

            if (!$project) {
                return redirect()->route('project.index')
                    ->with('error', 'Project not found.');
            }

            $user = User::where(DB::raw('LOWER(username)'), '=', strtolower($request->user_input))
                ->orWhere(DB::raw('LOWER(email)'), '=', strtolower($request->user_input))
                ->first();

            if (!$user) {
                return back()->withErrors([
                    'user_input' => 'User not found.'
                ])->onlyInput('user_input');
            }

            if($user->id !== Auth::user()->id) {
                return redirect()->route('notifications.index')
                    ->with('error', 'You can only accept your own invitations.');
            }

            $project_member = ProjectMember::where('project_id', $request->project_id)
                ->where('user_id', $user->id)
                ->first();

            if ($project_member != null) {
                return back()->withErrors([
                    'user_input' => 'User already in project.'
                ])->onlyInput('user_input');
            }

            $project_member = new ProjectMember([
                'project_id' => $request->project_id,
                'user_id' => $user->id
            ]);

            $project_member->save();

            Notification::where('notif', 'project_invitation')
                ->where('project_id', $request->project_id)
                ->where('user_id', $user->id)
                ->delete();

            $coordinators = ProjectCoordinator::join('project_members', 'project_coordinators.project_member_id', '=', 'project_members.id')
                ->join('users', 'project_members.user_id', '=', 'users.id')
                ->where('project_members.project_id', $request->project_id)
                ->get();

            foreach ($coordinators as $coordinator) {
                $acceptedNotification = new Notification([
                    'notif' => 'invitation_accepted',
                    'project_id' => $request->project_id,
                    'user_id' => $coordinator->user_id,
                ]);
                $acceptedNotification->save();
            }

            return redirect()->route('project.show', ['project' => $request->project_id])
                ->with('success', 'Successfully joined project!');

        } else {
            return redirect()->route('login');
        }
    }

    public function invite(Request $request)
    {
        if (Auth::check()) {
            $request->validate([
                'project_id' => ['required'],
                'user_input' => ['required']
            ]);

            $project = Project::where('id', $request->project_id)
                ->first();

            if (!$project) {
                return redirect()->route('project.index')
                    ->with('error', 'Project not found.');
            }

            $user = User::where(DB::raw('LOWER(username)'), '=', strtolower($request->user_input))
                ->orWhere(DB::raw('LOWER(email)'), '=', strtolower($request->user_input))
                ->first();

            if (!$user) {
                return back()->withErrors([
                    'user_input' => 'User not found.'
                ])->onlyInput('user_input');
            }

            $project_member = ProjectMember::where('project_id', $request->project_id)
                ->where('user_id', $user->id)
                ->first();

            if ($project_member != null) {
                return back()->withErrors([
                    'user_input' => 'User already in project.'
                ])->onlyInput('user_input');
            }

            $invitation = Notification::where('notif', 'project_invitation')
                ->where('project_id', $request->project_id)
                ->where('user_id', $user->id)
                ->first();

            if ($invitation != null) {
                return back()->withErrors([
                    'user_input' => 'User already invited.'
                ])->onlyInput('user_input');
            }

            $invitation = new Notification([
                'notif' => 'project_invitation',
                'project_id' => $request->project_id,
                'user_id' => $user->id,
            ]);

            $invitation->save();
            return redirect()->route('project.show', ['project' => $request->project_id])
                ->with('success', 'Member added successfully.');

        } else {
            return redirect()->route('login');
        }
    }

    /**
     * Display the specified resource.
     */
    public function show(ProjectMember $projectMember)
    {
        //
    }

    /**
     * Show the form for editing the specified resource.
     */
    public function edit(ProjectMember $projectMember)
    {
        //
    }

    /**
     * Update the specified resource in storage.
     */
    public function update(Request $request, int $projectMemberId)
    {
        if (Auth::check()) {
            $request->validate([
                'is_favorite' => ['required', 'boolean']
            ]);

            $project_member = ProjectMember::find($projectMemberId);

            if(!$project_member) {
                return redirect()->route('project.index')
                    ->with('error', 'Project member doesn\'t exist.');
            }

            if ($project_member->user_id !== Auth::user()->id) {
                return redirect()->route('project.index')
                    ->with('error', 'You are not authorized to update this member.');
            }

            $project_member->is_favorite = $request->is_favorite;
            $project_member->save();
            return redirect()->route('project.show', ['project' => $project_member->project_id])
                ->with('success', 'Project marked as favorite.');
        } else {
            return redirect()->route('login');
        }
    }

    /**
     * Remove the specified resource from storage.
     */
    public function destroy(int $projectMemberId)
    {
        if (Auth::check()) {
            $project_member = ProjectMember::findOrFail($projectMemberId);
            if (ProjectCoordinator::isCoordinator($project_member->project_id, Auth::id()) 
                || $project_member->user_id === Auth::id()) {
                Notification::where("project_id", $project_member->project_id)
                    ->where("user_id", $project_member->user_id)
                    ->delete();
                
                $project_member->user_id = null;
                $project_member->save();
                
                if (ProjectCoordinator::isCoordinator($project_member->project_id, Auth::id())) {
                    return redirect()->back()
                        ->with('success', 'Member removed successfully.');   
                } else {
                    return redirect()->route('project.index')->with('success', 'Left successfully.');
                }
                
            }
            return redirect()->route('project.show', ['project' => $project_member->project_id])
                ->with('error', 'You are not authorized to remove this member.');
        } else {
            return redirect()->route('login');
        }
    }
}
