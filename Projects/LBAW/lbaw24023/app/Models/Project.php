<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;

class Project extends Model
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
        'name',
        'detail',
        'is_archived'
    ];

    public function notifications() {
        return $this->hasMany(Notification::class);
    }

    public function task_notifications() {
        return $this->hasMany(TaskNotification::class);
    }
    
    public function tasks() {
        return $this->hasMany(Task::class);
    }

    public function forum_messages() {
        return $this->hasMany(ForumMessage::class);
    }
}
