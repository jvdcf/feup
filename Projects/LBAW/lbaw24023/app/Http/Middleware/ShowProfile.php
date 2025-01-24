<?php

namespace App\Http\Middleware;

use App\Models\ProjectMember;
use Closure;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Auth;
use Symfony\Component\HttpFoundation\Response;

class ShowProfile
{
    /**
     * Handle an incoming request.
     *
     * @param  \Closure(\Illuminate\Http\Request): (\Symfony\Component\HttpFoundation\Response)  $next
     */
    public function handle(Request $request, Closure $next): Response
    {
        if (Auth::guard('admin')->check()) {
            return $next($request);
        }

        $user_id = $request->route()->parameter('user')->id;

        if ($user_id == Auth::user()->id) {
            return $next($request);
        }

        $pms = ProjectMember::where('user_id', $user_id)->get();

        foreach ($pms as $pm1) {
            $pm2 = ProjectMember::where('project_id', $pm1->project_id)->where('user_id', Auth::user()->id)->first();
            if (!$pm2) continue;
            return $next($request);
        }

        return redirect()->route('project.index')->withErrors(['error' => 'You are not authorized to see this profile.']);
    }
}
