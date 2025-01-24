@extends('layouts.app')

@section('content')
    <section class="container my-5">
        <h2 class="text-center mb-4" style="color: #6c757d;">Monitor Projects</h2>

        <!-- Search Bar -->
        <section class="row mb-4">
            <div class="col-12">
                <input type="text" id="project-search" placeholder="Search projects..." class="form-control search-bar">
            </div>
        </section>

        <!-- Projects Table -->
        <article class="table-responsive">
            <table id="projects-table" class="table table-striped table-hover">
                <thead class="tablek" style="background-color: #6c757d; color: white; font-size: 18px;">
                <tr>
                    <th>Project Name</th>
                    <th>Detail</th>
                    <th>Actions</th>
                </tr>
                </thead>
                <tbody>
                @foreach ($projects as $project)
                    <tr>
                        <td style="color: #5A5A5A;">{{ $project->name }}</td>
                        <td style="color: #5A5A5A;">{{ $project->detail }}</td>
                        <td>
                            <a href="{{ route('administrator.show-members', ['project' => $project->id]) }}" class="btn btn-info btn-md" style="background-color: #6c757d; border-color: #6c757d; color: white; font-size: 18px;">View Members</a>
                        </td>
                    </tr>
                @endforeach
                </tbody>
            </table>
        </article>
    </section>
@endsection

@section('scripts')
    <script src="{{ url('js/app.js') }}"></script>
@endsection
