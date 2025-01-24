<?php

namespace App\Models;

// use Illuminate\Contracts\Auth\MustVerifyEmail;
use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Foundation\Auth\User as Authenticatable;
use Illuminate\Notifications\Notifiable;
use Laravel\Sanctum\HasApiTokens;

// Added to define Eloquent relationships.
use Illuminate\Database\Eloquent\Relations\HasMany;

class User extends Authenticatable
{

    use HasApiTokens, HasFactory, Notifiable;

    // Don't add, create and update timestamps in database.
    public $timestamps = false;

    /**
     * The attributes that are mass assignable, i.e, can be set when creating or updating.
     *
     * @var array<int, string>
     */
    protected $fillable = [
        'username',
        'name',
        'email',
        'password',
        'photo',
        'is_blocked'
    ];

    /**
     * The attributes that should be hidden for serialization.
     *
     * @var array<int, string>
     */
    protected $hidden = [
        'password',
        'is_blocked'
    ];

    /**
     * The attributes that should be cast.
     *
     * @var array<string, string>
     */
    protected $casts = [
        'password' => 'hashed',
    ];

    // Get all projects associated with this user via "ProjectMember" classes
    public function projectMembers(): HasMany
    {
        return $this->hasMany(ProjectMember::class);
    }

    public function notifications() {
        return $this->hasMany(Notification::class);
    }

    public function task_notifications() {
        return $this->hasMany(TaskNotification::class);
    }


}
