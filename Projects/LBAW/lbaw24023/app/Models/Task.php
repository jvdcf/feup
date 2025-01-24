<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;

class Task extends Model
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
        'title',
        'description',
        'project_member_id',
        'is_completed',
        'due_date'
    ];

    public function project() {
        return $this->belongsTo(Project::class);
    }

    public function assigned_user() {
        return $this->belongsTo(User::class);
    }

    public function suggested_users() {
        return $this->hasMany(Suggested::class);
    }

    public function comments() {
        return $this->hasMany(TaskComment::class);
    }

    public function notifications() {
        return $this->hasMany(Notification::class);
    }

    public function task_notifications() {
        return $this->hasMany(TaskNotification::class);
    }
}
