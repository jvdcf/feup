@extends('layouts.app')

@section('content')
    <main class="vh-100 d-flex justify-content-center align-items-center">
        <section class="w-100 text-white p-4" style="max-width: 700px;">
            <h3 class="text-center mb-5" style="color: #6c757d">Welcome to the Admin Dashboard!</h3>
            <ul class="list-unstyled d-flex flex-wrap justify-content-center gap-4">
                <li>
                    <a href="{{ route('administrator.index') }}"
                       class="btn w-100 p-4 rounded-5"
                       style="background-color: #6c757d; color: #fff; min-width: 300px; text-align: center; font-size: 1.25rem;">
                        Manage Administrators
                    </a>
                </li>
                <li>
                    <a href="{{ route('users.index') }}"
                       class="btn w-100 p-4 rounded-5"
                       style="background-color: #6c757d; color: #fff; min-width: 300px; text-align: center; font-size: 1.25rem;">
                        Manage Users
                    </a>
                </li>
                <li>
                    <a href="{{ route('administrator.list-projects') }}"
                       class="btn w-100 p-4 rounded-5"
                       style="background-color: #6c757d; color: #fff; min-width: 300px; text-align: center; font-size: 1.25rem;">
                        Monitor Projects
                    </a>
                </li>
                <li>
                    <a href="{{ route('administrator.show', ['administrator' => Auth::guard('admin')->user()->id]) }}"
                       class="btn w-100 p-4 rounded-5"
                       style="background-color: #6c757d; color: #fff; min-width: 300px; text-align: center; font-size: 1.25rem;">
                        View Profile
                    </a>
                </li>
            </ul>
        </section>
    </main>
@endsection
