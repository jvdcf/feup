@extends('layouts.app')

@section('content')
    <main class="vh-100 m-3" style="max-height: 90vh">
        <h1 class="row"> {{ $task->title  }} </h1>

        <p class="row"> {{ $task->description }} </p>

        @if ($task->project_member_id === null)
            <p> Unassigned </p>
        @elseif($task->project_member_id === Auth::user()->id)
            <p> <strong> Assigned to me </strong> </p>
        @elseif ($assignedUser !== null)
            <p> Assigned to <a href="{{ route('users.show', ['user' => $assignedUser->id]) }}"> {{ $assignedUser->name }} </a></p>
        @else
            <p>This had been assigned to a removed user.</p>
        @endif

        <p> Project: <a href="{{ route('project.show', ['project' => $task->project_id]) }}">{{ $project->name  }}</a></p>

        <p>  
            @if ($task->due_date === null)
                No due date
            @else
                Due in {{ date('Y-m-d', strtotime($task->due_date)) }} 
            @endif
        </p>

        @if ($task->is_completed)
            <p> Task completed </p>
        @else
            @if (!$project->is_archived)
                <a href="{{ route('tasks.complete', ['project' => $task->project_id, 'task' => $task->id]) }}" class="btn btn-secondary row m-1">Complete Task</a>
            @endif
        @endif
        <br>
        @if ($isCoordinator && !$project->is_archived)
        <a href="{{ route('tasks.edit', ['project' => $task->project_id, 'task' => $task->id]) }}" class="btn btn-secondary row m-1">Edit Task</a>
        <br>
        <a href="{{ route('suggested.index', ['project' => $task->project_id, 'task' => $task->id]) }}" class="btn btn-secondary row m-1">See suggestions for assignee</a>
        <form method="POST" action="{{ route('tasks.destroy', ['project' => $task->project_id, 'task' => $task->id]) }}">
            @csrf
            @method('DELETE')
            <button type="submit" class="btn btn-secondary row m-1">Delete Task</button>
        </form>
        @else
            @if (!$project->is_archived)
                <a href="{{ route('suggested.create', ['project' => $task->project_id, 'task' => $task->id]) }}" class="btn btn-secondary row m-1">Suggest a new assignee</a>
            @endif
        @endif

        <br>

        <h3 class="row"> Comments: </h3>
        <ul class="list-group">
            @foreach ($comments as $comment)
                <li class="list-group-item" style="background: lightgray;">
                    <div class="d-flex w-100 justify-content-between">
                        @if ($comment->user_id === Auth::user()->id)
                            <small> You </small>
                        @elseif ($comment->user_id === null)
                            <small>Removed User</small>
                        @else
                            <small><a href="{{ route('users.show', ['user' => $comment->user_id]) }}"> {{ $comment->name }} </a></small>
                        @endif
                            <small> {{ date('d M H\hi', strtotime($comment->post_date)) }} </small>
                    </div>
                    <div class="d-flex w-100 justify-content-between">
                        <p> {{ $comment->comment }} </p>
                        @if ($comment->user_id === Auth::user()->id && !$project->is_archived)
                            <div class="d-flex">
                                <form method="POST" action="{{ route('task_comments.destroy', ['project' => $task->project_id, 'task' => $task->id, 'id' => $comment->id]) }}">
                                    @csrf
                                    @method('DELETE')
                                    <button type="submit" class="btn btn-danger row m-1">Delete</button>
                                </form>

                                <button type="button" class="btn btn-secondary row m-1" data-bs-toggle="modal" data-bs-target="#editComment">Edit</button>
                                <div id="editComment" class="modal fade">
                                    <div class="modal-dialog">
                                        <div class="modal-content">
                                            <div class="modal-header">
                                                <h1 class="modal-title fs-5" id="editComment">Edit Comment</h1>
                                                <button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
                                            </div>
                                            <div class="modal-body">
                                                <form method="POST" action="{{ route('task_comments.update', ['project' => $task->project_id, 'task' => $task->id, 'id' => $comment->id]) }}">
                                                    @csrf
                                                    @method("PUT")
                                                    <textarea class="form-control" id="comment" name="comment" rows="3" required>{{ $comment->comment }}</textarea>
                                                    <br>
                                                    <button type="submit" class="btn btn-secondary">Apply Edit</button>
                                                </form>
                                            </div>
                                            <!-- <div class="modal-footer">
                                                <button type="button" class="btn btn-secondary" data-bs-dismiss="modal">Close</button>
                                            </div> -->
                                        </div>
                                    </div>
                                </div>
                            </div>
                        @endif
                    </div>
                </li>
            @endforeach
        </ul>

        <br>
        @if (!$project->is_archived)
        <form method="POST" action="{{ route('task_comments.store', ['project' => $task->project_id, 'task' => $task->id]) }}">
            @csrf
            <label for="comment">Add a new comment:</label>
            <textarea class="form-control" id="comment" name="comment" rows="3" required></textarea>
            <button type="submit" class="btn btn-secondary row m-1">Add Comment</button>
        </form>
        @endif
    </main>
@endsection
