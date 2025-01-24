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


    <script type="text/javascript">
        // Fix for Firefox autofocus CSS bug
        // See: http://stackoverflow.com/questions/18943276/html-5-autofocus-messes-up-css-loading/18945951#18945951
    </script>
    <script type="text/javascript" src={{ url('js/app.js') }} defer>
    </script>
</head>

<body>
    @include('components.header')

    <main class="p-5" style="background-color: #e0e0e0; --bs-gutter-x: 0">
        <section id="about" class="p-5 d-flex w-100 justify-content-between">
            <div style="max-width: 60%;">
                <h1> SyncTask is </h1>
                <h4> a web app to manage projects for small teams, where everyone is up to date. </h4>
                <p>Sometimes, synchronizing the tasks with your team can be a hassle. Many times, the asymmetric delivery of information contributes to the misalignment of the project vision between peers. Well, here is where this is done easier.</p>
                <p>SyncTask allows workers to create a list of tasks to be done, providing features for communicating in the context of that task. The coordinators can assign workers to a task that can quickly be searched for with filters for date, assignee, etc. In each task, the workers can share details to keep the others updated about the status of that specific task.</p>
                <p>A non-privileged user of the app can contribute to a project as a normal worker but can also be a project coordinator, who manages tasks and workers, including sending invitations to users. There is also the administrator profile, who cannot directly intervene in the project but is privileged, privilege which allows the deletion of projects and tasks, banning users, etc. Every authenticated user can edit their own profile and see the profile of every team member. A guest has access to all public pages about SyncTask (like everybody else), as well as the ability to register and sign in with an email or username and password.</p>
            </div>
            <img src={{ url('images/pitch.png') }} alt="Task icon abstraction" class="img-fluid" style="width: 20%; height: 20%;">
        </section>

        <section id="team" class="p-5">
            <h2> The Team behind </h2>
            <ul class="list-group list-group-flush">
                <li class="list-group"> <a class="text-decoration-none me-3" href="mailto:up202207987@up.pt">Afonso Fernandes</a> </li>
                <li class="list-group"> <a class="text-decoration-none me-3" href="mailto:up202208319@up.pt">Duarte Assunção</a> </li>
                <li class="list-group"> <a class="text-decoration-none me-3" href="mailto:up202208755@up.pt">Guilherme Matos</a> </li>
                <li class="list-group"> <a class="text-decoration-none me-3" href="mailto:up202208393@up.pt">João Ferreira</a> </li>
            </ul>
        </section>

        <section id="credits" class="p-5">
            <h2> Credits </h2>
            <p> In the context of the curricular unit "Database and Web Applications Laboratory" of the L.EIC course in FEUP, this project was developed by group 24023 with the help of Sérgio Sobral Nunes (Regent teacher) and Rúben Filipe Seabra de Almeida (Laboratory teacher). </p>
            <p><a href="https://www.flaticon.com/free-icons/notification" title="notification icons">Notification icons created by Tempo_doloe - Flaticon</a></p>
            <p><a href="https://www.flaticon.com/free-icons/tasks" title="tasks icons">Tasks icons created by Freepik - Flaticon</a></p>
        </section>

    </main>

    @include('components.footer')
</body>

</html>