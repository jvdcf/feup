<header class="bg-secondary text-white d-flex justify-content-between align-items-center px-4 py-3">
    <h1 class="h4 m-0">
        @if(Auth::guard('admin')->check())
            <a class="text-white text-decoration-none" href="{{ route('admin.dashboard') }}">SyncTask</a>
        @elseif(Auth::check())
            <a class="text-white text-decoration-none" href="{{ route('project.index') }}">SyncTask</a>
        @else
            <a class="text-white text-decoration-none" href="{{ route('login') }}">SyncTask</a>
        @endif
    </h1>
    <div class="d-flex align-items-center">
        @if (Auth::guard('admin')->check())
            <a class="btn btn-outline-light btn-md me-3 order-1" href="{{ route('logout') }}">Logout</a>
            <a href="{{ route('administrator.show', ['administrator' => Auth::guard('admin')->id()]) }}" class="btn btn-outline-light btn-md me-3 order-2">
                <span>{{ Auth::guard('admin')->user()->name }}</span>
            </a>
        @elseif (Auth::check())
            <a href="{{ route('notifications.index') }}" class="btn btn-secondary me-3 order-1 p-0 d-flex">
                <img src="{{ asset('notification_bell.png') }}" alt="Notifications" width="36" height="36">
                @if($notification_count >0)
                    <p class="col text-light fs-5 m-0">{{ $notification_count }}</p>
                @endif
            </a>
            <a class="btn btn-outline-light btn-md me-3 order-2" href="{{ url('/logout') }}"> Logout </a>
            <a href="{{ route('users.show', ['user' => Auth::user()->id]) }}" class="btn btn-outline-light btn-md me-3 order-3">
                <span>{{ Auth::user()->name }}</span>
            </a>
        @else
            <a class="btn btn-outline-light btn-md" href="{{ route('login') }}">Login</a>
        @endif
    </div>
</header>
