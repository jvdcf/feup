@extends('layouts.app')

@section('content')
    <section class="container my-5">
        <h2 class="text-center mb-4" style="color: #6c757d;">Manage Users</h2>

        <header class="d-flex justify-content-end mb-3">
            <a href="{{ route('users.create') }}" class="btn btn-primary" style="background-color: #6c757d; border-color: #6c757d; font-size: 18px;">Create User</a>
        </header>

        <section class="row mb-4">
            <div class="col-12 col-md-6 col-lg-8 mb-2">
                <input type="text" id="search" placeholder="Search users..." class="form-control search-bar">
            </div>
            <div class="col-12 col-md-3 col-lg-2 mb-2">
                <select id="filter-blocked" class="form-control" style="color: #6c757d;">
                    <option value="default">Default</option>
                    <option value="blocked">Is Blocked</option>
                    <option value="not-blocked">Is Not Blocked</option>
                </select>
            </div>
            <div class="col-12 col-md-3 col-lg-2 mb-2">
                <select id="filter-role" class="form-control" style="color: #6c757d;">
                    <option value="default">Default</option>
                    <option value="member">Project Member</option>
                    <option value="coordinator">Project Coordinator</option>
                </select>
            </div>
        </section>
        <article class="table-responsive">
            <table class="table table-striped table-hover">
                <thead class="tablek" style="background-color: #6c757d; color: white; font-size: 18px;">
                <tr>
                    <th>Username</th>
                    <th>Name</th>
                    <th>Email</th>
                    <th>Photo</th>
                    <th style="text-align: center">Actions</th>
                </tr>
                </thead>
                <tbody id="users-table">
                @foreach ($users as $user)
                    <tr data-blocked="{{ $user->is_blocked ? 'true' : 'false' }}" data-user-id="{{ $user->id }}">
                        <td style="color: #5A5A5A;" >{{ $user->username }}</td>
                        <td style="color: #5A5A5A;" >{{ $user->name }}</td>
                        <td style="color: #5A5A5A;" >{{ $user->email }}</td>
                        <td style="color: #5A5A5A;" >
                            @if (isset($user->photo) && $user->photo)
                                <img src="{{ route('photos.show', ['filename' => basename($user->photo)]) }}" alt="User Photo" class="rounded-circle" style="width: 50px; height: 50px; object-fit: cover; background-color: #e0e0e0;">
                            @else
                                <div class="rounded-circle d-flex justify-content-center align-items-center" style="width: 50px; height: 50px; background-color: #4b5563; color: white; font-size: 1.5rem;">
                                    {{ strtoupper(substr($user->name, 0, 1)) }}
                                </div>
                            @endif
                        </td>
                        <td>
                            <section class="d-flex  gap-2">
                                <a href="{{ route('users.edit', $user) }}" class="btn btn-warning btn-md" style="background-color: #6c757d; border-color: #6c757d; color: white; font-size: 18px;">Edit</a>
                                <form action="{{ route('users.delete', $user) }}" method="POST" style="display:inline;">
                                    @csrf
                                    @method('DELETE')
                                    <button type="submit" class="btn btn-danger btn-md" style="background-color: #6c757d; border-color: #6c757d; color: white; font-size: 18px;" onclick="return confirm('Are you sure you want to delete this user?')">Delete</button>
                                </form>
                                <a href="{{ route('users.show', $user) }}" class="btn btn-info btn-md" style="background-color: #6c757d; border-color: #6c757d; color: white; font-size: 18px;">View Profile</a>
                                <form action="{{ route('users.toggleBlock', $user) }}" method="POST" style="display: inline;">
                                    @csrf
                                    @method('PATCH')
                                    <button type="submit" class="btn btn-secondary btn-md" style="color: white;">
                                        {{ $user->is_blocked ? 'Unblock' : 'Block' }}
                                    </button>
                                </form>

                            </section>
                        </td>
                    </tr>
                @endforeach
                </tbody>
            </table>
        </article>
    </section>
@endsection

@section('scripts')
    <script src="{{ url('js/app.js') }}"></script>
@endsection
