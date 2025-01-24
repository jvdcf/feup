<?php

namespace App\Http\Controllers;

use App\Models\Project;
use App\Models\SendMail;
use App\Models\ProjectMember;
use App\Models\User;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Auth;
use Illuminate\Support\Facades\DB;
use Illuminate\Support\Facades\Mail;
use Illuminate\Support\Facades\Storage;
use Symfony\Component\HttpFoundation\StreamedResponse;


class UserController extends Controller
{

    public function listUsers()
    {
        $users = User::all();
        return view('users.index', compact('users'));
    }

    public function createUser()
    {
        return view('administrator.create-user');
    }

    public function showUser(User $user)
    {
        $administrator = Auth::guard('admin')->check() ? Auth::guard('admin')->user() : null;
        return view('administrator.show-profile', compact('user', 'administrator'));
    }

    public function editUser(User $user)
    {
        $administrator = Auth::guard('admin')->check() ? Auth::guard('admin')->user() : null;
        return view('administrator.edit-profile', compact('user', 'administrator'));
    }

    public function getPhoto($filename): StreamedResponse
    {
        $path = 'photos/' . $filename;

        if (!Storage::disk('private')->exists($path)) {
            abort(404);
        }

        return Storage::disk('private')->response($path);
    }
    public function storeUser(Request $request)
    {
        $request->validate([
            'name' => 'required|string|max:255',
            'username' => 'required|string|max:255|unique:users',
            'email' => 'required|string|email|max:255|unique:users',
            'password' => 'required|min:8|confirmed',
            'photo' => 'nullable|image|max:2048',
        ]);

        $user = new User($request->except('password'));
        $user->password = bcrypt($request->input('password'));

        if ($request->hasFile('photo')) {
            $user->photo = $request->file('photo')->store('photos', 'private');
        }

        $user->save();

        return redirect()->route('users.index')->with('success', 'User created successfully.');
    }
    // app/Http/Controllers/UserController.php

    public function updateUser(Request $request, User $user)
    {
        $request->validate([
            'name' => 'required|string|max:255',
            'email' => 'required|string|email|max:255|unique:users,email,' . $user->id,
            'username' => 'required|string|max:255',
            'password' => 'nullable|min:8|confirmed',
            'photo' => 'nullable|image|max:2048',
        ]);

        try {
            $user->name = $request->input('name');
            $user->username = $request->input('username');
            $user->email = $request->input('email');

            if ($request->filled('password')) {
                $user->password = bcrypt($request->input('password'));
            }

            if ($request->input('delete_photo') == '1') {
                if ($user->photo) {
                    Storage::disk('private')->delete($user->photo); // Delete photo from private disk
                    $user->photo = null; // Set photo field to null
                }
            }

            if ($request->hasFile('photo')) {
                $user->photo = $request->file('photo')->store('photos', 'private');
            }

            $user->save();

            return redirect()->route('users.show', $user)->with('success', 'Profile updated successfully.');
        } catch (\Illuminate\Database\QueryException $e) {
            if ($e->errorInfo[0] == '23505') { // Unique violation
                $errorField = strpos($e->getMessage(), 'users_username_uk') !== false ? 'username' : 'email';
                $errorMessage = $errorField === 'username' ? 'Username already exists' : 'Email already has an account';
                return back()->withErrors([$errorField => $errorMessage])->withInput();
            }
            return back()->withErrors(['error' => 'An unexpected error occurred'])->withInput();
        }
    }


    public function showResetPage(User $user)
    {
        return view('users.reset-page', ['user' => $user, 'email' => $user->email]);
    }

