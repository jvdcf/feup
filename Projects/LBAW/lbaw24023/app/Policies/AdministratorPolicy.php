<?php

namespace App\Policies;

use App\Models\Administrator;
use App\Models\User;
use Illuminate\Auth\Access\Response;
use Illuminate\Foundation\Auth\User as Authenticable;


class AdministratorPolicy
{

    public function isSelf(Administrator $admin): Response {
        return Auth::id() == $admin->id 
            ? Response::allow()
            : Response::denyWithStatus(403);
    }
    
    // /**
    //  * Determine whether the user can view any models.
    //  */
    // public function viewAny(Authenticable $user): bool
    // {
    //     //
    // }

    // /**
    //  * Determine whether the user can view the model.
    //  */
    // public function view(Authenticable $user, Administrator $administrator): bool
    // {
    //     //
    // }

    // /**
    //  * Determine whether the user can create models.
    //  */
    // public function create(Authenticable $user): bool
    // {
    //     //
    // }

    // /**
    //  * Determine whether the user can update the model.
    //  */
    // public function update(Authenticable $user, Administrator $administrator): bool
    // {
    //     //
    // }

    // /**
    //  * Determine whether the user can delete the model.
    //  */
    // public function delete(Authenticable $user, Administrator $administrator): bool
    // {
    //     //
    // }

    // /**
    //  * Determine whether the user can restore the model.
    //  */
    // public function restore(Authenticable $user, Administrator $administrator): bool
    // {
    //     //
    // }

    // /**
    //  * Determine whether the user can permanently delete the model.
    //  */
    // public function forceDelete(Authenticable $user, Administrator $administrator): bool
    // {
    //     //
    // }
}
