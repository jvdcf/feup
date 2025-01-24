<?php

namespace App\Http\Middleware;

use App\Models\ProjectCoordinator;
use App\Models\ProjectMember;
use Closure;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Auth;
use Symfony\Component\HttpFoundation\Response;

class Coordinator
{
    /**
     * Handle an incoming request.
     *
     * @param  \Closure(\Illuminate\Http\Request): (\Symfony\Component\HttpFoundation\Response)  $next
     */
    public function handle(Request $request, Closure $next): Response
    {
        if (Auth::guard('admin')->check()) {
            return redirect()->route('administrator.list-projects')->withErrors(['error' => 'You are not a member of this project.']);
        }

        $projectId = $request->route()->parameter('project') ?? $request->project_id;

        if (is_null($projectId)) {
            $project_member_id = $request->route()->parameter('member_id');

            $project_member = ProjectMember::find($project_member_id);

            if(!$project_member) {
                return redirect()->route('project.index')->withErrors(['error' => 'Invalid project member.']);
            }

            $projectId = $project_member->project_id;
        }

        $projectId = (int) $projectId;

        $project_member = ProjectMember::where('project_id', $projectId)
            ->where('user_id', Auth::user()->id)
            ->first();

        if(!$project_member) {
            return redirect()->route('project.index')->withErrors(['error' => 'You are not a member of this project.']);
        }

        $project_coordinator = ProjectCoordinator::where('project_member_id', $project_member->id)
            ->first();

        if(!$project_coordinator) {
            return redirect()->route('project.show', ['project' => $projectId])->withErrors(['error' => 'You are not a coordinator of this project.']);
        }

        return $next($request);
    }
}
