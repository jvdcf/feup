@extends('layouts.app')

@section('content')
    <main class="vh-100" style="max-height: 90vh">
        <div class="row m-3 fs-4 btn btn-secondary rounded-pill d-flex justify-content-between">
            <a href="{{ route('project.create') }}" class="p-0 link-light text-light link-underline-opacity-0">Create Project</a>
        </div>
        <div class="row m-3 d-flex">
            @foreach($projects as $project)
                @php($member = \App\Models\ProjectMember::getProjectMember($project->id, Auth::user()->id))
                @if($member->is_favorite)
                    <div class="my-2 p-4 bg-primary rounded-5">
                        <a href="{{ route('project.show', ['project' => $project->id]) }}" class="p-0 link-light text-light link-underline-opacity-0">
                            <h2 class="fs-4">{{ $project->name }}</h2>
                            <p>{{ $project->detail }}</p>
                        </a>
                    </div>
                @else
                    <div class="my-2 p-4 bg-secondary rounded-5">
                        <a href="{{ route('project.show', ['project' => $project->id]) }}" class="p-0 link-light text-light link-underline-opacity-0">
                            <h2 class="fs-4">{{ $project->name }}</h2>
                            <p>{{ $project->detail }}</p>
                        </a>
                    </div>
                @endif
            @endforeach
        </div>

    </main>
@endsection
