@extends('layouts.app')

@section('content')
    <main class="vh-100 d-flex align-items-center" style="max-height: 90vh">
        <div class="m-3 w-100 bg-secondary text-light rounded-5">
            <form method="POST" action="{{ route('project.store') }}" class="m-3 p-3">
                @csrf
                <fieldset class="row input-group m-3">
                    <label for="project_name">Project Name</label>
                    <input id="project_name" type="text" name="name" required autofocus class="form-control rounded">
                </fieldset>

                <fieldset class="row input-group m-3">
                    <label for="project_detail">Project Details</label>
                    <input id="project_detail" type="text" name="detail" class="form-control rounded">
                </fieldset>

                <button type="submit" class="row m-3 btn btn-light">Create Project</button>
            </form>
        </div>
    </main>
@endsection
