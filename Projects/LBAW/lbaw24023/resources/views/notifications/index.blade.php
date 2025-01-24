@extends('layouts.app')

@section('content')
    <main class="vh-100" style="max-height: 90vh">
        <div class="row m-3 d-flex">
            @foreach($notifications as $notification)
                <div class="my-2 p-4 bg-secondary rounded-5">
                    @if($notification->notif === 'coordinator_change')
                        <h2 class="row m-2 fs-4 text-light">Coordinators of {{ $notification->name }} were changed.</h2>
                        <p class="row m-2 fs-5 text-light d-flex justify-content-end">{{ (new DateTime($notification->sent_date))->format('H:i d-m-Y') }}</p>
                        <div class="row m-2">
                            <a href="{{ route('project.show', ['project' => $notification->project_id]) }}" class="col mx-3 w-100 p-0 link-liht text-light link-underline-opacity-0">
                                <div class="btn btn-light w-100">Go to Project</div>
                            </a>
                            <form action="{{ route('notifications.destroy', ['id' => $notification->notification_id]) }}" method="POST" class="col p-0 mx-3">
                                @csrf
                                @method('DELETE')
                                <button type="submit" class="w-100 btn btn-danger">Delete</button>
                            </form>
                        </div>
                    @elseif($notification->notif === 'invitation_accepted')
                        <h2 class="row m-2 fs-4 text-light">Someone accepted invitation to {{ $notification->name }}!</h2>
                        <p class="row m-2 fs-5 text-light d-flex justify-content-end">{{ (new DateTime($notification->sent_date))->format('H:i d-m-Y') }}</p>
                        <div class="row m-2">
                            <a href="{{ route('project.show', ['project' => $notification->project_id]) }}" class="col mx-3 w-100 p-0 link-light text-light link-underline-opacity-0">
                                <div class="btn btn-light w-100">Go to Project</div>
                            </a>
                            <form action="{{ route('notifications.destroy', ['id' => $notification->notification_id]) }}" method="POST" class="col p-0 mx-3">
                                @csrf
                                @method('DELETE')
                                <button type="submit" class="w-100 btn btn-danger">Delete</button>
                            </form>
                        </div>
                    @elseif($notification->notif === 'project_invitation')
                        <h2 class="row m-2 fs-4 text-light">You were invited to {{ $notification->name }}!</h2>
                        <p class="row m-2 fs-5 text-light d-flex justify-content-end">{{ (new DateTime($notification->sent_date))->format('H:i d-m-Y') }}</p>
                        <div class="row m-2">
                            <form action="{{ route('members.store') }}" method="POST" class="col p-0 mx-3">
                                @csrf
                                <input type="hidden" name="project_id" value="{{ $notification->project_id }}">
                                <input type="hidden" name="user_input" value="{{ App\Http\Controllers\UserController::getUser($notification->user_id)->username }}">
                                <button type="submit" class="w-100 btn btn-light">Accept invitation</button>
                            </form>
                            <form action="{{ route('notifications.destroy', ['id' => $notification->notification_id]) }}" method="POST" class="col p-0 mx-3">
                                @csrf
                                @method('DELETE')
                                <button type="submit" class="w-100 btn btn-danger">Delete</button>
                            </form>
                        </div>
                    @elseif($notification->notif === 'task_assign')
                        <h2 class="row m-2 fs-4 text-light">You were assigned to {{ $notification->title }} of {{ $notification->name }}.</h2>
                        <p class="row m-2 fs-5 text-light d-flex justify-content-end">{{ (new DateTime($notification->sent_date))->format('H:i d-m-Y') }}</p>
                        <div class="row m-2">
                            <a href="{{ route('tasks.show', ['project' => $notification->project_id, 'task' => $notification->task_id]) }}" class="col mx-3 w-100 p-0 link-liht text-light link-underline-opacity-0">
                                <div class="btn btn-light w-100">Go to Task</div>
                            </a>
                            <form action="{{ route('task_notifications.destroy', ['id' => $notification->notification_id]) }}" method="POST" class="col p-0 mx-3">
                                @csrf
                                @method('DELETE')
                                <button type="submit" class="w-100 btn btn-danger">Delete</button>
                            </form>
                        </div>
                    @else
                        <h2 class="row m-2 fs-4 text-light">The task {{ $notification->title }} of project {{ $notification->name }} was completed!</h2>
                        <p class="row m-2 fs-5 text-light d-flex justify-content-end">{{ (new DateTime($notification->sent_date))->format('H:i d-m-Y') }}</p>
                        <div class="row m-2">
                            <a href="{{ route('tasks.show', ['project' => $notification->project_id, 'task' => $notification->task_id]) }}" class="col mx-3 w-100 p-0 link-liht text-light link-underline-opacity-0">
                                <div class="btn btn-light w-100">Go to Task</div>
                            </a>
                            <form action="{{ route('task_notifications.destroy', ['id' => $notification->notification_id]) }}" method="POST" class="col p-0 mx-3">
                                @csrf
                                @method('DELETE')
                                <button type="submit" class="w-100 btn btn-danger">Delete</button>
                            </form>
                        </div>
                    @endif
                </div>
            @endforeach
        </div>
    </main>
@endsection