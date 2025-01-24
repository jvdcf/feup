<?php

namespace App\Http\Controllers;

use App\Models\Administrator;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Auth;
use Illuminate\Support\Facades\DB;


class AdministratorController extends Controller
{
    public function profile()
    {
        $administrator = Auth::guard('admin')->user();
        return view('administrator.show-profile', compact('administrator'));
    }

    public function dashboard()
    {
        return view('administrator.dashboard');
    }

    public function listAdministrators()
    {
        $administrators = Administrator::all();
        return view('administrator.list-administrators', compact('administrators'));

    }

    public function showProfile(Administrator $administrator)
    {
        return view('administrator.show-profile', compact('administrator'));
    }
    public function createAdministrator()
    {
        return view('administrator.create-administrator');
    }

    public function storeAdministrator(Request $request)
    {
        $request->validate([
            'username' => 'required|unique:administrators,username',
            'name' => 'required',
            'email' => 'required|email|unique:administrators,email',
            'password' => 'required|min:8|confirmed',
            'photo' => 'nullable|image',
        ]);

        $administrator = new Administrator($request->except('password'));
        $administrator->password = bcrypt($request->input('password'));

        if ($request->hasFile('photo')) {
            $administrator->photo = $request->file('photo')->store('photos', 'public');
        }

        $administrator->save();

        return redirect()->route('administrator.index')->with('success', 'Administrator created successfully.');
    }

    public function editAdministratorProfile(Administrator $administrator)
    {
        return view('administrator.edit-profile', compact('administrator'));
    }


    public function updateAdministratorProfile(Request $request, Administrator $administrator)
    {
        $request->validate([
            'username' => 'required|unique:administrators,username,' . $administrator->id,
            'name' => 'required',
            'email' => 'required|email|unique:administrators,email,' . $administrator->id,
            'password' => 'nullable|min:8|confirmed',
            'photo' => 'nullable|image',
        ]);

        try {
            $administrator->name = $request->input('name');
            $administrator->username = $request->input('username');
            $administrator->email = $request->input('email');

            if ($request->filled('password')) {
                $administrator->password = bcrypt($request->input('password'));
            }

            if ($request->hasFile('photo')) {
                $administrator->photo = $request->file('photo')->store('photos', 'public');
            }

            $administrator->save();

            return redirect()->route('administrator.show', $administrator)->with('success', 'Profile updated successfully.');
        }  catch (\Illuminate\Database\QueryException $e) {
            if ($e->errorInfo[0] == '23505') { // Unique violation
                $errorField = strpos($e->getMessage(), 'users_username_uk') !== false ? 'username' : 'email';
                $errorMessage = $errorField === 'username' ? 'Username already exists' : 'Email already has an account';
                return back()->withErrors([$errorField => $errorMessage])->withInput();
            }
            return back()->withErrors(['error' => 'An unexpected error occurred'])->withInput();
        }
    }

    public function deleteProfile(Administrator $administrator)
    {
        $administrator->delete();
        return redirect()->route('administrator.index')->with('success', 'Administrator deleted successfully.');
    }

    public function deleteOwnProfile(Administrator $administrator)
    {
        $administrator->delete();
        return redirect()->route('login')->with('success', 'Administrator deleted successfully.');
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
    public function show(Administrator $administrator)
    {
        //
    }

    /**
     * Show the form for editing the specified resource.
     */
    public function edit(Administrator $administrator)
    {
        //
    }

    /**
     * Update the specified resource in storage.
     */
    public function update(Request $request, Administrator $administrator)
    {
        //
    }

    /**
     * Remove the specified resource from storage.
     */
    public function destroy(Administrator $administrator)
    {
        //
    }



}
