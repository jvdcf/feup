@extends('layouts.app')

@section('content')
<h1 class="row m-3">Suggest a new user to the task "{{ $task->title }}" </h1>
<main class="vh-100 d-flex align-items-center" style="max-height: 90vh">
    <div class="m-3 w-100 bg-secondary text-light rounded-5">
        <form method="POST" action="{{ route('suggested.store', ['project' => $project->id, 'task' => $task->id]) }}" enctype="application/x-www-form-urlencoded" class="m-3 p-3">
            @csrf

            <input type="hidden" name="project_id" value="{{ $project->id }}">
            <input type="hidden" name="task_id" value="{{ $task->id }}">

            <fieldset class="row input-group m-3">
                <label for="project_member_id">Assignee Suggested:*</label>
                <select name="project_member_id" id="project_member_id" class="form-control rounded">
                    @foreach($members as $member)
                    <option value="{{ $member->id }}">{{ $member->name }}</option>
                    @endforeach
                </select>
            </fieldset>

            <button type="submit" class="row m-3 btn btn-light">Submit Suggestion</button>
        </form>
    </div>
</main>
@endsection