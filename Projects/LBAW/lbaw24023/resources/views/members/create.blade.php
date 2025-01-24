@extends('layouts.app')

@section('content')
    <main class="vh-100 d-flex align-items-center" style="max-height: 90vh">
        <div class="m-3 w-100 bg-secondary text-light rounded-5">
            <form method="POST" action="{{ route('members.invite') }}" class="row m-3 p-3">
                @csrf
                <input type="hidden" name="project_id" value="{{ $project->id }}">

                <fieldset class="col m-3 p-0 input-group d-flex flex-column">
                    <label for="user_input">Username or Email</label>
                    <input type="text" name="user_input" id="user_input" required autofocus class="form-control rounded w-100">
                    @error('user_input')
                        <span class="invalid-feedback">{{ $message }}</span>
                    @enderror
                </fieldset>

                <button type="submit" class="col-2 m-3 btn btn-light">Invite Member</button>
            </form>
            <div id="search_results" class="row mx-5 mt-3 mb-5 p-3 bg-light text-dark rounded-pill" style="display: none">
                <p class="col m-0">name</p>
                <p class="col m-0">username</p>
                <p class="col m-0">email</p>
            </div>
        </div>
    </main>
@endsection

@section('scripts')
    <script src="{{ url('js/app.js') }}"></script>
@endsection
