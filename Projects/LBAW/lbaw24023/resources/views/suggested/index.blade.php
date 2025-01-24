@extends('layouts.app')

@section('content')
<main class="vh-100 m-3" style="max-height: 90vh">
    <h1>Suggestions for assignee of {{ $task->title }}</h1>

    <table class="table table-striped table-hover">
        <thead class="table" style="background-color: #6c757d; color: white; font-size: 18px;">
            <tr>
                <th>Assignee</th>
                <th>Actions</th>
            </tr>
        </thead>
        <tbody id="suggestions-table">
            @foreach ($suggestions as $sug)
            <tr name="{{ $sug->name }}">
                <td style="color: #5A5A5A;">
                    <a href="{{ route('users.show', ['user' => $sug->user_id]) }}">
                        {{ $sug->name }}
                    </a>
                </td>
                <td style="color: #5A5A5A;">
                    <form method="POST" action="{{ route('suggested.destroy', ['project' => $project->id, 'task' => $task->id, 'id' => $sug->id]) }}" class="btn">
                        @csrf
                        @method('delete')
                        <button type="submit" class="btn btn-danger">Remove</button>
                    </form>
                    <form method="POST" action="{{ route('suggested.accept', ['project' => $project->id, 'task' => $task->id, 'id' => $sug->id]) }}" class="btn">
                        @csrf
                        <button type="submit" class="btn btn-success">Assign</button>
                    </form>
                </td>
            </tr>
            @endforeach
        </tbody>
    </table>
</main>
@endsection
