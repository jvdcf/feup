@extends('layouts.app')

@section('content')
    <main class="vh-100 d-flex justify-content-center align-items-center">
        <section class="bg-secondary shadow-lg p-4 w-100 text-white rounded-5" style="max-width: 500px;">
            <header class="text-center mb-4">
                <h2 class="text-white">Create Administrator</h2>
            </header>

            @if ($errors->any())
                <div class="alert alert-danger text-center" role="alert">
                    Please fix the errors below and try again.
                </div>
            @endif

            <form action="{{ route('administrator.store') }}" method="POST" enctype="multipart/form-data">
                @csrf

                <!-- Username -->
                <fieldset class="mb-3">
                    <label for="username" class="form-label">Username</label>
                    <input id="username" type="text" name="username" value="{{ old('username') }}" required
                           class="form-control @error('username') is-invalid @enderror" style="background-color: #e0e0e0;">
                    @error('username')
                    <span class="invalid-feedback">{{ $message }}</span>
                    @enderror
                </fieldset>

                <!-- Name -->
                <fieldset class="mb-3">
                    <label for="name" class="form-label">Name</label>
                    <input id="name" type="text" name="name" value="{{ old('name') }}" required autofocus
                           class="form-control @error('name') is-invalid @enderror" style="background-color: #e0e0e0;">
                    @error('name')
                    <span class="invalid-feedback">{{ $message }}</span>
                    @enderror
                </fieldset>

                <!-- Email -->
                <fieldset class="mb-3">
                    <label for="email" class="form-label">E-mail Address</label>
                    <input id="email" type="email" name="email" value="{{ old('email') }}" required
                           class="form-control @error('email') is-invalid @enderror" style="background-color: #e0e0e0;">
                    @error('email')
                    <span class="invalid-feedback">{{ $message }}</span>
                    @enderror
                </fieldset>

                <!-- Password -->
                <fieldset class="mb-3">
                    <label for="password" class="form-label">Password</label>
                    <input id="password" type="password" name="password" required
                           class="form-control @error('password') is-invalid @enderror" style="background-color: #e0e0e0;">
                    @error('password')
                    <span class="invalid-feedback">{{ $message }}</span>
                    @enderror
                </fieldset>

                <!-- Confirm Password -->
                <fieldset class="mb-3">
                    <label for="password_confirmation" class="form-label">Confirm Password</label>
                    <input id="password_confirmation" type="password" name="password_confirmation" required
                           class="form-control" style="background-color: #e0e0e0;">
                </fieldset>

                <!-- Photo -->
                <fieldset class="mb-3">
                    <label for="photo" class="form-label">Photo</label>
                    <input type="file" id="photo" name="photo" class="form-control">
                    @error('photo')
                    <span class="invalid-feedback">{{ $message }}</span>
                    @enderror
                </fieldset>

                <!-- Submit -->
                <footer class="d-grid">
                    <button type="submit" class="btn text-white register-btn" style="background-color: #4b5563;">Create Administrator</button>
                </footer>
            </form>
        </section>
    </main>
@endsection
