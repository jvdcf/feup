<?php
 
namespace App\Http\Controllers\Auth;

use App\Http\Controllers\Controller;
use App\Models\Administrator;
use App\Models\User;
use Illuminate\Http\Request;

use Illuminate\Http\RedirectResponse;
use Illuminate\Support\Facades\Auth;

use Illuminate\View\View;

class LoginController extends Controller
{

    /**
     * Display a login form.
     */
    //showLoginForm
    public function showLoginForm()
    {
        if (Auth::check()) {
            return redirect()->route('project.index')
                ->withSuccess('You have successfully registered & logged in!');
        } else {
            return view('auth.login');
        }
    }

    /**
     * Handle an authentication attempt.
     */
    public function authenticate(Request $request): RedirectResponse
    {
        $credentials = $request->validate([
            'email' => ['required', 'email'],
            'password' => ['required'],
        ]);

        $admin = Administrator::where('email', $credentials['email'])->first();
        $user = User::where('email', $credentials['email'])->first();


        if ($admin) {
            if (Auth::guard('admin')->attempt($credentials, $request->filled('remember'))) {
                $request->session()->regenerate();
                return redirect()->route('admin.dashboard')
                    ->with('success', 'You have successfully logged in as an admin!');
            }
        } elseif ($user && $user->is_blocked) {
            return back()->withErrors([
                'email' => 'Your account is currently blocked, contact support for more info.',
            ])->onlyInput('email');
        } else {
            if (Auth::attempt($credentials, $request->filled('remember'))) {
                $request->session()->regenerate();
                return redirect()->route('project.index')
                    ->withSuccess('You have successfully registered & logged in!');
            }
        }

        return back()->withErrors([
            'email' => 'The provided credentials do not match our records.',
        ])->onlyInput('email');
    }

    /**
     * Log out the user from application.
     */
    public function logout(Request $request)
    {
        Auth::logout();
        $request->session()->invalidate();
        $request->session()->regenerateToken();
        return redirect()->route('login')
            ->withSuccess('You have logged out successfully!');
    } 
}
