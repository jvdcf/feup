@extends('layouts.app')

@section('content')
    <section class="container my-5">
        <h2 class="text-center mb-4" style="color: #6c757d;">Project Coordinators</h2>
        <ul class="list-unstyled">
            @foreach ($coordinators as $coordinator)
                <li class="d-flex justify-content-center align-items-center mb-2" style="color: #5A5A5A;">
                    <a href="{{ route('users.show', ['user' => $coordinator->id]) }}" style="color: #5A5A5A; text-decoration: none;">
                        {{ $coordinator->name }} ({{ $coordinator->email }})
                    </a>
                </li>
            @endforeach
        </ul>
    </section>

    <section class="container my-5">
        <h2 class="text-center mb-4" style="color: #6c757d;">Project Members</h2>
        <ul class="list-unstyled">
            @foreach ($members as $member)
                <li class="d-flex justify-content-center align-items-center mb-2" style="color: #5A5A5A;">
                    <a href="{{ route('users.show', ['user' => $member->id]) }}" style="color: #5A5A5A; text-decoration: none;">
                        {{ $member->name }} ({{ $member->email }})
                    </a>
                </li>
            @endforeach
        </ul>
    </section>
@endsection
