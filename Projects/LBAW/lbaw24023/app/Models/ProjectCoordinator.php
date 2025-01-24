<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;
use Illuminate\Support\Facades\DB;

class ProjectCoordinator extends Model
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
        'project_member_id',
    ];

    public function member() {
        return $this->belongsTo(ProjectMember::class);
    }

    public static function isCoordinator(int $project_id, int $user_id) : bool {
        $coordinator = DB::table('project_coordinators')
            ->join('project_members', 'project_coordinators.project_member_id', '=', 'project_members.id')
            ->where('project_members.project_id', $project_id)
            ->where('project_members.user_id', $user_id)
            ->first();

        return $coordinator !== null;
    }

    public static function getCoordinator(int $project_id, int $user_id) : ProjectCoordinator {
        $project_member = ProjectMember::where('project_id', $project_id)->where('user_id', $user_id)->first();
        $coordinator = ProjectCoordinator::where('project_member_id', $project_member->id)->first();
        return $coordinator;
    }
}
