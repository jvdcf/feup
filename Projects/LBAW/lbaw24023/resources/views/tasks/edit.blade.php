@extends('layouts.app')

@section('content')
    <main class="vh-100 d-flex align-items-center" style="max-height: 90vh">
        <div class="m-3 w-100 bg-secondary text-light rounded-5">
            <form method="POST" action="{{ route('tasks.update', ['project' => $project->id, 'task' => $task->id]) }}" enctype="application/x-www-form-urlencoded" class="m-3 p-3">
                {{ csrf_field() }}
                @method('PATCH')

                <fieldset class="row input-group m-3">
                    <label for="title">Title:</label>
                    <input id="title" type="text" name="title" value="{{ $task->title }}" required autofocus class="form-control rounded">
                </fieldset>

                <fieldset class="row input-group m-3">
                    <label for="description">Description:</label>
                    <textarea name="description" id="description" cols="30" rows="10" required class="form-control rounded">{{ $task->description }}</textarea>
                </fieldset>

                <input type="hidden" name="project_id" value="{{ $project->id }}">

                <fieldset class="row input-group m-3">
                    <label for="project_member_id">Assign To:</label>
                    <select name="project_member_id" id="project_member_id" class="form-control rounded">
                        <option value="" selected>Unselected</option>
                        @foreach($members as $member)
                            <option value="{{ $member->id }}" {{ $task->project_member_id == $member->id ? 'selected' : '' }}>{{ $member->name }}</option>
                        @endforeach
                    </select>
                </fieldset>

                <fieldset class="row input-group m-3">
                    <label for="due_date">Due Date:</label>
                    <input id="due_date" type="date" name="due_date" value="{{ date('Y-m-d', strtotime($task->due_date)) }}" class="form-control rounded">
                </fieldset>

                <button type="submit" class="row m-3 btn btn-light">Update Task</button>
            </form>
        </div>
    </main>
@endsection
