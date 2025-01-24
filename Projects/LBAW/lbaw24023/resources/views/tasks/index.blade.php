@extends('layouts.app')

@section('content')
    <main class="vh-100 m-3" style="max-height: 90vh">
        <h1>Tasks</h1>

        @if($can_create_task && !$curr_proj->is_archived)
            <header class="d-flex mb-3">
                <a href="{{ route('tasks.create', $curr_proj->id) }}" class="btn btn-secondary">Create Task</a>
            </header>
        @endif

        <section class="row mb-4">
            <div class="col-12 col-md-6 col-lg-8 mb-2">
                <label for="fts">Search:</label>
                <input name="fts" type="text" id="search-task" placeholder="Search tasks..." class="search-task">
            </div>

            <div class="col-12 col-md-6 col-lg-8 mb-2">
                <label for="completed">Completion:</label>
                <select name="completed" id="filter-is-completed">
                    <option value="all">All</option>
                    <option value="completed">Is Completed</option>
                    <option value="not-completed">Is Not Completed</option>
                </select>
            </div>

            <div class="col-12 col-md-6 col-lg-8 mb-2">
                <label for="assigned">Assigned:</label>
                <select name="assigned" id="filter-assigned-to">
                    <option value="any">Any</option>
                    @foreach ($users as $user)
                        <option value="{{ $user->id }}">{{ $user->name }}</option>
                    @endforeach
                </select>
            </div>

            <div class="col-12 col-md-6 col-lg-8 mb-2">
                <label for="due_date">Due Date:</label>
                <input name="due_date" type="date" id="date" placeholder="Set due date" class="date-field">
            </div>
        </section>

        <table class="table table-striped table-hover">
            <thead class="tablek" style="background-color: #6c757d; color: white; font-size: 18px;">
            <tr>
                <th>Title</th>
                <th>Assignee</th>
                <th>Completion Status</th>
                <th>Due Date</th>
            </tr>
            </thead>
            <tbody id="tasks-table">
            @foreach ($tasks as $task)
                <tr name="{{ $task->id }}">
                    <td style="color: #5A5A5A;">
                        <a href="{{ route("tasks.show", ["project" => $curr_proj->id, "task" => $task->id]) }}">
                        {{ $task->title }}
                        </a>
                    </td>
                    <td style="color: #5A5A5A;">
                        @if ($task->name !== null)
                        {{ $task->name }}
                        @else
                        Unassigned
                        @endif
                    </td>
                    <td style="color: #5A5A5A;">{{ $task->is_completed ? 'Completed' : 'Not completed' }}</td>
                    <td style="color: #5A5A5A;">
                        @if($task->due_date == null)
                            No due date
                        @else
                            {{ date('Y-m-d', strtotime($task->due_date)) }}
                        @endif
                    </td>
                </tr>
            @endforeach
            </tbody>
        </table>
    </main>
@endsection

@section('scripts')
    <script src="{{ url('js/app.js') }}"></script>
@endsection
