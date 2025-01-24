@extends('layouts.app')

@section('content')
    <main class="vh-100 d-flex justify-content-center align-items-center">
        <section class="bg-secondary shadow-lg p-4 w-100 text-white rounded-5" style="max-width: 500px;">
            <header class="text-center mb-4">
                <h2 class="text-white">Login</h2>
            </header>

            @if (session('success'))
                <p class="alert alert-success text-center" role="alert">
                    {{ session('success') }}
                </p>
            @endif

            @if (session('status'))
                <div class="alert alert-success">
                    {{ session('status') }}
                </div>
            @endif

            <form method="POST" action="{{ route('login') }}">
                @csrf

                <fieldset class="mb-3">
                    <label for="email" class="form-label">E-mail</label>
                    <input id="email" type="email" name="email" value="{{ old('email') }}" required autofocus
                           class="form-control @error('email') is-invalid @enderror" style="background-color: #e0e0e0;">
                    @error('email')
                    <span class="invalid-feedback">{{ $message }}</span>
                    @enderror
                </fieldset>

                <fieldset class="mb-3">
                    <label for="password" class="form-label">Password</label>
                    <input id="password" type="password" name="password" required
                           class="form-control @error('password') is-invalid @enderror" style="background-color: #e0e0e0;">
                    @error('password')
                    <span class="invalid-feedback">{{ $message }}</span>
                    @enderror
                </fieldset>



                <footer class="d-grid">
                    <button type="submit" class="btn text-white login-btn" style="background-color: #4b5563;">Login</button>
                </footer>

                <footer class="text-center mt-3">
                    <a href="{{ route('register') }}" class="btn btn-link text-white register-link">Register</a>
                    <a href="#" class="btn btn-link text-white" id="reset-password-link">Reset Password</a>
                </footer>
            </form>
        </section>
    </main>

    <div id="reset-password-popup" class="d-none position-fixed top-0 start-0 w-100 h-100 d-flex justify-content-center align-items-center" style="background-color: rgba(0, 0, 0, 0.5);">
        <div class="bg-secondary shadow-lg p-4 w-100 text-white rounded-5" style="max-width: 400px;">
            <header class="text-center mb-4">
                <h2 class="text-white">Reset Password</h2>
            </header>
            <form method="POST" action="{{ route('password.email') }}">
                @csrf
                <fieldset class="mb-3">
                    <label for="reset-email" class="form-label">E-mail</label>
                    <div class="input-group">
                        <input id="reset-email" type="email" name="email" required class="form-control" style="background-color: #e0e0e0;">
                        <button type="submit" class="btn text-white" style="background-color: #4b5563;">Send</button>
                    </div>
                </fieldset>
            </form>
        </div>
    </div>


@endsection
