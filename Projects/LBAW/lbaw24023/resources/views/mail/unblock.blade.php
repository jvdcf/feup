<!DOCTYPE html>
<html>
<head>
    <title>Unblocked Account</title>
</head>
<body>
<h1>Unblocked Account</h1>
<p>Hello {{ $user->name }},</p>
<p>Your account has been unblocked by the administrator.</p>
<p>You can now log in and continue using our services.</p>
<p>
    <a href="{{ url('/login') }}" style="display: inline-block; padding: 10px 20px; color: white; background-color: #4b5563; text-decoration: none;">Login</a>
</p>
</body>
</html>