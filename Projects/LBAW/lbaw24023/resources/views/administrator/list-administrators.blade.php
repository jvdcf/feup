@extends('layouts.app')

@section('content')
    <section class="container my-5">
        <h2 class="text-center mb-4" style="color: #6c757d;">Administrators</h2>

        <header class="d-flex justify-content-end mb-3">
            <a href="{{ route('administrator.create') }}" class="btn btn-primary" style="background-color: #6c757d; border-color: #6c757d; font-size: 18px;">Create Administrator</a>
        </header>

        <article class="table-responsive">
            <table class="table table-striped table-hover">
                <thead class="tablek" style="background-color: #6c757d; color: white; font-size: 18px;">
                <tr>
                    <th>Username</th>
                    <th>Name</th>
                    <th>Email</th>
                    <th>Photo</th>
                    <th style="text-align: center">Actions</th>
                </tr>
                </thead>
                <tbody>
                @foreach ($administrators as $administrator)
                    <tr style="font-size: 18px;">
                        <td style="color: #5A5A5A;">{{ $administrator->username }}</td>
                        <td style="color: #5A5A5A;">{{ $administrator->name }}</td>
                        <td style="color: #5A5A5A;">{{ $administrator->email }}</td>
                        <td style="color: #5A5A5A;">
                            @if (isset($administrator->photo) && $administrator->photo)
                                <img src="{{ route('photos.show', ['filename' => basename($administrator->photo)]) }}" alt="User Photo" class="rounded-circle" style="width: 50px; height: 50px; object-fit: cover; background-color: #e0e0e0;">
                            @else
                                <div class="rounded-circle d-flex justify-content-center align-items-center" style="width: 50px; height: 50px; background-color: #4b5563; color: white; font-size: 1.5rem;">
                                    {{ strtoupper(substr($administrator->name, 0, 1)) }}
                                </div>
                            @endif
                        </td>
                        <td>
                            <section class="d-flex flex-wrap justify-content-center gap-2">
                                <a href="{{ route('administrator.edit', $administrator) }}" class="btn btn-warning btn-md" style="background-color: #6c757d; border-color: #6c757d; color: white; font-size: 18px;">Edit</a>
                                <form action="{{ route('administrator.destroy', $administrator) }}" method="POST" style="display:inline;">
                                    @csrf
                                    @method('DELETE')
                                    <button type="submit" class="btn btn-danger btn-md" style="background-color: #6c757d; border-color: #6c757d; color: white; font-size: 18px;" onclick="return confirm('Are you sure you want to delete this administrator?')">Delete</button>
                                </form>
                                <a href="{{ route('administrator.show', $administrator) }}" class="btn btn-info btn-md" style="background-color: #6c757d; border-color: #6c757d; color: white; font-size: 18px;">View Profile</a>
                            </section>
                        </td>
                    </tr>
                @endforeach
                </tbody>
            </table>
        </article>
    </section>
@endsection
