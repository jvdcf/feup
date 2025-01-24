@extends('layouts.app')

@section('content')
    <main class="vh-100" style="max-height: 90vh">
        <div class="m-3">
            <h2 class="row">{{ $project->name }}</h2>
            <p class="row bg-light rounded-5 m-0 p-3">{{ $project->detail }}</p>
        </div>
        <div class="m-3 d-flex flex-row">
            @if(\App\Models\ProjectCoordinator::isCoordinator($project->id, Auth::user()->id) && !$project->is_archived)
                <button type="button" class="btn btn-secondary mx-2" data-bs-toggle="modal" data-bs-target="#editProjectDetailModal">
                    Edit Description
                </button>
                <div id="editProjectDetailModal" class="modal fade">
                    <div class="modal-dialog">
                        <div class="modal-content">
                            <div class="modal-header">
                                <h1 class="modal-title fs-5" id="editProjectDetailModalLabel">Leave Project</h1>
                                <button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
                            </div>
                            <form method="POST" action="">
                                @csrf
                                @method("PATCH")
                                <input type="hidden" name="project_id" value="{{ $project->id }}">
                                <div class="modal-body">
                                    <textarea name="detail" class="form-control" rows="5">{{ $project->detail }}</textarea>
                                </div>
                                <div class="modal-footer">
                                    <button type="button" class="btn btn-secondary" data-bs-dismiss="modal">Close</button>
                                    <button type="submit" class="btn btn-secondary">Save</button>
                                </div>
                            </form>
                        </div>
                    </div>
                </div>
                <form method="POST" action="{{ route('project.archive', ['project' => $project->id]) }}">
                    @csrf
                    @method('POST')
                    <button type="submit" class="btn btn-danger mx-2">Archive Project</button>
                </form>
            @endif
            <a href="{{ route('tasks.index', ['project' => $project->id]) }}" class="btn btn-secondary mx-2">View Tasks</a>
                        
            @php($member = \App\Models\ProjectMember::getProjectMember($project->id, Auth::id()))
            @if($member->is_favorite)
                <form method="POST" action="{{ route('members.update', ['member_id' => $member->id]) }}">
                    @csrf
                    @method('PATCH')
                    <input type="hidden" name="is_favorite" value="0">
                    <button type="submit" class="btn btn-secondary mx-2">Unmark as favorite</button>
                </form>
            @else
                <form method="POST" action="{{ route('members.update', ['member_id' => $member->id]) }}">
                    @csrf
                    @method('PATCH')
                    <input type="hidden" name="is_favorite" value="1">
                    <button type="submit" class="btn btn-secondary mx-2">Mark as favorite</button>
                </form>
            @endif
        </div>
        <div class="m-3">
            <h3 class="row">Coordinator</h3>
            <ul class="row my-3 list-group">
                @foreach (App\Http\Controllers\ProjectController::getCoordinators($project->id) as $coordinator)
                    <a href="{{ route('users.show', ['user' => $coordinator->id]) }}" class="my-1 link-dark link-underline-opacity-0">
                        <li class="list-group-item fs-5 rounded-pill d-flex w-100 justify-content-between">
                            {{ $coordinator->name }} ({{ $coordinator->email }})
                            @if (count(App\Http\Controllers\ProjectController::getCoordinators($project->id)) > 1 && App\Models\ProjectCoordinator::isCoordinator($project->id, Auth::id()) && !$project->is_archived)
                                <form method="POST" action="{{ route('coordinators.destroy', ['id' => \App\Models\ProjectCoordinator::getCoordinator($project->id, $coordinator->id)->id]) }}">
                                    @csrf
                                    @method('DELETE')
                                    <button type="submit" class="btn btn-danger mx-2 rounded-pill">Demote to Member</button>
                                </form>                            
                            @endif
                        </li>
                    </a>
                @endforeach
            </ul>
            <h3 class="row">Members</h3>
            @if(!$project->is_archived)
            <a href="{{ route('members.create', ['project' => $project->id]) }}" class="btn btn-secondary">Invite Member</a>
            @endif
            <ul class="row my-3 list-group">
                @foreach (App\Http\Controllers\ProjectController::getMembers($project->id) as $member)
                    <a href="{{ route('users.show', ['user' => $member->id]) }}" class="my-1 link-dark link-underline-opacity-0">
                        <li class="list-group-item fs-5 rounded-pill d-flex w-100 justify-content-between">
                            {{ $member->name }} ({{ $member->email }})
                            @if (App\Models\ProjectCoordinator::isCoordinator($project->id, Auth::id()) && !$project->is_archived)
                                <div class="justify-content-between d-flex">
                                    <form method="POST" action="{{ route('coordinators.store') }}" class="">
                                        @csrf
                                        @method('POST')
                                        <input type="hidden", name="project_member_id", value="{{ \App\Models\ProjectMember::getProjectMember($project->id, $member->id)->id }}">
                                        <button type="submit" class="btn btn-secondary mx-2 rounded-pill">Promote to Coordinator</button>
                                    </form>
                                    <form method="POST" action="{{ route('members.destroy', ['member_id' => \App\Models\ProjectMember::getProjectMember($project->id, $member->id)->id]) }}" class="">
                                        @csrf
                                        @method('DELETE')
                                        <button type="submit" class="btn btn-danger mx-2 rounded-pill">Remove from project</button>
                                    </form>
                                </div>                            
                            @endif
                        </li>
                    </a>
                @endforeach
            </ul>
        </div>
        <div class="m-3">
            <canvas class="row" id="barChart" width="400" height="400"></canvas>
        </div>
        <div class="m-3">
            <canvas class="row" id="doughnutChart" width="400" height="400"></canvas>
        </div>
        @if(!\App\Models\ProjectCoordinator::isCoordinator($project->id, Auth::user()->id) || (\App\Http\Controllers\ProjectController::getCoordinators($project->id)->count() > 1))
            <div class="m-3">
                <button type="button" class="btn btn-danger" data-bs-toggle="modal" data-bs-target="#deleteProjectModal">
                    Leave Project
                </button>
                <div id="deleteProjectModal" class="modal fade">
                    <div class="modal-dialog">
                        <div class="modal-content">
                            <div class="modal-header">
                                <h1 class="modal-title fs-5" id="deleteProjectModalLabel">Leave Project</h1>
                                <button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
                            </div>
                            <div class="modal-body">
                                <p>Are you sure you want to leave this project?</p>
                            </div>
                            <div class="modal-footer">
                                <button type="button" class="btn btn-secondary" data-bs-dismiss="modal">Close</button>
                                <form method="POST" action="{{ route('members.destroy', ['member_id' => \App\Models\ProjectMember::getProjectMember($project->id, Auth::user()->id)->id]) }}">
                                    @csrf
                                    @method("DELETE")
                                    <button type="submit" class="btn btn-danger">Leave</button>
                                </form>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        @endif
        <p hidden id="project_id">{{ $project->id }}</p>
        <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
        <script src="{{ url('js/statistics.js') }}"></script>
    </main>
@endsection
