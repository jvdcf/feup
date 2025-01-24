<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;
use Illuminate\Support\Facades\Auth;

class ProjectMember extends Model
{
    use HasFactory;

    // Don't add, create and update timestamps in database.
    public $timestamps = false;

    /**
     * The attributes that are mass assignable, i.e, can be set when creating or updating.
     *
     * @var array<int, string>
     */
    protected $fillable = [
        'project_id',
        'user_id',
        'is_favorite'
    ];

    public function user() {
        return $this->belongsTo(User::class);
    }

    public function assigned_tasks() {
        return $this->hasMany(Task::class);
    }

    public function suggested_tasks() {
        return $this->hasMany(Suggested::class);
    }

    public function forum_messages() {
        return $this->hasMany(ForumMessage::class);
    }

    public function task_comments() {
        return $this->hasMany(TaskComment::class);
    }

    public static function getProjectMember(int $projectId, int $userId) {
        return ProjectMember::where(function ($query) use ($projectId, $userId) {
            $query->where('project_id', $projectId)
                ->where('user_id', $userId);
        })->first();
    }

    public static function removeMember(int $projectMemberId) {
        if (Auth::check()) {
            $project_member = ProjectMember::findOrFail($projectMemberId);
            if (ProjectCoordinator::isCoordinator($project_member->project_id, Auth::id()) 
                || $project_member->user_id === Auth::id()) {

                    $coordinatorSize = ProjectCoordinator::join('project_members', 'project_coordinators.project_member_id', '=', 'project_members.id')
                        ->where('project_members.project_id', $project_member->project_id)
                        ->count();
                    
                    if ($coordinatorSize === 1 && ProjectCoordinator::isCoordinator($project_member->project_id, Auth::id())) {
                        return false;
                    }

                Notification::where("project_id", $project_member->project_id)
                    ->where("user_id", $project_member->user_id)
                    ->delete();
                
                $project_member->user_id = null;
                $project_member->save();

                return true;
                
                /* if (ProjectCoordinator::isCoordinator($project_member->project_id, Auth::id())) {
                    return redirect()->back()
                        ->with('success', 'Member removed successfully.');   
                } else {
                    return redirect()->route('project.index')->with('success', 'Left successfully.');
                } */
                
            }
            return false;
        } else {
            return false;
        }
    }
}
