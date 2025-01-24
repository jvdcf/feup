<?php

namespace App\Http\Controllers;

use App\Models\User;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Mail;
use App\Models\SendMail;

class EmailController extends Controller
{
    public function emailResetPassword(Request $request)
    {
        $toEmail = $request->input('email');
        $user = User::where('email', $toEmail)->firstOrFail();
        $viewName = 'mail.reset';
        $subject = 'Reset Password';
        $data = [
            'name' => $user->name,
            'email' => $user->email,
            'resetLink' => route('password.reset.form', $user)
        ];

        Mail::to($toEmail)->send(new SendMail($viewName, $subject, $data));
        return redirect()->route('login')->with('status', 'Email sent successfully!');
    }
}