    public function resetPassword(Request $request, User $user)
    {
        $request->validate([
            'name' => 'required|string|max:255',
            'email' => 'required|string|email|max:255|unique:users,email,' . $user->id,
            'username' => 'required|string|max:255',
            'password' => 'nullable|min:8|confirmed',
            'photo' => 'nullable|image|max:2048',
        ]);

        try {
            $user->name = $request->input('name');
            $user->username = $request->input('username');
            $user->email = $request->input('email');

            if ($request->filled('password')) {
                $user->password = bcrypt($request->input('password'));
            }

            if ($request->input('delete_photo') == '1') {
                if ($user->photo) {
                    Storage::disk('private')->delete($user->photo); // Delete photo from private disk
                    $user->photo = null; // Set photo field to null
                }
            }

            if ($request->hasFile('photo')) {
                $user->photo = $request->file('photo')->store('photos', 'private');
            }

            $user->save();

            return redirect()->route('login')->with('status', 'Password reset successfully!');
        } catch (\Illuminate\Database\QueryException $e) {
            if ($e->errorInfo[0] == '23505') { // Unique violation
                $errorField = strpos($e->getMessage(), 'users_username_uk') !== false ? 'username' : 'email';
                $errorMessage = $errorField === 'username' ? 'Username already exists' : 'Email already has an account';
                return back()->withErrors([$errorField => $errorMessage])->withInput();
            }
            return back()->withErrors(['error' => 'An unexpected error occurred'])->withInput();
        }

    }


    public function deletePhoto(Request $request)
    {
        $user = Auth::user();

        if ($user->photo) {
            Storage::disk('private')->delete($user->photo);

            $user->photo = null;
            $user->save();
        }

        return response()->json(['success' => true]);
    }

    public function destroyUser(User $user)
    {
        $projectMembers = ProjectMember::where('user_id', $user->id)->get();
        for ($i = 0; $i < count($projectMembers); $i++) {
            if (!ProjectMember::removeMember($projectMembers[$i]->id)) {
                return redirect()->back()->withErrors(['error' => 'Cannot delete user in this state.']);
            }
        }
        $user->delete();

        return redirect()->route('login')->with('success', 'User deleted successfully.');
    }

    public function deleteOwnUser(User $user)
    {
        $projectMembers = ProjectMember::where('user_id', $user->id)->get();
        for ($i = 0; $i < count($projectMembers); $i++) {
            if (!ProjectMember::removeMember($projectMembers[$i]->id)) {
                return redirect()->back()->withErrors(['error' => 'Cannot delete user in this state.']);
            }
        }
        $user->delete();

        return redirect()->route('login')->with('success', 'User deleted successfully.');
    }

    public function toggleBlock(User $user)
    {
        $user->is_blocked = !$user->is_blocked;
        $user->save();

        if ($user->is_blocked) {
            $viewName = 'mail.block';
            $subject = 'Your account has been blocked';
            $data = ['user' => $user];
        } else {
            $viewName = 'mail.unblock';
            $subject = 'Your account has been unblocked';
            $data = ['user' => $user];
        }

        Mail::to($user->email)->send(new SendMail($viewName, $subject, $data));

        return redirect()->route('users.index')->with('success', 'User status updated successfully.');
    }

    public function getProjects(User $user) {
        $projects = ProjectMember::join('projects', 'project_members.project_id', '=', 'projects.id')
            ->where('project_members.user_id', $user->id)
            ->get();
        return response()->json(['projects' => $projects]);
    }
    
    /*public function checkProjectMember($userId) {
        $isMember = DB::table('project_member')->where('id_user', $userId)->exists();
        return response()->json(['isMember' => $isMember]);
    }

    public function checkProjectCoordinator($userId) {
        $isCoordinator = DB::table('project_coordinator')
            ->join('project_member', 'project_coordinator.id_member', '=', 'project_member.id')
            ->where('project_member.id_user', $userId)
            ->exists();
        return response()->json(['isCoordinator' => $isCoordinator]);
    }*/

    public static function getUser(int $userId) {
        return User::find($userId);
    }

    public function searchUser($user_input)
    {
        $user = User::where(DB::raw('LOWER(username)'), '=', $user_input)
            ->orWhere(DB::raw('LOWER(email)'), '=', $user_input)
            ->first();

        if (!$user) {
            return response()->json(['error' => 'User not found']);
        }
        return response()->json([
            'name' => $user->name,
            'username' => $user->username,
            'email' => $user->email
        ]);
    }



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
    public function show(User $users)
    {
        //
    }

    /**
     * Show the form for editing the specified resource.
     */
    public function edit(User $users)
    {
        //
    }

    /**
     * Update the specified resource in storage.
     */
    public function update(Request $request, User $users)
    {
        //
    }

    /**
     * Remove the specified resource from storage.
     */
    public function destroy(User $users)
    {
        //
    }
}
