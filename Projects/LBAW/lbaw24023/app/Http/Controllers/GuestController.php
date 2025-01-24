<?php

namespace App\Http\Controllers;

use App\Models\Guest;
use Illuminate\Http\RedirectResponse;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Auth;
use Illuminate\View\View;

class GuestController extends Controller
{
    /**
     * Root URL
     */
    public function root(): RedirectResponse
    {
        if (Auth::check())
            return to_route('project.index');
        else
            return to_route('guest.features');
    }

    /**
     * Features and Main page
     */
    public function features(): View
    {
        return view('guest.features');
    }

    /**
     * About page
     */
    public function about(): View
    {
        return view('guest.about');
    }

    /**
     * Contact page
     */
    public function contacts(): View
    {
        return view('guest.contacts');
    }
}
