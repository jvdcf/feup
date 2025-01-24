@if(Auth::check() && !Auth::guard('admin')->check())
    <nav class="sidebar col-1 col-md-2 col-sm-3 order-1 p-4 d-flex justify-content-center" style="width:150px">
        <ul class="bg-secondary p-2 rounded-pill d-flex flex-column align-items-center" style="width:92px">
            @foreach($projects as $project)
                @if($project->is_favorite)
                    <a href="{{ route("project.show", ["project" => $project->id]) }}" class="link-light link-underline-opacity-0">
                        <li class="btn btn-primary rounded-circle m-2 fs-2 text-light" style="width: 60px; height: 60px">
                            {{ substr($project->name, 0, 1) }}
                        </li>
                    </a>
                @else
                    <a href="{{ route("project.show", ["project" => $project->id]) }}" class="link-secondary link-underline-opacity-0">
                        <li class="btn btn-light rounded-circle m-2 fs-2 text-secondary" style="width: 60px; height: 60px">
                            {{ substr($project->name, 0, 1) }}
                        </li>
                    </a>
                @endif
            @endforeach
        </ul>
    </nav>
@endif

