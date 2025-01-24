<!DOCTYPE html>
<html lang="{{ app()->getLocale() }}" style="background-color: #e0e0e0;">

<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">

    <!-- CSRF Token -->
    <meta name="csrf-token" content="{{ csrf_token() }}">

    <title>{{ config('app.name', 'Laravel') }}</title>

    <!-- Styles -->
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha3/dist/css/bootstrap.min.css" rel="stylesheet">
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha3/dist/js/bootstrap.bundle.min.js" defer></script>

    <style>
        @media (min-width: 1200px) {
            .responsive-img {
                width: 50%;
            }
        }
    </style>

    <script type="text/javascript">
        // Fix for Firefox autofocus CSS bug
        // See: http://stackoverflow.com/questions/18943276/html-5-autofocus-messes-up-css-loading/18945951#18945951
    </script>
    <script type="text/javascript" src={{ url('js/app.js') }} defer>
    </script>
</head>

<body>
    @include('components.header')

    <main class="p-5" style="background-color: #e0e0e0; --bs-gutter-x: 0;">
        <section id="pitch" class="d-flex w-100 justify-content-between p-5" style="max-height: 250px;">
            <div>
                <h1><strong> SyncTask </strong></h1>
                <h3> Project management done easier, </h3>
                <h3> keeping everyone up to date </h3>
            </div>
            <img src={{ url('images/pitch.png') }} alt="Task icon abstraction" class="img-fluid flex-column flex-lg-row" style="object-fit: contain;">
        </section>

        <section id="features" class="p-5">
            <article class="d-flex w-100 justify-content-between p-5 flex-column flex-xl-row ">
                <div>
                    <h2><strong> Automatic Notifications </strong></h2>
                    <h4> Never miss a project update again. </h4>
                </div>
                <img src={{ url('images/notifications.png') }} alt="Preview of the notifications in SyncTask" class="img-fluid responsive-img" style="object-fit: contain;">
            </article>
            <article class="d-flex w-100 justify-content-between p-5 flex-column flex-xl-row">
                <div>
                    <h2><strong> Task Management </strong></h2>
                    <h4> Organize your tasks the way your team likes best. </h4>
                </div>
                <img src={{ url('images/tasks.png') }} alt="Preview of the tasks functionality of SyncTask" class="img-fluid responsive-img" style="object-fit: contain;">
            </article>
            <article class="d-flex w-100 justify-content-between p-5 flex-column flex-xl-row">
                <div>
                    <h2><strong> Project Catalog </strong></h2>
                    <h4> Keep track of all your projects; favorites or not. </h4>
                </div>
                <img src={{ url('images/projects.png') }} alt="Preview of the projects page in SyncTask" class="img-fluid responsive-img" style="object-fit: contain;">
            </article>
        </section>
        <section id="register" class="bg-secondary text-white p-5 d-flex justify-content-between">
            <h4> Ready to never miss a memo again? </h4>
            <a href="{{ route('register') }}" class="btn btn-light"> Register now! </a>
        </section>
    </main>

    @include('components.footer')
</body>

</html>