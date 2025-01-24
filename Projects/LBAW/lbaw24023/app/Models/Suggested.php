<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;

class Suggested extends Model
{
    use HasFactory;

    // Don't add, create and update timestamps in database.
    public $timestamps = false;

    public $table = "suggested";

    /**
     * The attributes that are mass assignable, i.e, can be set when creating or updating.
     *
     * @var array<int, string>
     */
    protected $fillable = [
        'task_id',
        'project_member_id'
    ];

    public function member() {
        return $this->belongsTo(ProjectMember::class);
    }

    public function task() {
        return $this->belongsTo(Task::class);
    }
}
