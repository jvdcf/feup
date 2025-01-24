<?php

namespace App\Http\Controllers;

use App\Models\ProjectCoordinator;
use Illuminate\Http\Request;

class ProjectCoordinatorController extends Controller
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
    public function create()
    {
        //
    }

    /**
     * Store a newly created resource in storage.
     */
    public function store(Request $request)
    {
        $validated = $request->validate([
            'project_member_id' => 'required|exists:project_members,id'        
        ]);
        $proj_coord = new ProjectCoordinator;
        $proj_coord->project_member_id = $request->project_member_id;
        $proj_coord->save();
        return redirect()->back();
    }

    /**
     * Display the specified resource.
     */
    public function show(ProjectCoordinator $projectCoordinator)
    {
        //
    }

    /**
     * Show the form for editing the specified resource.
     */
    public function edit(ProjectCoordinator $projectCoordinator)
    {
        //
    }

    /**
     * Update the specified resource in storage.
     */
    public function update(Request $request, ProjectCoordinator $projectCoordinator)
    {
        //
    }

    /**
     * Remove the specified resource from storage.
     */
    public function destroy(int $projectCoordinatorId)
    {
        // Middleware this
        $projectCoordinator = ProjectCoordinator::findOrFail($projectCoordinatorId);
        $projectCoordinator->delete();
        return redirect()->back();
    }
}
