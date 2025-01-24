@extends('layouts.app')

@section('content')
    <main class="vh-100 d-flex justify-content-center align-items-center" style="max-height: 90vh">
        <section class="bg-secondary shadow-lg p-4 w-100 text-white rounded-5 d-flex align-items-center gap-4" style="max-width: 700px;">

            @if (Auth::guard('admin')->check())
                @if (isset($user))
                    <div class="photo-container flex-shrink-0">
                        @if (isset($user->photo) && $user->photo)
                            <img src="{{ route('photos.show', ['filename' => basename($user->photo)]) }}" alt="User Photo" class="rounded-circle" style="width: 150px; height: 150px; object-fit: cover; background-color: #e0e0e0;">
                        @else
                            <div class="rounded-circle d-flex justify-content-center align-items-center" style="width: 150px; height: 150px; background-color: #4b5563; font-size: 4rem;">
                                {{ strtoupper(substr($user->name, 0, 1)) }}
                            </div>
                        @endif
                    </div>
                @elseif (isset($administrator) && Auth::guard('admin')->id() === $administrator->id)
                    <div class="photo-container flex-shrink-0">
                        @if (isset($administrator->photo) && $administrator->photo)
                            <img src="{{ route('photos.show', ['filename' => basename($administrator->photo)]) }}" alt="User Photo" class="rounded-circle" style="width: 150px; height: 150px; object-fit: cover; background-color: #e0e0e0;">
                        @else
                            <div class="rounded-circle d-flex justify-content-center align-items-center" style="width: 150px; height: 150px; background-color: #4b5563; font-size: 4rem;">
                                {{ strtoupper(substr($administrator->name, 0, 1)) }}
                            </div>
                        @endif
                    </div>
                @else
                    <div class="photo-container flex-shrink-0">
                        @if (isset($administrator->photo) && $administrator->photo)
                            <img src="{{ route('photos.show', ['filename' => basename($administrator->photo)]) }}" alt="User Photo" class="rounded-circle" style="width: 150px; height: 150px; object-fit: cover; background-color: #e0e0e0;">
                        @else
                            <div class="rounded-circle d-flex justify-content-center align-items-center" style="width: 150px; height: 150px; background-color: #4b5563; font-size: 4rem;">
                                {{ strtoupper(substr($administrator->name, 0, 1)) }}
                            </div>
                        @endif
                    </div>
                @endif
            @else
                <div class="photo-container flex-shrink-0">
                    @if (isset($user->photo) && $user->photo)
                        <img src="{{ route('photos.show', ['filename' => basename($user->photo)]) }}" alt="User Photo" class="rounded-circle" style="width: 150px; height: 150px; object-fit: cover; background-color: #e0e0e0;">
                    @else
                        <div class="rounded-circle d-flex justify-content-center align-items-center" style="width: 150px; height: 150px; background-color: #4b5563; font-size: 4rem;">
                            {{ strtoupper(substr($user->name, 0, 1)) }}
                        </div>
                    @endif
                </div>
            @endif

            <div class="user-info flex-grow-1">
                @if (Auth::guard('admin')->check())
                    @if (isset($user))
                        <h2 class="mb-3 text-white">User Details</h2>
                        <p><strong>Username:</strong> {{ $user->username }}</p>
                        <p><strong>Name:</strong> {{ $user->name }}</p>
                        <p><strong>Email:</strong> {{ $user->email }}</p>
                        <div class="d-flex gap-3 mt-4">
                            <a href="{{ route('users.edit', ['user' => $user->id]) }}" class="btn btn-outline-light w-50">Edit User</a>
                            <form action="{{ route('users.delete', $user) }}" method="POST" class="w-50">
                                @csrf
                                @method('DELETE')
                                <button type="submit" class="btn btn-outline-danger bg-danger text-light w-100">Delete</button>
                            </form>
                        </div>
                    @elseif (isset($administrator) && Auth::guard('admin')->id() === $administrator->id)
                        <h2 class="mb-3 text-white">Profile</h2>
                        <p><strong>Username:</strong> {{ $administrator->username }}</p>
                        <p><strong>Name:</strong> {{ $administrator->name }}</p>
                        <p><strong>Email:</strong> {{ $administrator->email }}</p>
                        <div class="d-flex gap-3 mt-4">
                            <a href="{{ route('administrator.edit', $administrator) }}" class="btn btn-outline-light w-50">Edit</a>
                            <form action="{{ route('admin.destroy.own', $administrator) }}" method="POST" class="w-50">
                                @csrf
                                @method('DELETE')
                                <button type="submit" class="btn btn-outline-danger bg-danger text-light w-100">Delete</button>
                            </form>
                        </div>
                    @else
                        <h2 class="mb-3 text-white">Administrator Details</h2>
                        <p><strong>Username:</strong> {{ $administrator->username }}</p>
                        <p><strong>Name:</strong> {{ $administrator->name }}</p>
                        <p><strong>Email:</strong> {{ $administrator->email }}</p>
                        <div class="d-flex gap-3 mt-4">
                            <a href="{{ route('administrator.edit', $administrator) }}" class="btn btn-outline-light w-50">Edit</a>
                            <form action="{{ route('administrator.destroy', $administrator) }}" method="POST" class="w-50">
                                @csrf
                                @method('DELETE')
                                <button type="submit" class="btn btn-outline-danger bg-danger text-light w-100">Delete</button>
                            </form>
                        </div>
                    @endif
                @else
                    <h2 class="mb-3 text-white">Profile</h2>
                    <p><strong>Username:</strong> {{ $user->username }}</p>
                    <p><strong>Name:</strong> {{ $user->name }}</p>
                    <p><strong>Email:</strong> {{ $user->email }}</p>
                    @if($user->id == Auth::user()->id)
                        <div class="d-flex gap-3 mt-4">
                            <a href="{{ route('users.edit', ['user' => $user->id]) }}" class="btn btn-outline-light w-50">Edit User</a>
                            <form action="{{ route('users.destroy.own', ['user' => $user->id]) }}" method="POST" class="w-50">
                                @csrf
                                @method('DELETE')
                                <button type="submit" class="btn btn-outline-danger bg-danger text-light w-100">Delete</button>
                            </form>
                        </div>
                    @endif
                @endif
            </div>

        </section>
    </main>
@endsection
