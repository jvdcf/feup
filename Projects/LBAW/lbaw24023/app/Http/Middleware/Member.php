<?php

namespace App\Http\Middleware;

use App\Models\ProjectMember;
use Closure;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Auth;
use Symfony\Component\HttpFoundation\Response;

class Member
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

        $projectId = (int) $request->route()->parameter('project');

        $project_member = ProjectMember::where('project_id', $projectId)
            ->where('user_id', Auth::user()->id)
            ->first();

        if(!$project_member) {
            return redirect()->route('project.index')->withErrors(['error' => 'You are not a member of this project.']);
        }

        return $next($request);
    }
}
