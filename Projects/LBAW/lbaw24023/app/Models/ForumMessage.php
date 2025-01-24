<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;

class ForumMessage extends Model
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
        'project_member_id',
        'send_date',
        'message'
    ];

    public function project() {
        return $this->belongsTo(Project::class);
    }

    public function sender() {
        return $this->belongsTo(ProjectMember::class);
    }
}
