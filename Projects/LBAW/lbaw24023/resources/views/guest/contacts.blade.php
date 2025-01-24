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

    <main class="p-5" style="background-color: #e0e0e0; --bs-gutter-x: 0;">
        <h1> Having troubles? </h1>
        <h4> Contact us via one of our communication channels. </h4>
        <article class="d-flex w-100 justify-content-between p-5">
            <div>
                <h2><strong> Email </strong></h2>
                <p><a href="mailto:synctask@example.com"> synctask@example.com </a></p> <!-- Placeholder email -->
            </div>
            <div>
                <h2><strong> Phone </strong></h2>
                <p> (0)152-28817386 </p> <!-- See https://en.wikipedia.org/wiki/Fictitious_telephone_number -->
            </div>
            <div>
                <h2><strong> Headquarters </strong></h2>
                <p> Rua Dr. Roberto Frias, s/n <br> <!-- FEUP address as placeholder -->
                    4200-465 Porto <br>
                    Portugal </p>
            </div>
    </main>

    @include('components.footer')
</body>
</html>