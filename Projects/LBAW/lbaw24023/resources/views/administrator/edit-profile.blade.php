@extends('layouts.app')

@section('content')
    @if (Auth::guard('admin')->check())
        @if (isset($user))
            <main class="vh-100 d-flex justify-content-center align-items-center">
                <section class="bg-secondary shadow-lg p-4 w-100 text-white rounded-5" style="max-width: 500px;">
                    <header class="text-center mb-4">
                        <h2 class="text-white">Edit User</h2>
                    </header>

                    <form action="{{ route('users.update', ['user' => $user->id]) }}" method="POST" enctype="multipart/form-data">
                        @csrf
                        @method('PUT')

                        <fieldset class="mb-3">
                            <label for="name" class="form-label">Name</label>
                            <input id="name" type="text" name="name" value="{{ old('name', $user->name) }}" required
                                   class="form-control" style="background-color: #e0e0e0;">
                        </fieldset>

                        <fieldset class="mb-3">
                            <label for="username" class="form-label">Username</label>
                            <input id="username" type="text" name="username" value="{{ old('username', $user->username) }}" required
                                   class="form-control @error('username') is-invalid @enderror" style="background-color: #e0e0e0;">
                            @error('username')
                            <span class="invalid-feedback">Username already exists</span>
                            @enderror
                        </fieldset>

                        <fieldset class="mb-3">
                            <label for="email" class="form-label">Email</label>
                            <input id="email" type="email" name="email" value="{{ old('email', $user->email) }}" required
                                   class="form-control @error('email') is-invalid @enderror" style="background-color: #e0e0e0;">
                            @error('email')
                            <span class="invalid-feedback">Email already has an account</span>
                            @enderror
                        </fieldset>

                        <fieldset class="mb-3">
                            <label for="photo" class="form-label">Photo</label>
                            @if ($user->photo)
                                <div class="mb-3">
                                    <span>Current Photo:</span>
                                    <img src="{{ asset('storage/' . $user->photo) }}" alt="User Photo" class="img-thumbnail" style="max-width: 150px;" id="current-photo">
                                    <button type="button" class="btn btn-danger mt-2" onclick="deletePhoto()">Delete Photo</button>
                                </div>
                            @else
                                <div class="mb-3" id="no-current-photo">
                                    <span>No current photo</span>
                                </div>
                            @endif
                            <div class="mb-3">
                                <span>New Photo:</span>
                                <img id="photo-preview" class="img-thumbnail" style="max-width: 150px; display: none;">
                            </div>
                            <input id="photo" type="file" name="photo" class="form-control" onchange="previewImage(event)">
                            <input type="hidden" name="delete_photo" id="delete_photo" value="0">
                        </fieldset>

                        <footer class="d-grid">
                            <button type="submit" class="btn text-white" style="background-color: #4b5563;">Update User</button>
                        </footer>
                    </form>
                </section>
            </main>
        @elseif (Auth::guard('admin')->id() === $administrator->id)
            <main class="vh-100 d-flex justify-content-center align-items-center">
                <section class="bg-secondary shadow-lg p-4 w-100 text-white rounded-5" style="max-width: 500px;">
                    <header class="text-center mb-4">
                        <h2 class="text-white">Edit Profile</h2>
                    </header>

                    <form action="{{ route('administrator.update', ['administrator' => $administrator->id]) }}" method="POST" enctype="multipart/form-data">
                        @csrf
                        @method('PUT')

                        <fieldset class="mb-3">
                            <label for="username" class="form-label">Username</label>
                            <input id="username" type="text" name="username" value="{{ old('username', $administrator->username) }}" required
                                   class="form-control @error('username') is-invalid @enderror" style="background-color: #e0e0e0;">
                            @error('username')
                            <span class="invalid-feedback">Username already exists</span>
                            @enderror
                        </fieldset>

                        <fieldset class="mb-3">
                            <label for="name" class="form-label">Name</label>
                            <input id="name" type="text" name="name" value="{{ old('name', $administrator->name) }}" required
                                   class="form-control" style="background-color: #e0e0e0;">
                        </fieldset>

                        <fieldset class="mb-3">
                            <label for="email" class="form-label">Email</label>
                            <input id="email" type="email" name="email" value="{{ old('email', $administrator->email) }}" required
                                   class="form-control @error('email') is-invalid @enderror" style="background-color: #e0e0e0;">
                            @error('email')
                            <span class="invalid-feedback">Email already has an account</span>
                            @enderror
                        </fieldset>

                        <fieldset class="mb-3">
                            <label for="password" class="form-label">Password</label>
                            <input id="password" type="password" name="password"
                                   class="form-control @error('password') is-invalid @enderror" style="background-color: #e0e0e0;">
                            @error('password')
                            <span class="invalid-feedback">{{ $message }}</span>
                            @enderror
                        </fieldset>

                        <fieldset class="mb-3">
                            <label for="password_confirmation" class="form-label">Confirm Password</label>
                            <input id="password_confirmation" type="password" name="password_confirmation"
                                   class="form-control" style="background-color: #e0e0e0;">
                            @error('password_confirmation')
                            <span class="invalid-feedback">Password does not match</span>
                            @enderror
                        </fieldset>

                        <fieldset class="mb-3">
                            <label for="photo" class="form-label">Photo</label>
                            @if ($administrator->photo)
                                <div class="mb-3">
                                    <span>Current Photo:</span>
                                    <img src="{{ asset('storage/' . $administrator->photo) }}" alt="Administrator Photo" class="img-thumbnail" style="max-width: 150px;" id="current-photo">
                                    <button type="button" class="btn btn-danger mt-2" onclick="deletePhoto()">Delete Photo</button>
                                </div>
                            @else
                                <div class="mb-3" id="no-current-photo">
                                    <span>No current photo</span>
                                </div>
                            @endif
                            <div class="mb-3">
                                <span>New Photo:</span>
                                <img id="photo-preview" class="img-thumbnail" style="max-width: 150px; display: none;">
                            </div>
                            <input id="photo" type="file" name="photo" class="form-control" onchange="previewImage(event)">
                            <input type="hidden" name="delete_photo" id="delete_photo" value="0">
                        </fieldset>

                        <footer class="d-grid">
                            <button type="submit" class="btn text-white" style="background-color: #4b5563;">Update Profile</button>
                        </footer>
                    </form>
                </section>
            </main>
        @else
            <main class="vh-100 d-flex justify-content-center align-items-center">
                <section class="bg-secondary shadow-lg p-4 w-100 text-white rounded-5" style="max-width: 500px;">
                    <header class="text-center mb-4">
                        <h2 class="text-white">Edit Administrator</h2>
                    </header>

                    <form action="{{ route('administrator.update', ['administrator' => $administrator->id]) }}" method="POST" enctype="multipart/form-data">
                        @csrf
                        @method('PUT')

                        <fieldset class="mb-3">
                            <label for="username" class="form-label">Username</label>
                            <input id="username" type="text" name="username" value="{{ old('username', $administrator->username) }}" required
                                   class="form-control @error('username') is-invalid @enderror" style="background-color: #e0e0e0;">
                            @error('username')
                            <span class="invalid-feedback">Username already exists</span>
                            @enderror
                        </fieldset>

                        <fieldset class="mb-3">
                            <label for="name" class="form-label">Name</label>
                            <input id="name" type="text" name="name" value="{{ old('name', $administrator->name) }}" required
                                   class="form-control" style="background-color: #e0e0e0;">
                        </fieldset>

                        <fieldset class="mb-3">
                            <label for="email" class="form-label">Email</label>
                            <input id="email" type="email" name="email" value="{{ old('email', $administrator->email) }}" required
                                   class="form-control @error('email') is-invalid @enderror" style="background-color: #e0e0e0;">
                            @error('email')
                            <span class="invalid-feedback">Email already has an account</span>
                            @enderror
                        </fieldset>

                        <fieldset class="mb-3">
                            <label for="photo" class="form-label">Photo</label>
                            @if ($administrator->photo)
                                <div class="mb-3">
                                    <span>Current Photo:</span>
                                    <img src="{{ asset('storage/' . $administrator->photo) }}" alt="Administrator Photo" class="img-thumbnail" style="max-width: 150px;" id="current-photo">
                                    <button type="button" class="btn btn-danger mt-2" onclick="deletePhoto()">Delete Photo</button>
                                </div>
                            @else
                                <div class="mb-3" id="no-current-photo">
                                    <span>No current photo</span>
                                </div>
                            @endif
                            <div class="mb-3">
                                <span>New Photo:</span>
                                <img id="photo-preview" class="img-thumbnail" style="max-width: 150px; display: none;">
                            </div>
                            <input id="photo" type="file" name="photo" class="form-control" onchange="previewImage(event)">
                            <input type="hidden" name="delete_photo" id="delete_photo" value="0">
                        </fieldset>

                        <footer class="d-grid">
                            <button type="submit" class="btn text-white" style="background-color: #4b5563;">Update Administrator</button>
                        </footer>
                    </form>
                </section>
            </main>
        @endif
    @else
        <main class="vh-100 d-flex justify-content-center align-items-center">
            <section class="bg-secondary shadow-lg p-4 w-100 text-white rounded-5" style="max-width: 500px;">
                <header class="text-center mb-4">
                    <h2 class="text-white">Edit Profile</h2>
                </header>

                <form action="{{ route('users.update', ['user' => $user->id]) }}" method="POST" enctype="multipart/form-data">
                    @csrf
                    @method('PUT')

                    <fieldset class="mb-3">
                        <label for="name" class="form-label">Name</label>
                        <input id="name" type="text" name="name" value="{{ old('name', $user->name) }}" required
                               class="form-control" style="background-color: #e0e0e0;">
                    </fieldset>

                    <fieldset class="mb-3">
                        <label for="username" class="form-label">Username</label>
                        <input id="username" type="text" name="username" value="{{ old('username', $user->username) }}" required
                               class="form-control @error('username') is-invalid @enderror" style="background-color: #e0e0e0;">
                        @error('username')
                        <span class="invalid-feedback">Username already exists</span>
                        @enderror
                    </fieldset>

                    <fieldset class="mb-3">
                        <label for="email" class="form-label">Email</label>
                        <input id="email" type="email" name="email" value="{{ old('email', $user->email) }}" required
                               class="form-control @error('email') is-invalid @enderror" style="background-color: #e0e0e0;">
                        @error('email')
                        <span class="invalid-feedback">Email already has an account</span>
                        @enderror
                    </fieldset>

                    <fieldset class="mb-3">
                        <label for="password" class="form-label">Password</label>
                        <input id="password" type="password" name="password"
                               class="form-control @error('password') is-invalid @enderror" style="background-color: #e0e0e0;">
                        @error('password')
                        <span class="invalid-feedback">{{ $message }}</span>
                        @enderror
                    </fieldset>

                    <fieldset class="mb-3">
                        <label for="password_confirmation" class="form-label">Confirm Password</label>
                        <input id="password_confirmation" type="password" name="password_confirmation"
                               class="form-control" style="background-color: #e0e0e0;">
                        @error('password_confirmation')
                        <span class="invalid-feedback">Password does not match</span>
                        @enderror
                    </fieldset>

                    <fieldset class="mb-3">
                        <label for="photo" class="form-label">Photo</label>
                        @if ($user->photo)
                            <div class="mb-3">
                                <span>Current Photo:</span>
                                <img src="{{ asset('storage/' . $user->photo) }}" alt="User Photo" class="img-thumbnail" style="max-width: 150px;" id="current-photo">
                                <button type="button" class="btn btn-danger mt-2" onclick="deletePhoto()">Delete Photo</button>
                            </div>
                        @else
                            <div class="mb-3" id="no-current-photo">
                                <span>No current photo</span>
                            </div>
                        @endif
                        <div class="mb-3">
                            <span>New Photo:</span>
                            <img id="photo-preview" class="img-thumbnail" style="max-width: 150px; display: none;">
                        </div>
                        <input id="photo" type="file" name="photo" class="form-control" onchange="previewImage(event)">
                        <input type="hidden" name="delete_photo" id="delete_photo" value="0">
                    </fieldset>

                    <footer class="d-grid">
                        <button type="submit" class="btn text-white" style="background-color: #4b5563;">Update Profile</button>
                    </footer>
                </form>
            </section>
        </main>
    @endif
@endsection
